addiu $t0, $t0, -4	# subtract 4 from $t0 (0xfffffffc)
addiu $t0, $t0, -4	# subtract 4 from $t0 (0xfffffff8)
addiu $t0, $t0, 12	# add 12 to $t0 (0x4)
addiu $t0, $t0, 12	# add 12 to $t0 (0x10)

addi	$0,$0,0 #unsupported instruction, terminate
