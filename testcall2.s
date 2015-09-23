# this will run a short test on all the assigned commands.
#
addiu $t0, $0, 0	# put 0 in $t0 (0x0)
addiu $t1, $0, 1	# put 1 in $t1 (0x1)
addiu $t2, $0, 3	# put 1 in $t2 (0x3)
slt $t3, $t2, $t0	# check if $t0 is less than $t2, store result in $t2 (0x0) 
slt $t3, $t0, $t2	# check if $t0 is less than $t2, store result in $t2 (0x1) 
accumulate:
addiu $t0, $t0, 1	# add 1 to $t0
beq $t0, $t1, accumulate	# check if $t0 is equal to $t1, if so, go back !!!!!! Problem - segfault
#bne $t0, $t2, accumulate	# check if $t0 is equal to $t2, if so, go back 
#addi	$0,$0,0 	# unsupported instruction, terminate
jal illbeback		# jump to the illbeback segment store return address in $31
j finishup			# jump over the illbeback segment

illbeback:
addiu $s0, $0, 3	# put 99 in $s0 (0x63)
jr $31			# jump back

finishup:
addiu $t0, $0, 0x00003ffc 	# put a base memory location in $t0 (last possible word)
lui $t0, 0x0040
addiu $t1, $0, 0x00001000 	# put a base memory location in $t0 (first possible word)
lui $t1, 0x0040
# addiu $t0, $0, 0 	# put a base memory location in $t0 (too low)
addiu $a0, $a0, 1
sw $s0, 0($t0)		# store the contents of $s0 at the indicated mem location
lw $s1, 0($t0)		# copy the word back from mem to $s1
sw $s0, 0($t1)		# store the contents of $s0 at the indicated mem location
lw $s1, 0($t1)		# copy the word back from mem to $s1
addiu $t2, $0, 0x00404000


addi	$0,$0,0 #unsupported instruction, terminate
