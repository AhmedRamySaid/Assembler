# Initialize registers with test values
addi r1, r0, 5       # r1 = 5 (r0 is always 0)
addi r2, r0, 3       # r2 = 3
addi r3, r0, 0       # r3 = 0
addi r4, r0, 255     # r4 = 255 (binary: 11111111)

# Test arithmetic operations
add r5, r1, r2       # r5 = r1 + r2 = 5 + 3 = 8
sub r6, r1, r2       # r6 = r1 - r2 = 5 - 3 = 2

# Test logical operations
and r7, r1, r4       # r7 = r1 AND r4 = 5 AND 255 = 5
                     # 5 (00000101) AND 255 (11111111) = 5 (00000101)

# Need to clear r7 for next operation (since we have limited registers)
addi r7, r0, 0       # r7 = 0
or r7, r1, r2        # r7 = r1 OR r2 = 5 OR 3 = 7
                     # 5 (00000101) OR 3 (00000011) = 7 (00000111)

# Test comparison
slt r5, r2, r1       # r5 = (r2 < r1) ? 1 : 0 = (3 < 5) ? 1 : 0 = 1
slt r6, r1, r2       # r6 = (r1 < r2) ? 1 : 0 = (5 < 3) ? 1 : 0 = 0

# Test conditional move
mvz r5, r1, r3       # r5 = (r3 == 0) ? r1 : r5 = (0 == 0) ? 5 : X = 5
mvz r6, r1, r2       # r6 = (r2 == 0) ? r1 : r6 = (3 == 0) ? 5 : X = unchanged (0)

# Test immediate operations
andi r7, r4, 15      # r7 = r4 AND 15 = 255 AND 15 = 15
                     # 255 (11111111) AND 15 (00001111) = 15 (00001111)
addi r5, r1, -2      # r5 = r1 + (-2) = 5 - 2 = 3

# Test load effective address (scaled addition)
lea r6, r1, 4        # r6 = r1 + (4 * r6) = 5 + (4 * 0) = 5 (since r6 was 0)
addi r6, r0, 2       # r6 = 2
lea r7, r1, 4        # r7 = r1 + (4 * r7) = 5 + (4 * 15) = 65

# Test branch operations
addi r6, r0, 5       # r6 = 5
beq r1, r6, 1        # if (r1 == r6) PC += 1 (will branch since both are 5) // error
addi r7, r0, 99      # This should be skipped
addi r7, r0, 42      # r7 = 42 (this is where we branch to)

# Test jump operation
jump 26
addi r7, r0, 64      # This should be skipped

# Test memory operations
addi r1, r0, 16
sw r1, r0, 1         # Mem[1] = r1 = 16
lw r2, r0, 1         # r2 = Mem[1] = 16

# Test pcm operation
addi r2, r0, 1       # r2 = 1
addi r3, r0, 4       # r3 = 4
pcm r2, r3, 1        # PC = Mem[r2] and Mem[r3 + i] = Mem[r2] + 1
                     # PC = 32, Mem[5] = 17