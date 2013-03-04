lui $r0, 0x33          

ori $r0, $r0, 0x44

lui $r1, 0x33

ori $r1, $r1, 0x44

self: beq $r0, $r1, self 