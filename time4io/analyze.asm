  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.

	.text
main:
	li	$s0,0x30        # ASCII table 0x30 is 0
loop:
	move	$a0,$s0		# copy from s0 to a0, a0 = s0 = 0x30
	
	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window, print it out ( print a0 )

	addi	$s0,$s0,3	# what happens if the constant is changed? If the constant is 1, it will print out one by one
						# s0 = s0 + 3 
	li	$t0,0x5b        # ASCII table 0x5b is ]
	ble	$s0,$t0,loop    # if s0 is less or equal to t0, go loop
	nop			# delay slot filler (just in case)

stop:	j	stop		# loop forever here
	nop			# delay slot filler (just in case)

