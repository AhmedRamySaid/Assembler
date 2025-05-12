addi r1, r0, -5  # Put -5 in r1
addi r2, r0, 15  # Put 15 in r2
add r2, r2, r2   # r2 *= 2
sw r2, r0, 0     # Store 15 at mem_0
lw r3, r0, 0     # Load 15 at r3
mvz r4, r3, r0   # Take content from r3 and put it in r4 if r0 is 0
jump 3