lui $r1, 0x00
lw $r0, 0($r1)
zb $r2, $r0
lui $r0, 0x01

loop:
beq $r2, $r0, break
addi $r1, $r1, 1
lw $r0, 0($r1)
zb $r2, $r0
lui $r0, 0x01

break:
lw $r0, 0($r1)
add $r1, $r1, $r1
ffo $r2, $r0
lui $r0, 0x00
bne $r2, $r0, end
addi $r1, $r1, 1

end:
disp $r1, 0
jr $r3
