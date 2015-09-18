#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "computer.h"
#undef mips			/* gcc already has a def for mips */

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);
void Decode (unsigned int, DecodedInstr*, RegVals*);
int Execute (DecodedInstr*, RegVals*);
int Mem(DecodedInstr*, int, int *);
void RegWrite(DecodedInstr*, int, int *);
void UpdatePC(DecodedInstr*, int);
void PrintInstruction (DecodedInstr*);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;
Control brain;

/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments govern how the program interacts with the user.
 */
void InitComputer (FILE* filein, int printingRegisters, int printingMemory,
  int debugging, int interactive) {
    int k;
    unsigned int instr;

    /* Initialize registers and memory */

    for (k=0; k<32; k++) {
        mips.registers[k] = 0;
    }
    
    /* stack pointer - Initialize to highest address of data segment */
    mips.registers[29] = 0x00400000 + (MAXNUMINSTRS+MAXNUMDATA)*4;

    for (k=0; k<MAXNUMINSTRS+MAXNUMDATA; k++) {
        mips.memory[k] = 0;
    }

    k = 0;
    while (fread(&instr, 4, 1, filein)) {
	/*swap to big endian, convert to host byte order. Ignore this.*/
        mips.memory[k] = ntohl(endianSwap(instr));
        k++;
        if (k>MAXNUMINSTRS) {
            fprintf (stderr, "Program too big.\n");
            exit (1);
        }
    }

    mips.printingRegisters = printingRegisters;
    mips.printingMemory = printingMemory;
    mips.interactive = interactive;
    mips.debugging = debugging;
}

unsigned int endianSwap(unsigned int i) {
    return (i>>24)|(i>>8&0x0000ff00)|(i<<8&0x00ff0000)|(i<<24);
}

/*
 *  Run the simulation.
 */
void Simulate () {
    char s[40];  /* used for handling interactive input */
    unsigned int instr;
    int changedReg=-1, changedMem=-1, val;
    DecodedInstr d;
    
    /* Initialize the PC to the start of the code section */
    mips.pc = 0x00400000;
    while (1) {
        if (mips.interactive) {
            printf ("> ");
            fgets (s,sizeof(s),stdin);
            if (s[0] == 'q') {
                return;
            }
        }

        /* Fetch instr at mips.pc, returning it in instr */
        instr = Fetch (mips.pc);

        printf ("Executing instruction at %8.8x: %8.8x\n", mips.pc, instr);

        /* 
	 * Decode instr, putting decoded instr in d
	 * Note that we reuse the d struct for each instruction.
	 */
        Decode (instr, &d, &rVals);

        /*Print decoded instruction*/
        PrintInstruction(&d);

        /* 
	 * Perform computation needed to execute d, returning computed value 
	 * in val 
	 */
        val = Execute(&d, &rVals);

	UpdatePC(&d,val);

        /* 
	 * Perform memory load or store. Place the
	 * address of any updated memory in *changedMem, 
	 * otherwise put -1 in *changedMem. 
	 * Return any memory value that is read, otherwise return -1.
         */
        val = Mem(&d, val, &changedMem);

        /* 
	 * Write back to register. If the instruction modified a register--
	 * (including jal, which modifies $ra) --
         * put the index of the modified register in *changedReg,
         * otherwise put -1 in *changedReg.
         */
        RegWrite(&d, val, &changedReg);

        PrintInfo (changedReg, changedMem);
    }
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated, otherwise -1.
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction, otherwise -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void PrintInfo ( int changedReg, int changedMem) {
    int k, addr;
    printf ("New pc = %8.8x\n", mips.pc);
    if (!mips.printingRegisters && changedReg == -1) {
        printf ("No register was updated.\n");
    } else if (!mips.printingRegisters) {
        printf ("Updated r%2.2d to %8.8x\n",
        changedReg, mips.registers[changedReg]);
    } else {
        for (k=0; k<32; k++) {
            printf ("r%2.2d: %8.8x  ", k, mips.registers[k]);
            if ((k+1)%4 == 0) {
                printf ("\n");
            }
        }
    }
    if (!mips.printingMemory && changedMem == -1) {
        printf ("No memory location was updated.\n");
    } else if (!mips.printingMemory) {
        printf ("Updated memory at address %8.8x to %8.8x\n",
        changedMem, Fetch (changedMem));
    } else {
        printf ("Nonzero memory\n");
        printf ("ADDR	  CONTENTS\n");
        for (addr = 0x00400000+4*MAXNUMINSTRS;
             addr < 0x00400000+4*(MAXNUMINSTRS+MAXNUMDATA);
             addr = addr+4) {
            if (Fetch (addr) != 0) {
                printf ("%8.8x  %8.8x\n", addr, Fetch (addr));
            }
        }
    }
}

/*
 *  Return the contents of memory at the given address. Simulates
 *  instruction fetch. 
 */
unsigned int Fetch ( int addr) {
    return mips.memory[(addr-0x00400000)/4];
}

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
	// Parse instr	
		d->op = instr >> 26;
		switch (d->op) {
			// Bitmask to get fields
			case 0: // R-type:
				// opcode	rs	rt	rd	shamt	funct
				// 31-26	25-21	20-16	15-11	10-6	5-0	
				d->type = R;	
				d->regs.r.rs = (instr & 0x03E00000) >> 21;
				d->regs.r.rt = (instr & 0x001F0000) >> 16;
				d->regs.r.rd = (instr & 0x0000F800) >> 11;
				d->regs.r.shamt = (instr & 0x000007C0) >> 6;
				d->regs.r.funct = instr & 0x0000003F;		
				rVals->R_rs = mips.registers[d->regs.r.rs];
				rVals->R_rt = mips.registers[d->regs.r.rt];
			break;	

			case 3: // Jump and link	
				// Set $ra
			case 2:	// Jumps
				// opcode	addr
				// 31-26	26-0
				d->type = J;
				d->regs.j.target = instr & 0x03FFFFFF;
			break;

			default:// I-type:
				// opcode	rs	rt	imm
				// 31-26	25-21	20-16	15-0
				d->type = I;
				d->regs.i.rs = (instr & 0x03E00000) >> 21;
				d->regs.i.rt = (instr & 0x001F0000) >> 16;
				d->regs.i.addr_or_immed = instr & 0x0000FFFF;
				rVals->R_rs = mips.registers[d->regs.i.rs];
			break;
		}
	// Fill RegVals
	// Extend immediates
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) {
    /* Your code goes here */	
	char* i;
	int rd, rs, rt, imm;
	switch (d->op) {
		case 0:
			switch (d->regs.r.funct) {
				case 0x00: // sll
					i = "sll"; break;
				case 0x02: // srl
					i = "srl"; break;
				case 0x08: // jr
					i = "jr"; break;
				case 0x21: // addu
					i = "addu"; break;
				case 0x23: // subu
					i = "subu"; break;
				case 0x24: // and
					i = "and"; break;
				case 0x25: // or
					i = "or"; break;
				case 0x2a: // slt
					i = "slt";
				default:
					exit(0); break;
			} break;
		case 0x2: // j	
			i = "j"; break;
		case 0x3: // jal
			i = "jal"; break;
		case 0x04: // beq
			i = "beq"; break;
		case 0x05: // bne
			i = "bne"; break;
		case 0x09: // addiu
			i = "addiu"; break;
		case 0x0c: // andi
			i = "andi"; break;
		case 0x0d: // ori
			i = "ori"; break;
		case 0x0f: // lui
			i = "lui"; break;
		case 0x23: // lw
			i = "lw"; break;
		case 0x2b: // sw
			i = "sw"; break;
		default:
			exit(0); break;	
	}
	if (d->type == R) {
		rd = d->regs.r.rd;
		rs = d->regs.r.rs;
		rt = d->regs.r.rt;
		if (d->regs.r.funct == 0x00 || d->regs.r.funct == 0x02) {
			imm = d->regs.r.shamt;
			printf("%s\t$%d, $%d, %d\n", i, rt, rs, imm); // addiu, srl, sll
		}
		else if (d->regs.r.funct == 0x08)
			printf("%s \t$%d\n", i, rs); //jr
		else
			printf("%s\t$%d, $%d, $%d\n", i, rd, rs, rt);
	}
	else if (d->type == I) {
		rs = d->regs.i.rs;
		rt = d->regs.i.rt;
		imm = d->regs.i.addr_or_immed;
		if (d->op == 0x09)
			printf("%s\t$%d, $%d, %d\n", i, rt, rs, imm); // addiu, srl, sll
		else if (d->op == 0x0c || d->op == 0x0d || d->op == 0x0f)
			printf("%s\t$%d, $%d, 0x%x\n", i, rt, rs, imm); // andi, ori, lui
		else if (d->op == 0x23 || d->op == 0x2b)
			printf("%s\t$%d, $%d($%d)\n", i, rt, rs, imm); // lw, sw
		else if (d->op == 0x04 || d->op == 0x05)
			printf("%s\t$%d, $%d, 0x%0.8x\n", i, rt, rs, mips.pc + imm * 4); // branch
	}
	else if (d->type == J) {
		imm = d->regs.j.target << 2;
		printf("%s\t0x%0.8x\n", i, imm); // jump
	}

	/*
addu
addiu
subu
sll
srl
and
andi
or
ori
lui
slt
beq
bne
j
jal
jr
lw
sw
	*/
}

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
	// Return ALU result (including lw, branch comparison, etc)
	switch (d->op) {
		case 0:
			switch (d->regs.r.funct) {
				case 0x00: // sll
					return rVals->R_rd = rVals->R_rt << d->regs.r.shamt; break;
				case 0x02: // srl
					return rVals->R_rd = rVals->R_rt >> d->regs.r.shamt; break;
				// case 0x03: // sra
				// 	return rVals->R_rd = rVals->R_rt >> d->regs.r.shamt + rVals->R_rt & 0x10000000; break;
				// case 0x04: // sllv
				//	return rVals->R_rd = rVals->R_rt << rVals->R_rs; break;
				// case 0x06: // srlv
				//	return rVals->R_rd = rVals->R_rt >> rVals->R_rs; break;
				// case 0x07: // srav
				//	return rVals->R_rd = rVals->R_rt >> rVals->R_rs + rVals->R_rt & 0x10000000; break;
				case 0x08: // jr
					return rVals->R_rs; break;
				// case 0x20: // add	
				case 0x21: // addu
					return rVals->R_rd = rVals->R_rs + rVals->R_rt; break;
				// case 0x22: // sub
				case 0x23: // subu
					return rVals->R_rd = rVals->R_rs - rVals->R_rt; break;
				case 0x24: // and
					return rVals->R_rd = rVals->R_rs & rVals->R_rt; break;
				case 0x25: // or
					return rVals->R_rd = rVals->R_rs | rVals->R_rt; break;
				// case 0x26: // xor
				//	return rVals->R_rd = rVals->R_rs ^ rVals->R_rt; break;
				// case 0x27: // nor
				//	return rVals->R_rd = !rVals->R_rs & !rVals->R_rt; break;
				case 0x2a: // slt
				// case 0x2b: // sltu
					return rVals->R_rd = rVals->R_rs < rVals->R_rt; break;
				default:
					exit(0); break;

			} break;
		case 2: // j
			break;
		case 3: // jal
			return mips.pc + 4;
		case 0x04: // beq
			return rVals->R_rs == rVals->R_rt;
		case 0x05: // bne
			return rVals->R_rs != rVals->R_rt;
		// case 0x08: // addi		
		case 0x09: // addiu
			return rVals->R_rt = rVals->R_rs + d->regs.i.addr_or_immed;
		// case 0x0a: // slti
		// case 0x0b: // sltiu
		// 	return rVals->R_rt = rVals->R_rs < d->regs.i.addr_or_immed;
		case 0x0c: // andi
			return rVals->R_rt = rVals->R_rs & d->regs.i.addr_or_immed;
		case 0x0d: // ori
			return rVals->R_rt = rVals->R_rs | d->regs.i.addr_or_immed;
		case 0x0f: // lui
			break;
		case 0x23: // lw
		// case 0x24: // lbu
		// case 0x25: // lhu
		// case 0x28: // sb
		// case 0x29: // sh
		case 0x2b: // sw
		// case 0x30: // ll
		// case 0x38: // sc
			return rVals->R_rs + d->regs.i.addr_or_immed; break;
		default:
			exit(0); break;
	}
  return 0;
}

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc += 4;
    /* Your code goes here */
	if (d->type == R && d->regs.r.funct == 0x08)
		mips.pc = mips.registers[d->regs.r.rs];
	else if (d->type == I && (d->op == 0x04 || d->op == 0x05)) {
		if (val)
			mips.pc += d->regs.i.addr_or_immed * 4;
	}
	else if (d->type == J)	
		mips.pc = d->regs.j.target << 2;

}

/*
 * Perform memory load or store. Place the address of any updated memory 
 * in *changedMem, otherwise put -1 in *changedMem. Return any memory value 
 * that is read, otherwise return -1. 
 *
 * Remember that we're mapping MIPS addresses to indices in the mips.memory 
 * array. mips.memory[0] corresponds with address 0x00400000, mips.memory[1] 
 * with address 0x00400004, and so forth.
 *
 */
int Mem( DecodedInstr* d, int val, int *changedMem) {
    /* Your code goes here */
	// Read/write memory
	// 0x00400000 is index 0
	// If outside 0x00401000 to 0x00403fff or lw or sw and not word aligned
		// Print specific error message
		// Call exit(0);
	if ((d->op == 0x23 || d->op == 0x2b) && ((d->regs.i.addr_or_immed < 0x00401000 || d->regs.i.addr_or_immed > 0x00403fff) || d->regs.i.addr_or_immed % 4 !=0)) {
		printf("Memory Access Exception at 0x%0.8x: address 0x%0.8x", mips.pc, d->regs.i.addr_or_immed);
		exit(0);
	}
	*changedMem = -1;
	switch (d->op) {
		// case 0x24: // lbu
		// case 0x25: // lhu
		// case 0x30: // ll
		case 0x23: // lw
			return mips.memory[val / 4 - 0x00400000];
		// case 0x28: // sb
		// case 0x38: // sc
		// case 0x29: // sh
		case 0x2b: // sw
			*changedMem = val;
			mips.memory[val / 4 - 0x00400000] = mips.registers[d->regs.i.rt]; break;
	}
	return val;
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    /* Your code goes here */
	switch (d->op) {
		case 0:
			switch (d->regs.r.funct) {
				case 0x00: // sll
				case 0x02: // srl	
				case 0x21: // addu
				case 0x23: // subu
				case 0x24: // and
				case 0x25: // or
				case 0x2a: // slt
					*changedReg = d->regs.r.rd;
					mips.registers[d->regs.r.rd] = val;
					break;
				case 0x08: //jr
					*changedReg = -1; break;
				default:
					exit(0); break;
			} break;
		case 0x03: // jal
			*changedReg = 31;
			mips.registers[31] = val;
			break;
		case 0x09: // addiu
		case 0x0c: // andi
		case 0x0d: // ori
			*changedReg = d->regs.i.rt;
			mips.registers[d->regs.i.rt] = val;
			break;
		case 0x0f: // lui
			*changedReg = d->regs.i.rt;
			mips.registers[d->regs.i.rt] &= val;
			break;
		case 0x23: // lw
			*changedReg = d->regs.r.rt;
			mips.registers[d->regs.i.rt] = val;
			break;
		case 0x2b: // sw
		case 0x02: // j
		case 0x04: // beq
		case 0x05: // bne
			*changedReg = -1; break;
		default:	
			exit(0); break;	
	}
}
