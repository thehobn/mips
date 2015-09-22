# this will run a short test on all the assigned commands.
#

addiu $t1, $0, 32	# put 32 in $t1 (0x20)
addiu $t2, $0, 6	# put 6 in $t2	(0x6)
addu $t3, $t1, $t2	# add both and put in $t3 (0x26)
subu $t4, $t1, $t2	# subtract $t2 from $t1 and put in t4 (0x1A)
subu $t5, $t2, $t1	# subtract $t1 from $t2 and put in t5 (0xffffffe6)
sll $t6, $t3, 3		# shift left by 3 the amount in $t3 and put in t6 (0x130)
srl $t7, $t3, 3		# shift right by 3 the amount in $t3 and put in t7 (0x4)
and $t8, $t4, $t5	# bitwise AND $t4 and $t5 put in t8 (0x2)
andi $t9, $t4, 15	# bitwise AND $t4 and 15 (0xFFFF) put in t9 (0xA)

or $t0, $t3, $t4	# bitwise OR $t3 and $t4 put in t0 (0x2E)
ori $t1, $t3, 17	# bitwise OR $t3 and $t4 put in t1 (0x37)
lui $t2, 22		# take 22 (0x16), shift left by 16 bits and store in $t2 (0x160000)
slt $t3, $t0, $t1	# check if $t0 is less than $t1, store result in $t3 (0x1)
#beq Rsrc1, Rsrc2, raddr
#bne Rsrc1, Rsrc2, raddr
#j address
#jal address
#jr Rsrc
#lw Rdest, offset (Radd)
#sw Rsrc, offset (Radd)

addi	$0,$0,0 #unsupported instruction, terminate
