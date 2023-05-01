# EP_815

## Description
In this challenge we were provided with :
`program.vsp`
`how_to_vesp-2.pdf`
Another vsp challenge but with another layer of difficulty, or maybe not


## Solution
The solution of this challenge starts exactly like the previous one, I ran the emulator. Again Following the instructions, I got the `output.txt`. Now just greping the memory, didn't get us anywhere we could see a lot of missing pieces in the flag.

```bash
$ cat output.txt| grep Memory
Memory[0000] = 0000
Memory[0001] = 0000
Memory[0010] = 0000
Memory[0000] = 0000
Memory[0001] = 0001
Memory[0010] = 0000
Memory[0001] = 0001
Memory[0001] = 0001
Memory[0001] = 0001
Memory[0001] = 0001
Memory[0010] = 000A
Memory[0000] = 0001
Memory[0001] = 000A
```

So my second thought was, let analyze the instruction a bit more.
The instuctions used for this were :
```bash
/* ADD: Add         Opcode: 0000  ----- A = A+B                          HexCode: 0
/* CMP: Compl       Opcode: 0001  ----- A = ~A                           HexCode: 1    
/* LDA: Load        Opcode: 0010  ----- M[IR[3:15] ] = M[MAR+1]          HexCode: 2
/* MOV: Move        Opcode: 0011  ----- M[IR[3:15] ] = M[M[MAR+1][3:15]] HexCode: 3  
/* JMP  Jump        Opcode: 0100  ----- PC = IR[3:15]                    HexCode: 4  
/* JEZ: Jump if 0   Opcode: 0101  ----- If (A = 0)  PC = IR[3:15]        HexCode: 5   
/* JPS: Jump if +   Opcode: 0110  ----- If (A > 0)  PC = IR[3:15]        HexCode: 6  
/* HLT: Hlt         Opcode: 0111  ----- reset = 1                        HexCode: 7 
```
(From the main.cpp file)

From here I found the ADD and the LDA operation, which adds and loads smth into the memory. Based on that I went to check the trace back of the CPU cycles and I saw this :

```bash
Machine Cycle 0007: PC = 000E, 
FETCH SUBCYCLE
MAR = 000E, IR = 0554, 
Clock cycle = 14
DECODE SUBCYCLE
Decoded instruction is: ADD
Clock cycle = 14
EXECUTE SUBCYCLE
Clock cycle = 15

*************Begin[Machine Level]*****************

A = 0001, B = 0001, Z = 0, S = 0, C = 0, F = 0
MAR = 000E, PC = 000F, IR = 0554, reset = 0
add = 1 complement = 0

Machine Cycle 0008: PC = 000F, 
FETCH SUBCYCLE
MAR = 000F, IR = 1000, 
Clock cycle = 17
DECODE SUBCYCLE
Decoded instruction is: CMP
Clock cycle = 17
EXECUTE SUBCYCLE
Clock cycle = 18

*************Begin[Machine Level]*****************

A = FFFE, B = 0001, Z = 0, S = 1, C = 0, F = 0
MAR = 000F, PC = 0010, IR = 1000, reset = 0
add = 0 complement = 1

Machine Cycle 0009: PC = 0010, 
FETCH SUBCYCLE
MAR = 0010, IR = 0000, 
Clock cycle = 1A
DECODE SUBCYCLE
Decoded instruction is: ADD
Clock cycle = 1A
EXECUTE SUBCYCLE
Clock cycle = 1B

*************Begin[Machine Level]*****************

A = FFFF, B = 0001, Z = 0, S = 1, C = 0, F = 0
MAR = 0010, PC = 0011, IR = 0000, reset = 0
add = 1 complement = 0

Machine Cycle 000A: PC = 0011, 
FETCH SUBCYCLE
MAR = 0011, IR = 2001, 
Clock cycle = 1D
DECODE SUBCYCLE
Decoded instruction is: LDA
Clock cycle = 1D
EXECUTE SUBCYCLE
Clock cycle = 1E

*************Begin[Machine Level]*****************

A = FFFF, B = 0001, Z = 0, S = 1, C = 0, F = 0
MAR = 0011, PC = 0013, IR = 2001, reset = 0
add = 0 complement = 0

Memory[0001] = 0001

*************End[Machine Level]*****************
```
As you can see from here, we can see that we are loading `0554` into the IR register, where `0` means the ADD instuction and the `554` the value added, and from here we can see that `55` is `U` in hex.
Well following the ADD instruction like this, we could get like half of the flag :
`U..CTF{smuggl3d_n`

And now the intresting part, after readin the memory over and over we can see this :
```bash
Memory[0001] = 000A
Memory[0010] = 000A
```
Yes we are loading the content of the register A into the memory.
And now we need to find a way to print the content of the A register. Sadly the emulator doesn't provide this, so I simply  added it ! Yes I modified the main.cpp files to dump the content of the hole memory which by default gonna print us everything (As we know the memory is linear)
So Adding :
```cpp
      std::cout << "\nThe memory after execution in hex:\n";
      for (int i = 0; i < 100; i++) {
          std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(vesp.MEMORY[i]) << " ";
          if ((i+1) % 16 == 0) {
              std::cout << "\n";
          }
      }
      std::cout << "\n";
```
Into the end of the `execute` function did print us everything we need.
```bash
The memory after execution in hex:
0B 0A 2000 00 2001 00 2010 00 2000 00 2001 01 2010 00 554 1000 
0A 2001 01 435 446 1000 7B7 2001 01 36D 756 1000 767 2001 01 6C3 
364 1000 5F6 2001 01 E30 2010 0A 2000 01 3001 10 6D3 757D 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00
```
Yes I was right as you can see here, the registers are dumped into the memory.
Note here for the `HLT` instuction the content of the IR register is : `757D` where 7 means the instuction op code and the `57D` is the content. 
Now Wrapping everything up we get:
```bash
... 5F6 E30 6D3 57D -> ... 5F 6E 30 6D 35 7D -> UMDCTF{smuggl3d_n0m5}
```





