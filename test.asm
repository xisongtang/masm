/******************************************
* MIPSϵͳ����2013.12.26.Q.
* ��ָһ��˫���ӣ����컻��ȫ�̾�
*****************************************/
INTadr	equ	0+1 * 2 + ~5*(3+4-2)	#ϵͳ������ʼ��ַ
KEYBOARD	equ	1000	#����I/O��ַ
IntEnable	equ	0x80000000	
STATUs	equ	12	#״̬��־�Ĵ���
CAUSE	equ	13	#�ж�ԭ��Ĵ���
EPC	equ	14	#�жϷ��ؼĴ���
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

CRTadr	.4byte	6144	#��ǰ��Ļ��ʽ��0=�ı�
CRTstart	.2byte	
WIDTH	.2byte	80	#��Ļ�ַ�����
HEIGHT	.2byte	25	#��Ļ�ַ�����
CurCOL	.2byte	20	#��ǰ�����x
CurROW	.2byte	10	#��ǰ�����y
SCRmod	.2byte	0	#��ǰ��Ļ��ʽ��0=�ı�
hi	.2byte	"Hello'2013", 13, "Mips CPU", 13, "ZheJiang University",0	
INTnum	.2byte	16,0	#ϵͳ�����ж�������ǰ
kyptr	.2byte	kybuf,	kybuf	#���̻�������ָ�롢βָ��
kybuf	.2byte	1,2,3,4,5,6,7,8	#16���ַ�32B���̻�����
.text	

.origin	INTadr	
INTvect:	#�ж������
push	$ra,$s0,$t0	 
la	$t0,INTnum	
lh	$t0,0($t0)	#ȡ�ж���
sltu	$t0,$s0,$t0	#�Ƚ�
beq	$t0,$zero,EXIT	#�Ƿ��ж�
sll	$s0,$s0,2	
lw	$s0,IntVtTable($s0)	#ת��Ӧ
jalr	$s0,$ra	
EXIT:	pop	$ra,$s0,$t0	

intkb:	#�����ж�
push	$t0,$s0,$s1	
la	$t0,kyptr	#��������ַ
lh	$s0,0($t0)	#��ָ��
lh	$s1,2($t0)	#βָ��
la	$t0,kybuf	#��������ַ
addi	$t0,$t0,32	#������ĩַ
addi	$s0,$s0,2	
blt	$s0,$t0,Ra1	#��������ĩ��
addi	$s0,$s0,-32	#�����۷�
Ra1:	beq	$s0,$s1,Ra9	#��������
lb	$t0,KEYBOARD($zero)	
sh	$t0,0($s0)	#����д����ַ
la	$t0,kyptr	#��������ַ
sh	$s0,0($t0)	#�滺������ַ
Ra9:	pop	$t0,$s0,$s1	
jr	$ra	

READchar:	#syscall
push	$t0,$s0,$s1	
push	$t0	
sll	$t0,$t0,1	
srl	$t0,$t0,1	#0:�����ж�
la	$t0,kyptr	#����ָ��
lh	$s1,2($t0)	#βָ��
Rb1:	lh	$s0,0($t0)	#��ָ��
beq	$s0,$s1,Rb1	#�ȼ����ж�����
la	$t0,kybuf	#��������ַ
addi	$t0,$t0,32	#������ĩַ
addi	$s1,$s1,2	
blt	$s1,$t0,Rb2	#��������ĩ��
addi	$s1,$s1,-32	#�����۷�
Rb2:	lh	$a0,0($s1)	#����: $a0
la	$t0,kyptr	#����ָ��
sh	$s1,2($t0)	#дβָ��
or	$s0,$s0,$t0	#1:��ֹ�ж�
pop	$t0	
pop	$t0,$s0,$s1	
jr	$ra	

READstring:	#syscall
push	$ra,$a0,$t0,$s0,$s1	
move	$s0,$a0	#�������׵�ַ
add	$s1,$s0,$a1	#������ĩ��ַ
addi	$t0,$zero,13	#�س�
Rm1:	jal	READchar	
sb	$a0,0($s0)	
addi	$s0,$s0,0+1 * 2 + ~5*(3+4-2)
beq	$a0,$t0,Rm9	#�س�����
bne	$s0,$s1,Rm1	#��������
Rm9:	pop	$ra,$a0,$t0,$s0,$s1	
jr	$ra	 

syscal:	#ϵͳ���ܵ���
jr	$ra	

dispCHxy:	#��(x,y)��ʾ�ַ�
push	$a1,$s0,$t0,$t1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
move	$t1,$zero	
Rc0:	andi	$t0,$a1,1	#��λ��$s4(ROW)*$s1(WIDTH)
beqz	$t0,Rc1	
add	$t1,$t1,$s0	
Rc1:	sll	$s0,$s0,1	
srl	$a1,$a1,1	
bnez	$a,Rc0	
add	$t1,$t1,$a2	#+COL
la	$t0,CRTadr	
lw	$t0,0($t0)	
add	$t1,$t1,$t0	
sb	$a0,0($t1)	#д
pop	$a1,$s0,$t0,$t1	
jr	$ra	

PRINTchar:	#��ʾ�ַ�
push	$ra,$a0,$a1,$a2,$t0	
la	$t0,CurROW	#CurROW adr
lh	$a1,0($t0)	#CurROW
la	$t0,CurCOL	#CurCOL adr
lh	$a2,0($t0)	#CurCOL
li	$t0,13	#�س�
beq	$a0,$t0,Rd4	
jal	dispCHxy	
la	$t0,WIDTH	#WIDTH adr
lh	$t0,0($t0)	#WIDTH
addi	$a2,$a2,1	#��+1
blt	$a2,$t0,Rd2	#���<WIDTH
Rd4:	move	$a2,$zero	
la	$t0,HEIGHT	#HEIGHT adr
lh	$t0,0($t0)	#HEIGHT
addi	$a1,$a1,1	#��+1
blt	$a1,$t0,Rd3	#���<HEIGHT
addi	$a1,$a1,-1	
jal	scrollUp	#���Ϲ���һ��
Rd3:	la	$t0,CurROW	#CurROW adr
sh	$a1,0($t0)	
Rd2:	la	$t0,CurCOL	#CurCOL adr
sh	$a2,0($t0)	
pop	$ra,$a0,$a1,$a2,$t0	
jr	$ra	

scrollUp:	#�����Ϲ�һ��
push	$t0,$t1,$s0,$s1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
la	$t0,HEIGHT	#HEIGHT adr
lh	$s1,0($t0)	#HEIGHT
addi	$s1,$s1,-1	#HEIGHT--
move	$t1,$zero	
Ru1:	andi	$t0,$s0,1	#��λ��$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,Ru2	
add	$t1,$t1,$s1	
Ru2:	sll	$s1,$s1,1	#HTIGHT<<1
srl	$s0,$s0,1	#WIDTH>>1
bne	$s0,$zero,Ru1	
la	$t0,CRTadr	#�Դ���ַ adr
lw	$s0,0($t0)	#�Դ���ַ
add	$t1,$t1,$s0	#(N-1)��:CRTadr+WT*(HT-1)
la	$t0,WIDTH	#WIDTH adr
lh	$s1,0($t0)	#WIDTH
add	$s1,$s0,$s1	#CRTadr+WIDTH
Ru3:	lw	$t0,0($s1)	#����
sw	$t0,0($s0)	#д��
addi	$s0,$s0,4	
addi	$s1,$s1,4	
bne	$s0,$t1,Ru3	
li	$t0,'.'	#����ַ�
Ru4:	sb	$t0,0($s0)	#���հ���
addi	$s0,$s0,1	
bne	$s0,$s1,Ru4	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

clr:	#����
push	$t0,$t1,$s0,$s1	
la	$t0,WIDTH	#WIDTH adr
lh	$s0,0($t0)	#WIDTH
la	$t0,HEIGHT	#HEIGHT adr
lh	$s1,0($t0)	#HEIGHT
move	$t1,$zero	
Re0:	andi	$t0,$s0,1	#��λ��$s1(WIDTH)*$s2(HEIGHT)
beqz	$t0,Re1	
add	$t1,$t1,$s1	
Re1:	sll	$s1,$s1,1	#HTIGHT<<1
srl	$s0,$s0,1	#WIDTH>>1
bne	$s0,$zero,Re0	
la	$t0,CRTadr	
lw	$s0,0($t0)	
add	$s1,$s0,$t1	#CRTadr+(WT*HT)
li	$t0,'^'	#����ַ�
Re2:	sb	$t0,0($s0)	#���հ���
addi	$s0,$s0,1	
bne	$s0,$s1,Re2	
pop	$t0,$t1,$s0,$s1	
jr	$ra	

PRINTstring:	#��ʾ�ַ���
push	$ra,$a0,$s0	
move	$s0,$a0	
Rn1:	lb	$a0,0($s0)	
beqz	$a0,Rn9	
jal	PRINTchar	
addi	$s0,$s0,1	
j	Rn1	
Rn9:	pop	$ra,$a0,$s0	
jr	$ra
InitOS:	#ϵͳ��ʼ��
jal	clr	#����
HelloWorld:	#��ʾ�ַ���
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