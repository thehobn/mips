addiu $t0, $0, 0		# put 0 in $t0 (0x0)
addiu $t1, $0, 1		# put 1 in $t1 (0x1)
addiu $t2, $0, 3		# put 1 in $t2 (0x3)
accumulate:
addiu $t0, $t0, 1		# add 1 to $t0
beq $t0, $t1, accumulate	# check if $t0 is equal to $t1, if so, go back 
bne $t0, $t2, accumulate	# check if $t0 is equal to $t2, if so, go back 
jal illbeback			# jump to the illbeback segment store return address in $31
j finishup			# jump over the illbeback segment

illbeback:
addiu $s0, $0, 99		# put 99 in $s0 (0x63)
jr $31				# jump back

finishup:
addiu $s1, $0, 44		# put 99 in $s1 (0x2c)

addi	$0,$0,0 		#unsupported instruction, terminate
