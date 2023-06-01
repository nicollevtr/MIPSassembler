lw $r2, 0($r0);       # Carrega o valor na mem 0 bit apos $r0 em $r2 
lw $r3, 1($r0);       # Carrega o valor na mem 1 bit apos $r0 em $r3
loop:
add $r4, $r2, $r3;    # $r4 = $r2 + $r3
sub $r7, $r8, $r5;    # $r4 = $r8 + $r5
beq $r4, $r3, label;
and $r2, $r2, $r3;    # $r2 = $r2 AND $r3
label:
sw $r2, 2($r0);       # Armazena $r2 na mem 2 bit apos $r0
jump loop;
