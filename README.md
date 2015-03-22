# masm  
a mips assembler    
一个mips的汇编器  

##指令集：  rt,rs,rd:寄存器,imme:立即数,L:标记,shamt:左/右移次数
addi rt,rs,imme  
addiu rt,rs,imme  
andi rt,rs,imme  
beq rt,rs,L  
bgez rs,L  
bgtz rs,L  
blez rs,L  
bltz rs,L  
bne rs,L  
lh rt,imme(rs)  
lw rt,imme(rs)  
ori rt,rs,imme  
sh rt,imme(rs)  
slti rt,rs,imme  
sw rt,imme(rs)  
xori rt,rs,imme  
j L  
jal L  
add rd,rs,rt  
addu rd,rs,rt  
and rd,rs,rt  
div rs,rt  
jalr rs,rd  
jr rs  
mfhi rd  
mflo rd  
mthi rs  
mtlo rs  
mult rs,rt  
or rd,rs,rt  
sll rd,rs,shamt  
sllv rd,rt,rs  
slt rd,rs,rt  
sltu rd,rs,rt  
sra rd,rs,shamt  
srav rd,rs,rt  
srl rd,rs,shamt  
srlv rd,rs,rt  
sub rd,rs,rt  
subu rd,rs,rt  
xor rd,rs,rt  
lui rs,imme  

##伪指令：  
abs rt,rs  
beqi rt, imme  
bnei rt, imme  
divi imme  
muli rt, imme  
move rt,rs  
clear rt  
not rt,rs  
la rd,L  
li rd,imme  
b L  
bal L  
bgt rs,rt,L  
blt rs,rt,L  
bge rs,rt,L  
ble rs,rt,L  
bgtu rs,rt,L  
bgtz rs,L  
beqz rs,L  
rem rd,rs,rt  
push rs1,rs2,rs3…..  
pop rs1,rs2,rs3…...  
lb和sb在写汇编代码的时候也可以使用，但因为整个结构为16位的，所以会直接被预处理成lh和sh指令  

##格式指令;格式;说明  
equ;符号 equ <表达式>;
.align;.align n;地址按2的n幂对齐  
.origin;.origin addr;
.data;;
.text;;
.end;.end L;从当前行到L行都被直接跳过而不被汇编器翻译（与注释相同，但是中间行会被语法检查）  
dup;n dup(<表达式>)或(‘字符’);重复括号内容n次  

.byte;变量 .byte<表>;会被直接当作.2byte使用
.2byte;;  
.4byte;;  
.8byte;;  
.word;;  
.dword;;   
.half;;  
.ascii;;  
.asciiz;以0结束的ascii字符串变量;  
.space;.space n;n字节空间  

##注释		//, #, /* */					支持MIPS的单行注释风格，以及C的单行和多行注释风格  
  	  
##表达式：  
+	加  
-	减  
*	乘  
/	除  
%	取余  
~	非  
&	与  
|	或  
^	异或  
<<	左移  
>>	右移  
>>>	算术右移  
(	左括号  
)	右括号  
  
以上表达式都可以计算，但是只有在数据段以及可能出现数字的指令中，其他位置出现将会报错  
  
##错误信息：  
主要分为两个阶段：  
###第一个阶段  
是语法检查，类似于gcc的输出.obj的目标文件的过程，检查是否符合以上语法规则，如果不符合则将所有错误信息输出，停止进行下一个阶段的运行与检查；  
###第二个阶段  
是编译阶段，类似于gcc的link，将所有的label全部翻译为对应的地址，如果不存在则报错。此部分基本上由以上语法规定，错误信息都是根据以上语法给出。比如，如果dup前面没有数字，则会报错不符合dup的使用规范等。  

##例程  
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
.4byte	mul_fun,	for	//int 08, 09  
.4byte	0,	0	//int 0a, 0b  
.4byte	0,	0	//int  
.4byte	0,	0	//int 0e, 0f  
.4byte	0,	0	//int 10, 11  
.4byte	0,	0	//int 12, 13  
.4byte	HelloWord,	0	//int 14, 15  
.4byte	0,	0	//int 16, 17  
.4byte	0,	0	//int 18, 19  
.4byte	0,	0	//int 1a, 1b  
.4byte	0,	0	//int 1c, 1d  
.4byte	0,	0	//int 1e, 1f  
byte .byte  
twobyte .2byte  
eightbyte .8byte  
HelloWord .asciiz "hello", ',', 'w' - 'a' + 0x50, "orld!"  
ones .ascii 30 dup('1')  
space .space 10  
.origin 1024  
.text  
main:  
//.end __DEBUG  
	addi $a0, $zero, '77'  
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
for_exit:  
__DEBUG:  
	pop	$t1, $t0  
	lw $t1, 2($sp)  
	lw $t0, 0($sp)  
	addi $sp, $sp, 4  
	jr $ra  
	  
only4test:  
	CONST_OP $t0, $t1, for_exit  
	CONST_OP $t0, $t1, for_exit  
	la $Ra, $r0, for_exit  
