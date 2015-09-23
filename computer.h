#define MAXNUMINSTRS 1024	/* max # instrs in a program */
#define MAXNUMDATA 3072		/* max # data words */

struct SimulatedComputer {
    int memory [MAXNUMINSTRS+MAXNUMDATA];
    int registers [32];
    int pc;
    int printingRegisters, printingMemory, interactive, debugging;
};
typedef struct SimulatedComputer Computer;

typedef enum { R=0, I, J } InstrType;

typedef struct {
  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
} RRegs;

typedef struct {
  int rs;
  int rt;
  int addr_or_immed;
} IRegs;

typedef struct {
  int target;
} JRegs;

typedef struct {
  InstrType type;
  int op;
  union {
    RRegs r;
    IRegs i;
    JRegs j;
  } regs;
} DecodedInstr;

typedef struct {
  int R_rs; /*Value in register rs*/
  int R_rt;
  int R_rd;
} RegVals;

void InitComputer (FILE*, int printingRegisters, int printingMemory,
    int debugging, int interactive);
void Simulate ();

typedef enum {	
	j = 0x02, 
	jal = 0x03, 
	beq = 0x04, 
	bne = 0x05, 
//	addi = 0x08,
	addiu = 0x09, 
//	slti = 0x0a,
	andi = 0x0c, 
	ori = 0x0d, 
	lui = 0x0f, 
//	lb = 0x20,
//	lh = 0x21,
	lw = 0x23, 
//	lbu = 0x24,
//	lhu = 0x25,
//	sb = 0x28,
//	sh = 0x29,
	sw = 0x2b
} opcode;

typedef enum {
	sll = 0x00, 
	srl = 0x02, 
//	sra = 0x03,
//	sllv = 0x04,
//	srlv = 0x06,
//	srav = 0x07,
	jr = 0x08, 
//	mfhi = 0x10,
//	mflo = 0x12,
//	mult = 0x18,
//	multu = 0x19,
//	div = 0x1a,
//	divu = 0x1b,
//	add = 0x20,
	addu = 0x21, 
//	sub = 0x22,
	subu = 0x23, 
	and = 0x24, 
	or = 0x25, 
//	xor = 0x26,
//	nor = 0x27,
	slt = 0x2a
} funct;
