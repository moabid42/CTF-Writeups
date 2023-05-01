# CUTTER

## Description
In this challenge we were provided with :
`clutter.vsp`
`how_to_vesp-2.pdf`
As you can see the author was nice enough to give us ths manual to use and run the vsp files.

## Solution
After reading the manual, I installed and ran the emulator. Following the instructions I got the `output.txt`, and from it we are only intrested in the memory, so with some quick bash we could get :
```bash
$ grep Memory vesp_out.txt | grep -v "\[0000\]" | grep -v "\[0001\]"
    Memory[015B] = 0055
    Memory[0285] = 004D
    Memory[0185] = 0044
    Memory[022A] = 0043
    Memory[011D] = 0054
    Memory[0153] = 0046
    Memory[02E9] = 007B
    Memory[0266] = 0055
    Memory[029E] = 0078
    Memory[02A7] = 0031
    Memory[0213] = 0033
    Memory[0298] = 002D
    Memory[0121] = 0075
    Memory[01C7] = 0073
    Memory[01B5] = 0033
    Memory[0256] = 002D
    Memory[0265] = 006D
    Memory[0150] = 0033
    Memory[0159] = 006D
    Memory[028C] = 0030
    Memory[0134] = 0072
    Memory[0268] = 0079
    Memory[02E7] = 002D
    Memory[01DE] = 0077
    Memory[0214] = 0031
    Memory[023A] = 0070
    Memory[019C] = 0033
    Memory[0171] = 0021
    Memory[012A] = 007D
    Memory[0153] = 000A
```
and as you can see here we can clearly see the flag.
And with some more bash script we can easily get the flag :
```bash
$ grep Memory vesp_out.txt | grep -v "\[0000\]" | grep -v "\[0001\]" | cut -d = -f2 | xxd -r -p
```
