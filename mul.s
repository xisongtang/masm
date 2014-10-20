/*
c-style
multi-line 
comment
*/
// c-style one-line comment
# mips'style comment

CONST_DATA equ 66 
CONST_IMME equ 0
CONST_LABEL equ mul_fun
CONST_REG equ $a0
CONST_OP equ beq

.data
IntVtTable	.4byte	0,	0+1 * 2 + ~5*(3+017>>>2)	//int 00, 01
.4byte	0,	0	//int 02, 03
.4byte	0,	0	//int 04, 05
.4byte	0,	0	//int 06, 07
.4byte	for_exit,	only4test	//int 08, 09
.4byte	0,	0	//int 0a, 0b
.4byte	0,	0	//int
.4byte	0,	0	//int 0e, 0f
.4byte	0,	0	//int 10, 11
.4byte	0,	0	//int 12, 13
.4byte	HelloWorld,	0	//int 14, 15
.4byte	0,	0	//int 16, 17
.4byte	0,	0	//int 18, 19
.4byte	0,	0	//int 1a, 1b
.4byte	0,	0	//int 1c, 1d
.4byte	0,	0	//int 1e, 1f
byte .byte
twobyte .2byte
eightbyte .8byte
HelloWorld .asciiz "hello", ',', 'w' - 'a' + 0x60, "orld!"
ones .ascii 30 dup('1')
space .space 10
word .word 
dword .dword
half .half
.end __DEBUG
.origin 1024
.text
main:
	addi $a0, $zero, '7'
	addi $a1, $zero, CONST_DATA
	jal CONST_LABEL

	add CONST_REG, $v0, $zero
	addi $v0, $zero, 1 
	syscall
	addi $v0, $zero, 10
	syscall

mul_fun:#a0 for a, a1 for b, v0 for result
	push $t1, $0
	addi $sp, $sp, -8
	sw $t0, CONST_IMME($sp)#i
	sw $t1, 2($sp)#flag
	
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
__DEBUG:
for_exit:

	pop	$t1, $t0
	lw $t1, 2($sp)
	lw $t0, 0($sp)
	addi $sp, $sp, 4
	jr $ra
	
only4test:
	CONST_OP $t0, $t1, for_exit
	//CONST_OPz $t0, $t1, for_exit
	//la $Ra, $r0, for_exi