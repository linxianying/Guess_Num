  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window

stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  # In the function, numbers larger than 9 and numbers less than 10 should be dealt with seperately
hexasc:
	sll 	$a0, $a0, 28
	srl 	$a0, $a0, 28	# we only need the last 4 bits, shifting left and shifting right can remove all other bits
	
	addi	$t0, $0, 0x09    
	ble	$a0, $t0,single	# if the number is less than 10 , this is a taken branch
	nop                    # delay slot filler (just in case)
	
	addi	$v0, $v0, 0x37 # if the number is more than 9, add 7 because there are 7 characters between A and 9
 
single:
	add	$v0, $a0, 0x30
	jr	$ra
	nop                    # delay slot filler (just in case)
