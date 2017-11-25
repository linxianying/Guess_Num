  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop			
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop			# delay slot filler (just in case)
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop			# delay slot filler (just in case)
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop			# delay slot filler (just in case)
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop			# delay slot filler (just in case)
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop			# delay slot filler (just in case)

  # you can write your code for subroutine "hexasc" below this line
  #
delay:
	li	$a2,10    # a2 = ms = 10
	li	$t0,4711      # t0 = 4711
	loop: 	
		blt	$a2, $0, finish # if the ms < 0, end the loop
		nop			# delay slot filler (just in case)
		addi	$a2, $a2,-1    # ms = ms ¨C 1;
		addi	$t3, $t3, 0    # t3 = i = 0
		looptwo:
			addi	$t3, $t3, 1    # i = i + 1
			ble	$t3, $t0, looptwo	# if the i is less or equal to 4711, go to the looptwo again
			nop			# delay slot filler (just in case)
		j loop
		nop			# delay slot filler (just in case)
finish:
	jr 	$ra
	nop			# delay slot filler (just in case)

hexasc:
	sll 	$a0, $a0, 28
	srl 	$a0, $a0, 28	# we only need the last 4 bits, shifting left and shifting right can remove all other bits
	
	addi	$t0, $0, 0x09    
	ble	$a0, $t0,single	# if the number is less than 10 
	nop                    # delay slot filler (just in case)
	
	addi	$v0, $v0, 0x37 # if the number is more than 9, add 7 because there are 7 characters between A and 9
 
single:
	add	$v0, $a0, 0x30
	jr	$ra
	nop                    # delay slot filler (just in case)

time2string:
	move    $t0, $a1   # get time string, t0 from a1
	move    $t7, $a0
	sll 	$t0, $t0, 16   # we only need the last 16 bits, shifting left and shifting right can remove all other bits
	srl 	$t0, $t0, 16   # time info(equivalent to andi $t0, $a1, 0xffff) 
	
	andi    $t1, $t0, 0x000f  # make t1 the last 4 bits, which is the second's 2nd digit
	andi    $t2, $t0, 0x00f0  # make t2 to represent the second's 1st digit
	andi    $t3, $t0, 0x0f00  # make t3 to represent the minute's 2nd digit
	andi    $t4, $t0, 0xf000  # make t4 to represent the minute's 1st digit
	
	srl     $t2, $t2, 4   # make the useful time infomation in the last 4 bits 
	srl     $t3, $t3, 8
	srl     $t4, $t4, 12

	PUSH	($ra)
	move 	$a0, $t4	# F0:00
	jal 	hexasc
	nop			# delay slot filler (just in case)
	POP	($ra)
	sb 	$v0, 0($t7)

	PUSH	($ra)		# 0F:00
	move 	$a0, $t3
	jal 	hexasc
	nop			# delay slot filler (just in case)
	POP	($ra)
	sb 	$v0, 1($t7)	
	
	addi 	$t5, $0, 0x3a   # add the colon, which is 0x3a in ACSII
	sb	$t5, 2($t7)
	
	PUSH($ra)
	move 	$a0, $t2	# 00:F0
	jal 	hexasc
	nop			# delay slot filler (just in case)
	POP($ra)
	sb 	$v0, 3($t7)	
	
	PUSH($ra)		# 00:0F
	move 	$a0, $t1
	jal 	hexasc
	nop			# delay slot filler (just in case)
	POP($ra)
	sb 	$v0, 4($t7)
		
	addi 	$t6, $0, 0x00    # add the NULL byte, which is 0x00 in ACSII
	sb	$t6, 5($t7)
	
	jr	$ra
	nop			# delay slot filler (just in case)
