
buildin_test.o:     file format elf32-tradlittlemips


Disassembly of section .text.startup:

00000000 <main>:
   0:	3c1c0000 	lui	gp,0x0
  1c:	279c0000 	jalr	t9
  20:	27bdffe0 	nop
  24:	8f990000 	move	v1,v0
  28:	8ca40004 	li	a0,2
  2c:	afbf001c 	bne	v1,a0,44 <main+0x2c>
  30:	afbc0010 	li	v0,3
  34:	0320f809 	lw	ra,28(sp)
  38:	00000000 	nop
  3c:	00401821 	jr	ra
  40:	24040002 	addiu	sp,sp,32
  44:	14640005 	j	34 <main+0x1c>
  48:	24020003 	addiu	v0,v1,-1
  4c:	Address 0x000000000000004c is out of bounds.

