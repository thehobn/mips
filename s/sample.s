# Short test case for your project.
#
# Note that this is by no means a comprehensive test!
#

		.text
00		addiu	$4,$0,3
04		addiu	$5,$0,2
08		jal	Mystery
0c		addi	$0,$0,0 #unsupported instruction, terminate


Mystery:
10		addiu	$2,$0,0
Loop:
14		beq	$4,$0,Done
18		addu	$2,$2,$5
1c		addiu	$4,$4,-1
20		j Loop
Done:	
24		jr	$31
