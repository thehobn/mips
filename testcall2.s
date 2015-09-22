# this will run a short test on all the assigned commands.
#
addiu $t0, $0, 0	# put 0 in $t0 (0x0)
addiu $t1, $0, 1	# put 1 in $t1 (0x2)
addiu $t2, $0, 3	# put 1 in $t2 (0x3)
#slt $t2, $t0, $t1	# check if $t0 is less than $t1, store result in $t2 (0x1)  !!!!!! Problem - terminates
accumulate:
addiu $t0, $t0, 1	# add 1 to $t0
beq $t0, $t1, accumulate	# check if $t0 is equal to $t1, if so, go back !!!!!! Problem - segfault
bne $t0, $t2, accumulate	# check if $t0 is equal to $t2, if so, go back 
addi	$0,$0,0 	# unsupported instruction, terminate
jal illbeback		# jump to the illbeback segment store return address in $31
j finishup			# jump over the illbeback segment

illbeback:
addiu $s0, $0, 3	# put 99 in $s0 (0x63)
jr $31			# jump back

finishup:
# addiu $t0, $0, 0x00403ffc 	# put a base memory location in $t0 (last possible word)
addiu $t0, $0, 0x00401000 	# put a base memory location in $t0 (first possible word)
# addiu $t0, $0, 0 	# put a base memory location in $t0 (too low)
sw $s0, 0($t0)		# store the contents of $s0 at the indicated mem location
lw $s1, 0($t0)		# copy the word back from mem to $s1


addi	$0,$0,0 #unsupported instruction, terminate
