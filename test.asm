/******************************************
* MIPS系统程序。2013.12.26.Q.
* 弹指一挥双甲子，改天换地全仗君
*****************************************/
INTadr	equ	0+1 * 2 + ~5*(3+4-2)	#系统程序起始地址
KEYBOARD	equ	1000	#键盘I/O地址
IntEnable	equ	0x80000000	
STATUs	equ	12	#状态标志寄存器
CAUSE	equ	13	#中断原因寄存器
EPC	equ	14	#中断返回寄存器
.origin	0	
.end __End
j	InitOS	
IntVtTable	.4byte	0,	0+1 * 2 + ~5*(3+4-2)	//int 00, 01
.4byte	0,	0	//int 02, 03
.4byte	0,	0	//int 04, 05
.4byte	0,	0	//int 06, 07
.4byte	syscal,	intkb	//int 08, 09
.4byte	0,	0	//int 0a, 0b
.4byte	0,	0	//int
.4byte	0,	0	//int 0e, 0f
.4byte	0,	0	//int 10, 11
.4byte	0,	0	//int 12, 13
.4byte	0,	0	//int 14, 15
.4byte	0,	0	//int 16, 17
.4byte	0,	0	//int 18, 19
.4byte	0,	0	//int 1a, 1b
.4byte	0,	0	//int 1c, 1d
.4byte	0,	0	//int 1e, 1f

#.origin	1024
.data	

CRTadr	.4byte	6144	#当前屏幕方式，0=文本
CRTstart	.2byte	
WIDTH	.2byte	80	#屏幕字符列数
HEIGHT	.2byte	25	#屏幕字符行数
CurCOL	.2byte	20	#当前光标列x
CurROW	.2byte	10	#当前光标行y
SCRmod	.2byte	0	#当前屏幕方式，0=文本
hi	.2byte	"Hello'2013", 13, "Mips CPU", 13, "ZheJiang University",0	
INTnum	.2byte	16,0	#系统允许中断数、当前
kyptr	.2byte	kybuf,	kybuf	#键盘缓冲区首指针、尾指针
kybuf	.2byte	1,2,3,4,5,6,7,8	#16个字符32B键盘缓冲区
.text	

.origin	INTadr	
INTvect:	#中断总入口
push	$ra,$s0,$t0	 
la	$t0,INTnum	
lh	$t0,0($t0)	#取中断数
sltu	$t0,$s0,$t0	#比较
beq	$t0,$zero,EXIT	#非法中断
sll	$s0,$s0,2	
lw	$s0,IntVtTable($s0)	#转相应
jalr	$s0,$ra	
EXIT:	pop	$ra,$s0,$t0	

intkb:	#键盘中断
push	$t0,$s0,$s1	
la	$t0,kyptr	#缓冲区首址
lh	$s0,0($t0)	#首指针
lh	$s1,2($t0)	#尾指针
la	$t0,kybuf	#缓冲区首址
addi	$t0,$t0,32	#缓冲区末址
addi	$s0,$s0,2	
blt	$s0,$t0,Ra1	#超缓冲区末？
addi	$s0,$s0,-32	#等于折返
Ra1:	beq	$s0,$s1,Ra9	#缓冲区满
lb	$t0,KEYBOARD($zero)	
sh	$t0,0($s0)	#键盘写入首址
la	$t0,kyptr	#缓冲区首址
sh	$s0,0($t0)	#存缓冲区首址
Ra9:	pop	$t0,$s0,$s1	
jr	$ra	

READchar:	#syscall
push	$t0,$s0,$s1	
push	$t0	
sll	$t0,$t0,1	
srl	$t0,$t0,1	#0:允许中断
la	$t0,kyptr	#键盘指针
lh	$s1,2($t0)	#尾指针
Rb1:	lh	$s0,0($t0)	#首指针
beq	$s0,$s1,Rb1	#等键盘中断输入
la	$t0,kybuf	#缓冲区首址
addi	$t0,$t0,32	#缓冲区末址
addi	$s1,$s1,2	
blt	$s1,$t0,Rb2	#超缓冲区末？
addi	$s1,$s1,-32	#等于折返
Rb2:	lh	$a0,0($s1)	#返回: $a0
la	$t0,kyptr	#键盘指针
sh	$s1,2($t0)	#写尾指针
or	$s0,$s0,$t0	#1:禁止中断
pop	$t0	
pop	$t0,$s0,$s1	
jr	$ra	

READstring:	#syscall
push	$ra,$a0,$t0,$s0,$s1	
move	$s0,$a0	#缓冲区首地址
add	$s1,$s0,$a1	#缓冲区末地址
addi	$t0,$zero,13	#回车
Rm1:	jal	READchar	
sb	$a0,0($s0)	
addi	$s0,$s0,0+1 * 2 + ~5*(3+4-2)
beq	$a0,$t0,Rm9	#回车结束
bne	$s0,$s1,Rm1	#不满继续
Rm9:	pop	$ra,$a0,$t0,$s0,$s1	
jr	$ra	 

syscal:	#系统功能调用
jr	$ra	

dispCHxy:	#在(x,y)显示字符
push	$a1,$s0,$t0,$t1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
move	$t1,$zero	
Rc0:	andi	$t0,$a1,1	#移位加$s4(ROW)*$s1(WIDTH)
beqz	$t0,Rc1	
add	$t1,$t1,$s0	
Rc1:	sll	$s0,$s0,1	
srl	$a1,$a1,1	
bnez	$a,Rc0	
add	$t1,$t1,$a2	#+COL
la	$t0,CRTadr	
lw	$t0,0($t0)	
add	$t1,$t1,$t0	
sb	$a0,0($t1)	#写
pop	$a1,$s0,$t0,$t1	
jr	$ra	

PRINTchar:	#显示字符
push	$ra,$a0,$a1,$a2,$t0	
la	$t0,CurROW	#CurROW adr
lh	$a1,0($t0)	#CurROW
la	$t0,CurCOL	#CurCOL adr
lh	$a2,0($t0)	#CurCOL
li	$t0,13	#回车
beq	$a0,$t0,Rd4	
jal	dispCHxy	
la	$t0,WIDTH	#WIDTH adr
lh	$t0,0($t0)	#WIDTH
addi	$a2,$a2,1	#列+1
blt	$a2,$t0,Rd2	#如果<WIDTH
Rd4:	move	$a2,$zero	
la	$t0,HEIGHT	#HEIGHT adr
lh	$t0,0($t0)	#HEIGHT
addi	$a1,$a1,1	#行+1
blt	$a1,$t0,Rd3	#如果<HEIGHT
addi	$a1,$a1,-1	
jal	scrollUp	#向上滚屏一行
Rd3:	la	$t0,CurROW	#CurROW adr
sh	$a1,0($t0)	
Rd2:	la	$t0,CurCOL	#CurCOL adr
sh	$a2,0($t0)	
pop	$ra,$a0,$a1,$a2,$t0	
jr	$ra	

scrollUp:	#整屏上滚一行
push	$t0,$t1,$s0,$s1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
la	$t0,HEIGHT	#HEIGHT adr
lh	$s1,0($t0)	#HEIGHT
addi	$s1,$s1,-1	#HEIGHT--
move	$t1,$zero	
Ru1:	andi	$t0,$s0,1	#移位加$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,Ru2	
add	$t1,$t1,$s1	
Ru2:	sll	$s1,$s1,1	#HTIGHT<<1
srl	$s0,$s0,1	#WIDTH>>1
bne	$s0,$zero,Ru1	
la	$t0,CRTadr	#显存首址 adr
lw	$s0,0($t0)	#显存首址
add	$t1,$t1,$s0	#(N-1)行:CRTadr+WT*(HT-1)
la	$t0,WIDTH	#WIDTH adr
lh	$s1,0($t0)	#WIDTH
add	$s1,$s0,$s1	#CRTadr+WIDTH
Ru3:	lw	$t0,0($s1)	#读字
sw	$t0,0($s0)	#写字
addi	$s0,$s0,4	
addi	$s1,$s1,4	
bne	$s0,$t1,Ru3	
li	$t0,'.'	#填充字符
Ru4:	sb	$t0,0($s0)	#填充空白行
addi	$s0,$s0,1	
bne	$s0,$s1,Ru4	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

clr:	#清屏
push	$t0,$t1,$s0,$s1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
la	$t0,HEIGHT	#HEIGHT adr
lh	$s1,0($t0)	#HEIGHT
move	$t1,$zero	
Re0:	andi	$t0,$s0,1	#移位加$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,Re1	
add	$t1,$t1,$s1	
Re1:	sll	$s1,$s1,1	#HTIGHT<<1
srl	$s0,$s0,1	#WIDTH>>1
bne	$s0,$zero,Re0	
la	$t0,CRTadr	
lw	$s0,0($t0)	
add	$s1,$s0,$t1	#CRTadr+(WT*HT)
li	$t0,'^'	#填充字符
Re2:	sb	$t0,0($s0)	#填充空白行
addi	$s0,$s0,1	
bne	$s0,$s1,Re2	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

PRINTstring:	#显示字符串
push	$ra,$a0,$s0	
move	$s0,$a0	
Rn1:	lb	$a0,0($s0)	
beqz	$a0,Rn9	
jal	PRINTchar	
addi	$s0,$s0,1	
j	Rn1	
Rn9:	pop	$ra,$a0,$s0	
jr	$ra
InitOS:	#系统初始化
jal	clr	#清屏
HelloWorld:	#显示字符串
la	$a0,hi	
__End:
li	$v0,4	
syscall	
RRRR:	li	$v0,12	
syscall	
li	$v0,11	
syscall	
j	RRRR	
li	$v0,12	
syscall

addi $1,$0,1