#
# MIPSϵͳ����2013.12.26.Q.
# ��ָһ��˫���ӣ����컻��ȫ�̾�
#
#ϵͳ������ʼ��ַ
#����I/O��ַ

#״̬��־�Ĵ���
#�ж�ԭ��Ĵ���
#�жϷ��ؼĴ���
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

crtadr	.4byte	6144#��ǰ��Ļ��ʽ��0=�ı�
crtstart	.2byte	
width	.2byte	80#��Ļ�ַ�����
height	.2byte	25#��Ļ�ַ�����
curcol	.2byte	20#��ǰ�����x
currow	.2byte	10#��ǰ�����y
scrmod	.2byte	0#��ǰ��Ļ��ʽ��0=�ı�
hi	.byte	"hello'2013", 13, "mips cpu", 13, "zhejiang university",0	
intnum	.2byte	16,0#ϵͳ�����ж�������ǰ
kyptr	.2byte	kybuf,	kybuf#���̻�������ָ�롢βָ��
kybuf	.2byte	1,2,3,4,5,6,7,8#16���ַ�32B���̻�����
.text	

.origin	1024	
intvect:#�ж������
push	$ra,$s0,$t0	
mfc0	$s0,13#ȡ�жϺ�
la	$t0,intnum	
lh	$t0,0($t0)#ȡ�ж���
sltu	$t0,$s0,$t0#�Ƚ�
beq	$t0,$zero,exit#�Ƿ��ж�
sll	$s0,$s0,2	
lw	$s0,intvttable($s0)#ת��Ӧ
jalr	$s0,$ra	
exit:	pop	$ra,$s0,$t0	
eret	

intkb:#�����ж�
push	$t0,$s0,$s1	
la	$t0,kyptr#��������ַ
lh	$s0,0($t0)#��ָ��
lh	$s1,2($t0)#βָ��
la	$t0,kybuf#��������ַ
addi	$t0,$t0,32#������ĩַ
addi	$s0,$s0,2	
blt	$s0,$t0,ra1#��������ĩ��
addi	$s0,$s0,-32#�����۷�
ra1:	beq	$s0,$s1,ra9#��������
lb	$t0,1000($zero)	
sh	$t0,0($s0)#����д����ַ
la	$t0,kyptr#��������ַ
sh	$s0,0($t0)#�滺������ַ
ra9:	pop	$t0,$s0,$s1	
jr	$ra	

readchar:#syscall
push	$t0,$s0,$s1	
mfc0	$t0,14	
push	$t0	
mfc0	$t0,12	
sll	$t0,$t0,1	
srl	$t0,$t0,1#0:�����ж�
mtc0	$t0,12	
la	$t0,kyptr#����ָ��
lh	$s1,2($t0)#βָ��
rb1:	lh	$s0,0($t0)#��ָ��
beq	$s0,$s1,rb1#�ȼ����ж�����
la	$t0,kybuf#��������ַ
addi	$t0,$t0,32#������ĩַ
addi	$s1,$s1,2	
blt	$s1,$t0,rb2#��������ĩ��
addi	$s1,$s1,-32#�����۷�
rb2:	lh	$a0,0($s1)#����: $a0
la	$t0,kyptr#����ָ��
sh	$s1,2($t0)#дβָ��
mfc0	$s0,12	
lui	$t0,0x8000	
or	$s0,$s0,$t0#1:��ֹ�ж�
mtc0	$s0,12	
pop	$t0	
mtc0	$t0,14	
pop	$t0,$s0,$s1	
jr	$ra	

readstring:#syscall
push	$ra,$a0,$t0,$s0,$s1	
move	$s0,$a0#�������׵�ַ
add	$s1,$s0,$a1#������ĩ��ַ
addi	$t0,$zero,13#�س�
rm1:	jal	readchar	
sb	$a0,0($s0)	
addi	$s0,$s0,1	
beq	$a0,$t0,rm9#�س�����
bne	$s0,$s1,rm1#��������
rm9:	pop	$ra,$a0,$t0,$s0,$s1	
jr	$ra	 

syscal:#ϵͳ���ܵ���
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

dispchxy:#��(x,y)��ʾ�ַ�
push	$a1,$s0,$t0,$t1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
move	$t1,$zero	
rc0:	andi	$t0,$a1,1#��λ��$s4(ROW)*$s1(WIDTH)
beqz	$t0,rc1	
add	$t1,$t1,$s0	
rc1:	sll	$s0,$s0,1	
srl	$a1,$a1,1	
bnez	$a1,rc0	
add	$t1,$t1,$a2#+COL
la	$t0,crtadr	
lw	$t0,0($t0)	
add	$t1,$t1,$t0	
sb	$a0,0($t1)#д
pop	$a1,$s0,$t0,$t1	
jr	$ra	

printchar:#��ʾ�ַ�
push	$ra,$a0,$a1,$a2,$t0	
la	$t0,currow#CurROW adr
lh	$a1,0($t0)#CurROW
la	$t0,curcol#CurCOL adr
lh	$a2,0($t0)#CurCOL
li	$t0,13#�س�
beq	$a0,$t0,rd4	
jal	dispchxy	
la	$t0,width#WIDTH adr
lh	$t0,0($t0)#WIDTH
addi	$a2,$a2,1#��+1
blt	$a2,$t0,rd2#���<WIDTH
rd4:	move	$a2,$zero	
la	$t0,height#HEIGHT adr
lh	$t0,0($t0)#HEIGHT
addi	$a1,$a1,1#��+1
blt	$a1,$t0,rd3#���<HEIGHT
addi	$a1,$a1,-1	
jal	scrollup#���Ϲ���һ��
rd3:	la	$t0,currow#CurROW adr
sh	$a1,0($t0)	
rd2:	la	$t0,curcol#CurCOL adr
sh	$a2,0($t0)	
pop	$ra,$a0,$a1,$a2,$t0	
jr	$ra	

scrollup:#�����Ϲ�һ��
push	$t0,$t1,$s0,$s1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
la	$t0,height#HEIGHT adr
lh	$s1,0($t0)#HEIGHT
addi	$s1,$s1,-1#HEIGHT--
move	$t1,$zero	
ru1:	andi	$t0,$s0,1#��λ��$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,ru2	
add	$t1,$t1,$s1	
ru2:	sll	$s1,$s1,1#HTIGHT<<1
srl	$s0,$s0,1#WIDTH>>1
bne	$s0,$zero,ru1	
la	$t0,crtadr#�Դ���ַ adr
lw	$s0,0($t0)#�Դ���ַ
add	$t1,$t1,$s0#(N-1)��:CRTadr+WT*(HT-1)
la	$t0,width#WIDTH adr
lh	$s1,0($t0)#WIDTH
add	$s1,$s0,$s1#CRTadr+WIDTH
ru3:	lw	$t0,0($s1)#����
sw	$t0,0($s0)#д��
addi	$s0,$s0,4	
addi	$s1,$s1,4	
bne	$s0,$t1,ru3	
li	$t0,'.'#����ַ�
ru4:	sb	$t0,0($s0)#���հ���
addi	$s0,$s0,1	
bne	$s0,$s1,ru4	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

clr:#����
push	$t0,$t1,$s0,$s1	
la	$t0,width#WIDTH adr
lh	$s0,0($t0)#WIDTH
la	$t0,height#HEIGHT adr
lh	$s1,0($t0)#HEIGHT
move	$t1,$zero	
re0:	andi	$t0,$s0,1#��λ��$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,re1	
add	$t1,$t1,$s1	
re1:	sll	$s1,$s1,1#HTIGHT<<1
srl	$s0,$s0,1#WIDTH>>1
bne	$s0,$zero,re0	
la	$t0,crtadr	
lw	$s0,0($t0)	
add	$s1,$s0,$t1#CRTadr+(WT*HT)
li	$t0,'^'#����ַ�
re2:	sb	$t0,0($s0)#���հ���
addi	$s0,$s0,1	
bne	$s0,$s1,re2	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

printstring:#��ʾ�ַ���
push	$ra,$a0,$s0	
move	$s0,$a0	
rn1:	lb	$a0,0($s0)	
beqz	$a0,rn9	
jal	printchar	
addi	$s0,$s0,1	
j	rn1	
rn9:	pop	$ra,$a0,$s0	
jr	$ra

initos:#ϵͳ��ʼ��
jal	clr#����
helloworld:#��ʾ�ַ���
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
