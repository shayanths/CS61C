lui $r0, 0x00
lw $r0, 0($r0)

andi $r2, $r0, 7
or $r1, $r1, $r2
srlv $r2, $r2, $r2
addi $r2, $r2, 4
sllv $r1, $r1, $r2
addi $r2, $r2, -1
srlv $r0, $r0, $r2
andi $r2, $r0, 7
or $r1, $r1, $r2
srlv $r2, $r2, $r2
addi $r2, $r2, 4
sllv $r1, $r1, $r2
addi $r2, $r2, -1
srlv $r0, $r0, $r2
andi $r2, $r0, 7
or $r1, $r1, $r2

disp $r1, 0
jr $r3
