  /******************************************************************************/ 
   /*                            VESP COMPUTER-Version 1.1                       *
   /*                            Author: A. Yavuz Oruc                           * 
   /*                            Copyright ¬© 2000,2004,2008.                    * 
   /*                                   AlgoritiX                                * 
   /*                              All rights reserved.                          *
   /*Copying and compiling this program for personal use is permitted. However,  *
   /*no part of this program may be reproduced or distributed in any form or     *
   /*by any means, or stored in a database or  retrieval system, without the     *
   /*prior written permission of the Author and AlgoritiX.                       *
   /*****************************************************************************/
/* This  computer program simulates the fetch,decode
/* and execute cycles of a hypothetical 16-bit computer, 
/* called VESP (_V_Ery _Simple _Processor)
/* Revised to reduce the address field to 12 bits. 2/13/03                        
/* VESP  has the following registers on which it executes 8 instructions:                   
/* A:     16 bits (Implicit) It refers to location 0 in  VESP's memory
/* B:     16 bits (Implicit) It refers to location 1 in  VESP's memory 
/* MAR:   12 bits  IR:     16 bits  PC:    12 bits
/* Its instruction repertoire consists of the following instructions: 
/* ADD: Add         Opcode: 0000  ----- A = A+B                          HexCode: 0
/* CMP: Compl       Opcode: 0001  ----- A = ~A                           HexCode: 1    
/* LDA: Load        Opcode: 0010  ----- M[IR[3:15] ] = M[MAR+1]          HexCode: 2
/* MOV: Move        Opcode: 0011  ----- M[IR[3:15] ] = M[M[MAR+1][3:15]] HexCode: 3  
/* JMP  Jump        Opcode: 0100  ----- PC = IR[3:15]                    HexCode: 4  
/* JEZ: Jump if 0   Opcode: 0101  ----- If (A = 0)  PC = IR[3:15]        HexCode: 5   
/* JPS: Jump if +   Opcode: 0110  ----- If (A > 0)  PC = IR[3:15]        HexCode: 6  
/* HLT: Hlt         Opcode: 0111  ----- reset = 1                        HexCode: 7 
                    Opcode: 1000  ----- TBA
                    Opcode: 1001  ----- TBA
                    Opcode: 1010  ----- TBA
                    Opcode: 1011  ----- TBA
                    Opcode: 1100  ----- TBA
                    Opcode: 1101  ----- TBA
                    Opcode: 1110  ----- TBA
                    Opcode: 1111  ----- TBA

Instruction opcodes 8 through 15 are left undefined for a future expansion of VESP.
*/
#include <iostream> 
#include <iomanip>
#include <stdio.h>
#include <limits.h> 
#include <unistd.h>

void initialize(void); int readprogram(void);
void displayprogram(int progend); 
void displayregisters(void); void displaymemory(void);
void maincycle(int trace);   void fetch(void);
void decode(void);           void execute(void);

//AYO: Define the registers of the architecture.
typedef struct 
{short   MAR,PC,IR,clock;
 short   MEMORY[8192],S,C,F,Z,reset,add,complement;
} architecture;  architecture   vesp;

int j=1;

using namespace std;

int main(void)
{int action,action2,progend;
 initialize();//AYO: Initialize all the registers.
   cout << "\nWelcome to Vesp 1.1\n\n";
while(1) {vesp.reset = false; //AYO: vesp keeps running unless it is reset.
//AYO: Input program, diplay registers or memory 
     cin.clear();
     cout << "\nType \n 0 to enter a program\n "
          << "1 to display registers\n 2 to display memory: ";
     cin >> action2; 
     cin.clear();
     cout << "\n";
//AYO: Read the program, display it, and execute it.    
if(action2 == 0) 
{progend = readprogram(); displayprogram(progend);
//AYO: Step through or execute the program.

while(vesp.reset == 0)
{cout << "\nEnter 0 if you wish to execute the program, 1 to step it, ";
 cout <<  "2 to exit to the main menu: ";
 cin >> action2;
 switch(action2)
 {case 0: cout << "Enter 0 for a brief trace 1 for long trace: "; 
          cin.clear(); cin >> action; 
          cout << "\n";
          while(vesp.reset == 0) maincycle(action); break;
  case 1: maincycle(1);    break;
  case 2: vesp.reset = true; break;
 }
}
 
//AYO: Display the number of instructions and clock cycles executed.
if(action2 != 2)
{cout << "The number of instructions executed  = " << dec << j-1  << "\n";  
cout << "The number of clock cycles used = " << dec << vesp.clock << "\n";  
j = 1; vesp.clock = 0;
}
}
if (action2 == 1) displayregisters();
if (action2 == 2) displaymemory();
} 
}

void initialize(void)
{vesp.PC = vesp.MEMORY[0] = vesp.MEMORY[1] = vesp.IR = 0;
 vesp.reset = 0; vesp.clock = 0;
}

int readprogram(void)
{int address,instruction,progend,action; //,i;
 //char longfilename[100] ="/UMD-Web-Dreamweaver/teaching/courses/enee350/vesp-source-code/vesp1.1X/";
 char filename[25]; 
 //The above path needs to be changed if the file is located in another directory. 
 FILE *file;
 do{ cout << "Enter your program's starting " 
      << "address ( >= 2) as a 3-digit hex number: ";
  cin >> hex >> vesp.PC; 
 } while (vesp.PC < 2);
 
 address =  vesp.PC; action = -1;
 cout << "\n";
 do { cout << "Enter 0 to type in your program or 1 to read it from a file: ";
 cin >> action;
 } while (action != 0 && action != 1);
 
 if(action == 1) 
 {cout << "\nEnter the file name: "; cin >> filename;
 
 
 file = fopen(filename,"r");
 if( file != NULL)
 {
 while (fscanf(file,"%x",&instruction) != EOF  &&  address < 8192 ) 
  {
   vesp.MEMORY[address] = instruction; address = address + 1;
   }
 fclose(file); 
 }
 else
 {//for (i=0; i <= 24; i++) longfilename[i+72] = filename[i]; 
  file = fopen(filename,"r");
  if( file != NULL)
  {
   while (fscanf(file,"%x",&instruction) != EOF  &&  address < 8192 ) 
   {vesp.MEMORY[address] = instruction; address = address + 1;}
  fclose(file); 
  }
  else
  {cout << "The file is not found. Check if file to be opened is in the program directory... \n"; exit(1);}
 
 
 }
 }
 else  
 do {cin.clear();
     cout << "Enter instruction  " 
          << (address -vesp.PC)  
          << " using a 4-digit hex number" << "\n";
     cout << "Or type -1 to end your program: ";
     
     cin >> hex >> instruction;   //AYO: read it in hex.
      vesp.MEMORY[address] = instruction;
      address = address + 1;
    }
 while ( ( vesp.MEMORY[address-1] !=  -1 ) && (address < 8192)); //AYO: -1 marks the end.

if (address >= 4096) 
{cout << "Memory overflow," 
      << "Please quit from the file menu and restart me!"; 
return address-1;}
progend = address - 1; 
return progend;

}

void displayprogram(int progend) 
{int i; 
cout << "\nHere is your program: \n\n";
for (i = vesp.PC; i<= progend; i++)
{
cout << "Location "; cout.fill('0');  cout.width(4); cout.setf(ios::uppercase);
cout <<  hex << i << ": ";  //AYO: display it in uppercase hex. 

cout.fill('0');  cout.width(4); cout.setf(ios::uppercase);
cout << hex << (0x0000FFFF & vesp.MEMORY[i]) << "\n";
}
}
void displayregisters(void)
{
cout << "A = ";  cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
cout << hex << (0x0000FFFF & vesp.MEMORY[0])<< ", "; 
cout << "B = ";  cout.fill('0'); cout.width(4); cout.setf(ios::uppercase); 
cout << hex << (0x0000FFFF & vesp.MEMORY[1])<< ", "; 
cout << "Z = " <<  vesp.Z << ", ";
cout << "S = " <<  vesp.S << ", ";
cout << "C = " <<  vesp.C << ", ";
cout << "F = " <<  vesp.F << "\n";
cout << "MAR = "; cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
cout << hex << vesp.MAR << ", "; 
cout << "PC = ";  cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
cout << hex << vesp.PC << ", "; 
cout << "IR = "; cout.fill('0');  cout.width(4); cout.setf(ios::uppercase); 
cout << hex << vesp.IR << ", "; 
cout << "reset = " << vesp.reset << "\n";
}

void displaymemory(void)
{int location1,location2,i; 
 cout << "Enter the first address (3 hex digits): ";
 cin.clear();
 cin >> hex >> location1;
 cin.clear();
 cout << "Enter the last address: (3 hex digits): ";
 cin.clear();
 cin >> hex >> location2;
 cin.clear();
 for (i = location1; i <= location2; i++)
 {cout <<  "Location "; cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
 cout << hex << i << " : "; 
 cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
 cout << hex << (0x0000FFFF & vesp.MEMORY[i]); cout << "\n"; 
}
}

void maincycle(int trace)
{  
    
	 cout << "Machine Cycle "; 
	 cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
	 cout << hex << j << ": ";


       j = j+1;
       //AYO: Fetch Step
        cout << "PC = ";  cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
        cout << hex << (vesp.PC & 0x0FFF) << ", ";     
        cout << "\nFETCH SUBCYCLE\n";
        fetch();
        cout << "\nClock cycle = " << vesp.clock << "\n";
        //AYO: Decode Step
        cout << "DECODE SUBCYCLE\n"; 
        decode();
        cout << "Clock cycle = " << vesp.clock << "\n";               
        //AYO: Execute Step   
        cout << "EXECUTE SUBCYCLE";  
        vesp.add = vesp.complement = 0;  
        execute(); 
        cout << "\nClock cycle = " << vesp.clock << "\n\n";
        //AYO: Display the registers 
        
       if(trace == 1)
       {cout << "*************Begin[Machine Level]*****************\n\n";
        displayregisters();  
        cout << "add = " << vesp.add << " "; 
        cout << "complement = " << vesp.complement << "\n\n"; 
        if( (vesp.IR >> 12 ) == 2  ||  (vesp.IR   >> 12 ) == 3) 
        {cout << "Memory["; 
         cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
         cout << hex << (vesp.IR & 0x0FFF) << "] = ";
         cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
         cout << hex << (0x0000FFFF & vesp.MEMORY[vesp.IR & 0x0FFF])<< "\n\n";
         cout << "*************End[Machine Level]*****************";  
         } 
        }     
}

void fetch(void)
{       //clock cycle 1.  Load next instruction's address into MAR. 
        vesp.MAR =  vesp.PC; vesp.PC =   vesp.PC +1; //Increment PC. 
		vesp.clock = vesp.clock +1; 
        //clock cycle 2.  Fetch the next Instruction into IR 
        cout << "MAR = ";   cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
        cout << hex << vesp.MAR << ", "; 
        vesp.IR = vesp.MEMORY[vesp.MAR]; vesp.clock = vesp.clock +1; 
        cout << "IR = ";   cout.fill('0'); cout.width(4); cout.setf(ios::uppercase);
        cout << hex << vesp.IR << ", "; 
}

void decode(void)
{cout << "Decoded instruction is: ";  
     switch( vesp.IR   >> 12)   
         {//Add                            //Complement
          case  0: cout << "ADD\n"; break; case  1: cout << "CMP\n"; break;          
         //Load                            //Move
          case  2: cout << "LDA\n"; break; case  3: cout << "MOV\n"; break;
         //Jump                            //Jump if A = 0
          case  4: cout << "JMP\n"; break; case  5: cout << "JEZ\n"; break;
         //Jump if A > 0                   //Halt
          case  6: cout << "JPS\n"; break; case  7: cout << "HLT\n"; break;
          }                    
}


void execute(void)
{ short temp;
  switch(vesp.IR >> 12) 
         {//clock cycle 3.
           //Add
          case  0:  
          temp = vesp.MEMORY[0] + vesp.MEMORY[1]; vesp.clock = vesp.clock +1;                                        
          if(vesp.MEMORY[0] > 0 && vesp.MEMORY[1] > 0 && temp  <  0 || 
          vesp.MEMORY[0] < 0 && vesp.MEMORY[1] < 0  && temp  >= 0) 
          vesp.F = 1; else vesp.F = 0; //AYO: Set Overflow Flag
          if (vesp.MEMORY[0] < 0 && vesp.MEMORY[1] < 0  || temp > 0 && 
          (vesp.MEMORY[0] < 0 && vesp.MEMORY[1] > 0 || vesp.MEMORY[0] > 0 && vesp.MEMORY[1] < 0)) 
          vesp.C = 1; else vesp.C = 0; //AYO: Set Carry Flag
          vesp.MEMORY[0]  = temp;    //Save the sum in MEMORY[0]
          //AYO: Set Zero Flag
          if(vesp.MEMORY[0]  == 0) vesp.Z = 1; else vesp.Z = 0; 
          //AYO: Set Sign Flag          
          vesp.S = (vesp.MEMORY[0] & 0x8000 ) >> 15;  vesp.add = 1; break;
         //Complement
          case  1:  vesp.MEMORY[0] = ~vesp.MEMORY[0]; 
                    vesp.clock = vesp.clock +1;  
                    if(vesp.MEMORY[0]  == 0)  vesp.Z = 1; else vesp.Z = 0; 
                    vesp.S = (vesp.MEMORY[0] & 0x8000 ) >> 15;  
                    vesp.complement = 1; break;
         //Load         
         case  2:   vesp.MEMORY[vesp.IR&0x0FFF] =  
                    vesp.MEMORY[(vesp.MAR) + 1]; 
                    vesp.clock = vesp.clock +1; vesp.PC = vesp.PC + 1; break;  
         //Move
         case 3:   vesp.MEMORY[vesp.IR&0x0FFF] = 
                    vesp.MEMORY[vesp.MEMORY[(vesp.MAR) + 1]];   
                    vesp.clock = vesp.clock + 2; vesp.PC = vesp.PC + 1; break;
         //Jump
         case 4:   vesp.PC = vesp.IR & 0x1FFF;vesp.clock = vesp.clock +1; break;  
         //Branch if A is  0
         case 5:   if (vesp.MEMORY[0] == 0) 
                    {vesp.PC = vesp.IR & 0x0FFF;} 
                    vesp.clock = vesp.clock +1; break;  
         //Branch if A is > 0
         case 6: if (vesp.MEMORY[0] > 0) 
                   {vesp.PC = vesp.IR & 0x0FFF;} 
                    vesp.clock = vesp.clock +1; break;  
         //Halt
         case 7:  vesp.reset = true;  vesp.clock = vesp.clock +1; break;  
      }
      std::cout << "\nThe memory after execution in hex:\n";
      for (int i = 0; i < 100; i++) {
          std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(vesp.MEMORY[i]) << " ";
          if ((i+1) % 16 == 0) {
              std::cout << "\n";
          }
      }
      std::cout << "\n";

}


