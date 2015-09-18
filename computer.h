
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

typedef struct {
	int RegDst;
	int Jump;
	int Branch;
	int MemRead;
	int MemtoReg;
	int ALUOp;
	int MemWrite;
	int ALUSrc;
	int RegWrite;
} Control;

typedef enum {
	j = 0x02, 
	jal = 0x03, 
	beq = 0x04, 
	bne = 0x05, 
	addiu = 0x09, 
	andi = 0x0c, 
	ori = 0x0d, 
	lui = 0x0f, 
	lw = 0x23, 
	sw = 0x2b
} opcode;

typedef enum {
	sll = 0x00, 
	srl = 0x02, 
	jr = 0x08, 
	addu = 0x21, 
	subu = 0x23, 
	and = 0x24, 
	or = 0x25, 
	slt = 0x2a, 
} funct;

typedef enum {
	opcode	= 0b
		1111
		1100
		0000
		0000
		0000
		0000	
		0000
		0000, 
		
	rs	= 0b
		0000
		0011
		1110
		0000
		0000
		0000	
		0000
		0000, 
	rt	= 0b
		0000
		0000
		0001
		1111
		0000	
		0000
		0000, 
	rd	= 0b
		0000
		0000
		0000
		0000
		1111
		1000	
		0000
		0000, 
	shamt	= 0b
		0000
		0000
		0000
		0000
		0000
		0111	
		1100
		0000, 
	funct	= 0b
		0000
		0000
		0000
		0000
		0000
		0000	
		0011
		1111, 
	imm	= 0b\
		0000
		0000
		0000
		0000
		1111
		1111	
		1111
		1111, 
	addr	= 0b0000
		    0000
		    0011
		    1111
		    1111
		    1111
		    1111	
		    1111
		    1111 
} field;
