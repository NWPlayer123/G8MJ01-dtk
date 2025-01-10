/* 0000149C 000014D0  7C 08 02 A6 */	mflr r0
/* 000014A0 000014D4  90 01 00 04 */	stw r0, 0x4(r1)
/* 000014A4 000014D8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 000014A8 000014DC  BF 81 00 08 */	stmw r28, 0x8(r1)
/* 000014AC 000014E0  3C 60 00 00 */	lis r3, __data_0@ha
/* 000014B0 000014E4  3B C3 00 00 */	addi r30, r3, __data_0@l
/* 000014B4 000014E8  3B E0 00 00 */	li r31, 0x0
/* 000014B8 000014EC  80 00 00 00 */	lwz r0, IsWGPipeRedirected
/* 000014BC 000014F0  2C 00 00 00 */	cmpwi r0, 0x0
/* 000014C0 000014F4  40 82 00 18 */	bne L_000014D8
/* 000014C4 000014F8  38 7E 00 14 */	addi r3, r30, 0x14
/* 000014C8 000014FC  38 80 06 10 */	li r4, 0x610
/* 000014CC 00001500  38 BE 05 20 */	addi r5, r30, 0x520
/* 000014D0 00001504  4C C6 31 82 */	crclr 4*cr1+eq
/* 000014D4 00001508  48 00 00 01 */	bl OSPanic
L_000014D8:
/* 000014D8 0000150C  38 00 00 00 */	li r0, 0x0
/* 000014DC 00001510  90 00 00 00 */	stw r0, IsWGPipeRedirected
/* 000014E0 00001514  48 00 00 01 */	bl OSDisableInterrupts
/* 000014E4 00001518  7C 7C 1B 78 */	mr r28, r3
/* 000014E8 0000151C  3B A0 00 00 */	li r29, 0x0
/* 000014EC 00001520  48 00 00 14 */	b L_00001500
L_000014F0:
/* 000014F0 00001524  38 00 00 00 */	li r0, 0x0
/* 000014F4 00001528  3C 60 CC 01 */	lis r3, 0xcc01
/* 000014F8 0000152C  98 03 80 00 */	stb r0, -0x8000(r3)
/* 000014FC 00001530  3B BD 00 01 */	addi r29, r29, 0x1
L_00001500:
/* 00001500 00001534  28 1D 00 1F */	cmplwi r29, 0x1f
/* 00001504 00001538  41 80 FF EC */	blt L_000014F0
/* 00001508 0000153C  48 00 00 01 */	bl PPCSync
L_0000150C:
/* 0000150C 00001540  48 00 00 01 */	bl PPCMfwpar
/* 00001510 00001544  54 60 07 FF */	clrlwi. r0, r3, 31
/* 00001514 00001548  40 82 FF F8 */	bne L_0000150C
/* 00001518 0000154C  3C 60 CC 01 */	lis r3, 0xcc01
/* 0000151C 00001550  38 63 80 00 */	addi r3, r3, -0x8000
/* 00001520 00001554  48 00 00 01 */	bl OSUncachedToPhysical
/* 00001524 00001558  48 00 00 01 */	bl PPCMtwpar
/* 00001528 0000155C  80 60 00 00 */	lwz r3, CPUFifo
/* 0000152C 00001560  80 03 00 00 */	lwz r0, 0x0(r3)
/* 00001530 00001564  54 00 00 BE */	clrlwi r0, r0, 2
/* 00001534 00001568  80 60 00 00 */	lwz r3, __piReg
/* 00001538 0000156C  90 03 00 0C */	stw r0, 0xc(r3)
/* 0000153C 00001570  80 60 00 00 */	lwz r3, CPUFifo
/* 00001540 00001574  80 03 00 04 */	lwz r0, 0x4(r3)
/* 00001544 00001578  54 00 00 BE */	clrlwi r0, r0, 2
/* 00001548 0000157C  80 60 00 00 */	lwz r3, __piReg
/* 0000154C 00001580  90 03 00 10 */	stw r0, 0x10(r3)
/* 00001550 00001584  80 60 00 00 */	lwz r3, CPUFifo
/* 00001554 00001588  80 03 00 18 */	lwz r0, 0x18(r3)
/* 00001558 0000158C  54 03 00 BE */	clrlwi r3, r0, 2
/* 0000155C 00001590  54 60 D9 55 */	rlwinm. r0, r3, 27, 5, 10
/* 00001560 00001594  41 82 00 18 */	beq L_00001578
/* 00001564 00001598  38 7E 00 14 */	addi r3, r30, 0x14
/* 00001568 0000159C  38 80 06 2A */	li r4, 0x62a
/* 0000156C 000015A0  38 BE 03 98 */	addi r5, r30, 0x398
/* 00001570 000015A4  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001574 000015A8  48 00 00 01 */	bl OSPanic
L_00001578:
/* 00001578 000015AC  80 60 00 00 */	lwz r3, CPUFifo
/* 0000157C 000015B0  80 03 00 18 */	lwz r0, 0x18(r3)
/* 00001580 000015B4  50 1F 01 B4 */	rlwimi r31, r0, 0, 6, 26
/* 00001584 000015B8  38 00 00 00 */	li r0, 0x0
/* 00001588 000015BC  50 1F D1 4A */	rlwimi r31, r0, 26, 5, 5
/* 0000158C 000015C0  80 60 00 00 */	lwz r3, __piReg
/* 00001590 000015C4  93 E3 00 14 */	stw r31, 0x14(r3)
/* 00001594 000015C8  88 00 00 00 */	lbz r0, CPGPLinked
/* 00001598 000015CC  28 00 00 00 */	cmplwi r0, 0x0
/* 0000159C 000015D0  41 82 00 24 */	beq L_000015C0
/* 000015A0 000015D4  38 60 00 01 */	li r3, 0x1
/* 000015A4 000015D8  38 80 00 01 */	li r4, 0x1
/* 000015A8 000015DC  48 00 00 01 */	bl __GXWriteFifoIntReset
/* 000015AC 000015E0  38 60 00 01 */	li r3, 0x1
/* 000015B0 000015E4  38 80 00 00 */	li r4, 0x0
/* 000015B4 000015E8  48 00 00 01 */	bl __GXWriteFifoIntEnable
/* 000015B8 000015EC  38 60 00 01 */	li r3, 0x1
/* 000015BC 000015F0  48 00 00 01 */	bl __GXFifoLink
L_000015C0:
/* 000015C0 000015F4  48 00 00 01 */	bl PPCSync
/* 000015C4 000015F8  7F 83 E3 78 */	mr r3, r28
/* 000015C8 000015FC  48 00 00 01 */	bl OSRestoreInterrupts
/* 000015CC 00001600  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 000015D0 00001604  BB 81 00 08 */	lmw r28, 0x8(r1)
/* 000015D4 00001608  38 21 00 18 */	addi r1, r1, 0x18
/* 000015D8 0000160C  7C 08 03 A6 */	mtlr r0
/* 000015DC 00001610  4E 80 00 20 */	blr