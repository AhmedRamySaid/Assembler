| Operation |                    Description                    |        Example        |
|-----------|---------------------------------------------------|-----------------------|
| `add`     | Adds two registers                                | `add $rd, $rs, $rt`   |
| `sub`     | Subtracts two registers                           | `sub $rd, $rs, $rt`   |
| `and`     | Logical AND between two registers                 | `and $rd, $rs, $rt`   |
| `or`      | Logical OR between two registers                  | `or $rd, $rs, $rt`    |
| `slt`     | Set rd to 0 if rs < rt                            | `slt $rd, $rs, $rt`   |
| `mvz`     | Moves value in rs to  rd if rt == 0               | `mvz $rd, $rs, $rt`   |
| `addi`    | Adds a register and a constant                    | `addi $rt, $rs, i`    |
| `andi`    | Logical AND between a register and a constant     | `addi $rt, $rs, i`    |
| `lea`     | Multiplies I by rt and adds the result to rs      | `lea $rt, $rs, i`     |
| `beq`     | Adds I to the PC if rs == rt                      | `beq $rt, $rs, i`     |
| `pcm`     | Sets PC to Mem[rt] and Mem[rs + I] to Mem[rt] + 1 | `beq $rt, $rs, i`     |
| `lw`      | Load word from memory                             | `lw $rt, $rs, i`      |
| `sw`      | Store word to memory                              | `sw $rt, $rs, i`      |
