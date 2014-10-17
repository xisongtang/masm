#
# MIPS系统程序。2013.12.26.Q.
# 弹指一挥双甲子，改天换地全仗君
#
#系统程序起始地址
#键盘I/O地址

#状态标志寄存器
#中断原因寄存器
#中断返回寄存器
.origin	0	
j	initos	
intvttable	.4byte	int00s,	int01s#int 00, 01
.4byte	int02s,	int03s#int 02, 03
.4byte	int04s,	int05s#int 04, 05
.4byte	int06s,	int07s#int 06, 07
.4byte	syscal,	intkb#int 08, 09
.4byte	int0as,	int0bs#int 0a, 0b
.4byte	int0cs,	int0ds#int
.4byte	int0es,	int0fs#int 0e, 0f
.4byte	int10s,	int11s#int 10, 11
.4byte	int12s,	int13s#int 12, 13
.4byte	int14s,	int15s#int 14, 15
.4byte	int16s,	int17s#int 16, 17
.4byte	int18s,	int19s#int 18, 19
.4byte	int1as,	int1bs#int 1a, 1b
.4byte	int1cs,	int1ds#int 1c, 1d
.4byte	int1es,	int1fs#int 1e, 1f
#.origin	1024
.data	

crtadr	.4byte	6144#当前屏幕方式，0=文本
crtstart	.2byte	
width	.2byte	80#屏幕字符列数
height	.2byte	25#屏幕字符行数
curcol	.2byte	20#当前光标列x
currow	.2byte	10#当前光标行y
scrmod	.2byte	0#当前屏幕方式，0=文本
hi	.byte	"hello'2013", 13, "mips cpu", 13, "zhejiang university",0	
intnum	.2byte	16,0#系统允许中断数、当前
kyptr	.2byte	kybuf,	kybuf#键盘缓冲区首指针、尾指针
kybuf	.2byte	1,2,3,4,5,6,7,8#16个字符32B键盘缓冲区
.text	

.origin	1024	
intvect:#中断总入口
push	$ra,$s0,$t0	
mfc0	$s0,13#取中断号
la	$t0,intnum	
lh	$t0,0($t0)#取中断数
sltu	$t0,$s0,$t0#比较
beq	$t0,$zero,exit#非法中断
sll	$s0,$s0,2	
lw	$s0,intvttable($s0)#转相应
jalr	$s0,$ra	
exit:	pop	$ra,$s0,$t0	
eret	

intkb:#键盘中断
push	$t0,$s0,$s1	
la	$t0,kyptr#缓冲区首址
lh	$s0,0($t0)#首指针
lh	$s1,2($t0)#尾指针
la	$t0,kybuf#缓冲区首址
addi	$t0,$t0,32#缓冲区末址
addi	$s0,$s0,2	
blt	$s0,$t0,ra1#超缓冲区末？
addi	$s0,$s0,-32#等于折返
ra1:	beq	$s0,$s1,ra9#缓冲区满
lb	$t0,1000($zero)	
sh	$t0,0($s0)#键盘写入首址
la	$t0,kyptr#缓冲区首址
sh	$s0,0($t0)#存缓冲区首址
ra9:	pop	$t0,$s0,$s1	
jr	$ra	

readchar:#syscall
push	$t0,$s0,$s1	
mfc0	$t0,14	
push	$t0	
mfc0	$t0,12	
sll	$t0,$t0,1	
srl	$t0,$t0,1#0:允许中断
mtc0	$t0,12	
la	$t0,kyptr#键盘指针
lh	$s1,2($t0)#尾指针
rb1:	lh	$s0,0($t0)#首指针
beq	$s0,$s1,rb1#等键盘中断输入
la	$t0,kybuf#缓冲区首址
addi	$t0,$t0,32#缓冲区末址
addi	$s1,$s1,2	
blt	$s1,$t0,rb2#超缓冲区末？
addi	$s1,$s1,-32#等于折返
rb2:	lh	$a0,0($s1)#返回: $a0
la	$t0,kyptr#键盘指针
sh	$s1,2($t0)#写尾指针
mfc0	$s0,12	
lui	$t0,0x8000	
or	$s0,$s0,$t0#1:禁止中断
mtc0	$s0,12	
pop	$t0	
mtc0	$t0,14	
pop	$t0,$s0,$s1	
jr	$ra	

readstring:#syscall
push	$ra,$a0,$t0,$s0,$s1	
move	$s0,$a0#缓冲区首地址
add	$s1,$s0,$a1#缓冲区末地址
addi	$t0,$zero,13#回车
rm1:	jal	readchar	
sb	$a0,0($s0)	
addi	$s0,$s0,1	
beq	$a0,$t0,rm9#回车结束
bne	$s0,$s1,rm1#不满继续
rm9:	pop	$ra,$a0,$t0,$s0,$s1	
jr	$ra	 

syscal:#系统功能调用
addi	$v0,$v0,-1	
beqz	$v0,printint#1
addi	$v0,$v0,-1	
beqz	$v0,printfloat#2
addi	$v0,$v0,-1	
beqz	$v0,printdouble#3
addi	$v0,$v0,-1	
beqz	$v0,printstring#4
addi	$v0,$v0,-1	
beqz	$v0,readint#5
addi	$v0,$v0,-1	
beqz	$v0,readfloat#6
addi	$v0,$v0,-1	
beqz	$v0,readdouble#7
addi	$v0,$v0,-1	
beqz	$v0,readstring#8
addi	$v0,$v0,-1	
beqz	$v0,sysbreak#9
addi	$v0,$v0,-1	
beqz	$v0,sysexit#10
addi	$v0,$v0,-1	
beqz	$v0,printchar#11
addi	$v0,$v0,-1	
beqz	$v0,readchar#12
addi	$v0,$v0,-1	
beqz	$v0,fileopen#13
addi	$v0,$v0,-1	
beqz	$v0,fileread#14
addi	$v0,$v0,-1	
beqz	$v0,filewrite#15
addi	$v0,$v0,-1	
beqz	$v0,fileclose#16
addi	$v0,$v0,-1	
beqz	$v0,sysexit2#17
jr	$ra	

dispchxy:#在(x,y)显示字符
push	$a1,$s0,$t0,$t1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
move	$t1,$zero	
rc0:	andi	$t0,$a1,1#移位加$s4(ROW)*$s1(WIDTH)
beqz	$t0,rc1	
add	$t1,$t1,$s0	
rc1:	sll	$s0,$s0,1	
srl	$a1,$a1,1	
bnez	$a1,rc0	
add	$t1,$t1,$a2#+COL
la	$t0,crtadr	
lw	$t0,0($t0)	
add	$t1,$t1,$t0	
sb	$a0,0($t1)#写
pop	$a1,$s0,$t0,$t1	
jr	$ra	

printchar:#显示字符
push	$ra,$a0,$a1,$a2,$t0	
la	$t0,currow#CurROW adr
lh	$a1,0($t0)#CurROW
la	$t0,curcol#CurCOL adr
lh	$a2,0($t0)#CurCOL
li	$t0,13#回车
beq	$a0,$t0,rd4	
jal	dispchxy	
la	$t0,width#WIDTH adr
lh	$t0,0($t0)#WIDTH
addi	$a2,$a2,1#列+1
blt	$a2,$t0,rd2#如果<WIDTH
rd4:	move	$a2,$zero	
la	$t0,height#HEIGHT adr
lh	$t0,0($t0)#HEIGHT
addi	$a1,$a1,1#行+1
blt	$a1,$t0,rd3#如果<HEIGHT
addi	$a1,$a1,-1	
jal	scrollup#向上滚屏一行
rd3:	la	$t0,currow#CurROW adr
sh	$a1,0($t0)	
rd2:	la	$t0,curcol#CurCOL adr
sh	$a2,0($t0)	
pop	$ra,$a0,$a1,$a2,$t0	
jr	$ra	

scrollup:#整屏上滚一行
push	$t0,$t1,$s0,$s1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
la	$t0,height#HEIGHT adr
lh	$s1,0($t0)#HEIGHT
addi	$s1,$s1,-1#HEIGHT--
move	$t1,$zero	
ru1:	andi	$t0,$s0,1#移位加$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,ru2	
add	$t1,$t1,$s1	
ru2:	sll	$s1,$s1,1#HTIGHT<<1
srl	$s0,$s0,1#WIDTH>>1
bne	$s0,$zero,ru1	
la	$t0,crtadr#显存首址 adr
lw	$s0,0($t0)#显存首址
add	$t1,$t1,$s0#(N-1)行:CRTadr+WT*(HT-1)
la	$t0,width#WIDTH adr
lh	$s1,0($t0)#WIDTH
add	$s1,$s0,$s1#CRTadr+WIDTH
ru3:	lw	$t0,0($s1)#读字
sw	$t0,0($s0)#写字
addi	$s0,$s0,4	
addi	$s1,$s1,4	
bne	$s0,$t1,ru3	
li	$t0,'.'#填充字符
ru4:	sb	$t0,0($s0)#填充空白行
addi	$s0,$s0,1	
bne	$s0,$s1,ru4	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

clr:#清屏
push	$t0,$t1,$s0,$s1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
la	$t0,height#HEIGHT adr
lh	$s1,0($t0)#HEIGHT
move	$t1,$zero	
re0:	andi	$t0,$s0,1#移位加$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,re1	
add	$t1,$t1,$s1	
re1:	sll	$s1,$s1,1#HTIGHT<<1
srl	$s0,$s0,1#WIDTH>>1
bne	$s0,$zero,re0	
la	$t0,crtadr	
lw	$s0,0($t0)	
add	$s1,$s0,$t1#CRTadr+(WT*HT)
li	$t0,'^'#填充字符
re2:	sb	$t0,0($s0)#填充空白行
addi	$s0,$s0,1	
bne	$s0,$s1,re2	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

printstring:#显示字符串
push	$ra,$a0,$s0	
move	$s0,$a0	
rn1:	lb	$a0,0($s0)	
beqz	$a0,rn9	
jal	printchar	
addi	$s0,$s0,1	
j	rn1	
rn9:	pop	$ra,$a0,$s0	
jr	$ra

initos:#系统初始化
jal	clr#清屏
helloworld:#显示字符串
la	$a0,hi	
li	$v0,4	
syscall	
rrrr:	li	$v0,12	
syscall	
li	$v0,11	
syscall	
j	rrrr	
li	$v0,12	
syscall
