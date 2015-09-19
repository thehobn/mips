
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

		//     \/  \/  \/
//	bitmask	= 0b11111100000000000000000000000000, 
		//     /\  /\  /\
		
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

		//     \/  \/  \/
//	functmask= 0b0000000000000000000000000111111, 
		//     /\  /\  /\

	sll = 0x00, 
	srl = 0x02, 
	jr = 0x08, 
	addu = 0x21, 
	subu = 0x23, 
	and = 0x24, 
	or = 0x25, 
	slt = 0x2a, 
} funct;
/*
typedef enum {
		//

		//
		//     \/  \/  \/
	rs	= 0b0000001111100000
		    0000000000000000, 
		//     /\  /\  /\
		//
		//     \/  \/  \/
	rt	= 0b0000000000011111
		    0000000000000000, 
		//     /\  /\  /\
		//
		//     \/  \/  \/
	rd	= 0b0000000000000000
		    1111100000000000, 
		//     /\  /\  /\
		//
		//     \/  \/  \/
	shamt	= 0b0000000000000000
		    0000011111000000, 
		//     /\  /\  /\
		//

		//
		//     \/  \/  \/
	imm	= 0b0000000000000000
		    1111111111111111, 
		//     /\  /\  /\
		//
		//     \/  \/  \/
	addr	= 0b0000001111111111
		    1111111111111111
		//     /\  /\  /\
		//
} field;
*/
