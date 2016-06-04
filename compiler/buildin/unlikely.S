
buildin_test.o:     file format elf32-tradlittlemips


Disassembly of section .text.startup:

00000000 <main>:
   0:	3c1c0000 	lui	gp,0x0
  1c:	279c0000 	jalr	t9
  20:	27bdffe0 	nop
  24:	8f990000 	li	v1,2
  28:	8ca40004 	beq	v0,v1,44 <main+0x2c>
  2c:	afbf001c 	nop
  30:	afbc0010 	addiu	v0,v0,-1
  34:	0320f809 	lw	ra,28(sp)
  38:	00000000 	nop
  3c:	24030002 	jr	ra
  40:	10430006 	addiu	sp,sp,32
  44:	00000000 	j	34 <main+0x1c>
  48:	2442ffff 	li	v0,3
  4c:	Address 0x000000000000004c is out of bounds.

