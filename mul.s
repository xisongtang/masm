main:
	addi $a0, $zero, 55
	addi $a1, $zero, 66
	jal mul_fun

	add $a0, $v0, $zero
	addi $v0, $zero, 1 
	syscall
	addi $v0, $zero, 10
	syscall

mul_fun:#a0 for a, a1 for b, v0 for result
	addi $sp, $sp, -8
	sw $t0, 0($sp)#i
	sw $t1, 4($sp)#flag
	
	add $v0, $zero, $zero#result
	add $t0, $zero, $zero
for:
	addi $t2, $zero, 32
	beq $t0, $t2, for_exit
	andi $t1, $a0, 1
	srl $a0, $a0, 1
	addi $t0, $t0, 1
	addi $t2, $zero, 1
	bne $t1, $t2, not_plus
	add $v0, $v0, $a1
not_plus:
	sll $a1, $a1, 1
	j for
for_exit:
	lw $t1, 4($sp)
	lw $t0, 0($sp)
	addi $sp, $sp, 8
	jr $ra
