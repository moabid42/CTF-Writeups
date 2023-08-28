#!/usr/bin/env python3
from z3 import *

s = Solver()

buffer = [BitVec(f'{i}', 8) for i in range(13)]

new_buffer = [BitVec(f'new{i}', 8) for i in range(13)]
for i in range(12):
    negated_i = ~BitVecVal(i, 8)
    new_buffer[i] = buffer[i] + buffer[i + 1] * negated_i

for i in range(12):
    s.add(And(buffer[i] >= 0x20, buffer[i] <= 0x7e))

s.add(Concat(new_buffer[7], new_buffer[6], new_buffer[5], new_buffer[4], new_buffer[3], new_buffer[2], new_buffer[1], new_buffer[0]) == 0x788C88B91D88AF0E)
s.add(Concat(new_buffer[11], new_buffer[10], new_buffer[9], new_buffer[8]) == 0x7DF311EC)
s.add(new_buffer[12] == 0)

second_part = ""
if s.check() == sat:
    m = s.model()
    for i in range(13):
        second_part += chr(int(str(m[buffer[i]])))
    print(second_part)