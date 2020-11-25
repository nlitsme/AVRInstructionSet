# investigating the AVR instruction set.

## Unallocated opcodes

There are some unallocated opcodes, they are listed at the bottom of the instruction table below.
I used [this python](analyze_avr_insns.py) script to verify that the table below has all possible bit combinations listed.

Then i investigated the unallocated codes using [this arduino project](insntest.ino)

## conditions

| Test  | Boolean  | Mnemonic  | Complementary  | Boolean  | Mnemonic  | Comment  |
| :---  | :------  | :-------  | :------------  | :------  | :-------  | :------  |
| Rd > Rr  | Z & (N ^ V) = 0  | BRLT(1)  | Rd <= Rr  | Z+(N ^ V) = 1  | BRGE*  | Signed  |
| Rd >= Rr  | (N ^ V) = 0  | BRGE  | Rd < Rr  | (N ^ V) = 1  | BRLT  | Signed  |
| Rd = Rr  | Z=1  | BREQ  | Rd != Rr  | Z=0  | BRNE  | Signed  |
| Rd <= Rr  | Z+(N ^ V) = 1  | BRGE(1)  | Rd > Rr  | Z & (N ^ V) = 0  | BRLT*  | Signed  |
| Rd < Rr  | (N ^ V) = 1  | BRLT  | Rd >= Rr  | (N ^ V) = 0  | BRGE  | Signed  |
| Rd > Rr  | C+Z=0  | BRLO(1)  | Rd <= Rr  | C+Z=1  | BRSH*  | Unsigned  |
| Rd >= Rr  | C=0  | BRSH/BRCC  | Rd < Rr  | C=1  | BRLO/BRCS  | Unsigned  |
| Rd = Rr  | Z=1  | BREQ  | Rd != Rr  | Z=0  | BRNE  | Unsigned  |
| Rd <= Rr  | C+Z=1  | BRSH(1)  | Rd > Rr  | C+Z=0  | BRLO*  | Unsigned  |
| Rd < Rr  | C=1  | BRLO/BRCS  | Rd >= Rr  | C=0  | BRSH/BRCC  | Unsigned  |
| Carry  | C=1  | BRCS  | No carry  | C=0  | BRCC  | Simple  |
| Negative  | N=1  | BRMI  | Positive  | N=0  | BRPL  | Simple  |
| Overflow  | V=1  | BRVS  | No overflow  | V=0  | BRVC  | Simple  |
| Zero  | Z=1  | BREQ  | Not zero  | Z=0  | BRNE  | Simple  |


## instructions

A prettier table can be found [here](http://htmlpreview.github.io/?https://github.com/nlitsme/arduino/blob/master/AVRInstructionTable.html)

| Opcode  | Mnemonics  | Operands  | Description  | Operation  | Flags  | #Clocks  | #Clocks XMEGA  | Note  | Remarks  |
| :-----  | :--------  | :-------  | :----------  | :--------  | :----  | :------  | :------------  | :---  | :------  |
| **Arithmetic and Logic Instructions** ||||||||| |
| 0000 11rd dddd rrrr  | ADD  | Rd, Rr  | Add without Carry  | Rd ← Rd+Rr  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0001 11rd dddd rrrr  | ADC  | Rd, Rr  | Add with Carry  | Rd ← Rd+Rr+C  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 1001 0110 KKdd KKKK  | ADIW  | Rd, K  | Add Immediate to Word  | Rd ← Rd+1:Rd+K  | Z,C,N,V,S  | 2  | -  | (1)   |  d in {24,26,28,30}  |
| 0001 10rd dddd rrrr  | SUB  | Rd, Rr  | Subtract without Carry  | Rd ← Rd-Rr  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0101 KKKK dddd KKKK  | SUBI  | Rd, K  | Subtract Immediate  | Rd ← Rd-K  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0000 10rd dddd rrrr  | SBC  | Rd, Rr  | Subtract with Carry  | Rd ← Rd-Rr-C  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0100 KKKK dddd KKKK  | SBCI  | Rd, K  | Subtract Immediate with Carry  | Rd ← Rd-K-C  | Z,C,N,V,S,H  | 1  | -  |     | d=16..31 |
| 1001 0111 KKdd KKKK  | SBIW  | Rd, K  | Subtract Immediate from Word  | Rd+1:Rd ← Rd+1:Rd-K  | Z,C,N,V,S  | 2  | -  | (1)    | d in {24,26,28,30} |
| 0010 00rd dddd rrrr  | AND  | Rd, Rr  | Logical AND  | Rd ← Rd & Rr  | Z,N,V,S  | 1  | -  |     |     |
| 0111 KKKK dddd KKKK  | ANDI  | Rd, K  | Logical AND with Immediate  | Rd ← Rd & K  | Z,N,V,S  | 1  | -  |     | d = 16..31 |
| 0010 10rd dddd rrrr  | OR  | Rd, Rr  | Logical OR  | Rd ← Rd|Rr  | Z,N,V,S  | 1  | -  |     | 
| 0110 KKKK dddd KKKK  | ORI  | Rd, K  | Logical OR with Immediate  | Rd ← Rd|K  | Z,N,V,S  | 1  | -  |     |
| 0010 01rd dddd rrrr  | EOR  | Rd, Rr  | Exclusive OR  | Rd ← Rd^Rr  | Z,N,V,S  | 1  | -  |     |     |
| 1001 010d dddd 0000  | COM  | Rd  | One's Complement  | Rd ← $FF-Rd  | Z,C,N,V,S  | 1  | -  |     |     |
| 1001 010d dddd 0001  | NEG  | Rd  | Two's Complement  | Rd ← $00-Rd  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0110 KKKK dddd KKKK  | SBR  | Rd,K  | Set Bit(s) in Register  | Rd ← Rd or K  | Z,N,V,S  | 1  | -  |     | _alias_ for ORI Rd,K |
| 0111 KKKK dddd KKKK  | CBR  | Rd,K  | Clear Bit(s) in Register  | Rd ← Rd & ($FFh-K)  | Z,N,V,S  | 1  | -  |     |  _alias_ for ANDI |
| 1001 010d dddd 0011  | INC  | Rd  | Increment  | Rd ← Rd+1  | Z,N,V,S  | 1  | -  |     |     |
| 1001 010d dddd 1010  | DEC  | Rd  | Decrement  | Rd ← Rd-1  | Z,N,V,S  | 1  | -  |     |     |
| 0010 00dd dddd dddd  | TST  | Rd  | Test for Zero or Minus  | Rd ← Rd & Rd  | Z,N,V,S  | 1  | -  |     | _alias_ for AND Rd,Rd |
| 0010 01dd dddd dddd  | CLR  | Rd  | Clear Register  | Rd ← Rd xor Rd  | Z,N,V,S  | 1  | -  |     | _alias_ for EOR Rd,Rd |
| 1110 1111 dddd 1111  | SER  | Rd  | Set Register  | Rd ← $FF  | None  | 1  | -  |     | _alias_ for LDI Rd,0xFF |
| 1001 11rd dddd rrrr  | MUL  | Rd,Rr  | Multiply Unsigned  | R1:R0 ← Rd x Rr (UU)  | Z,C  | 2  | -  | (1)  | |
| 0000 0010 dddd rrrr  | MULS  | Rd,Rr  | Multiply Signed  | R1:R0 ← Rd x Rr (SS)  | Z,C  | 2  | -  | (1)  | d,r=16..31 |
| 0000 0011 0ddd 0rrr  | MULSU  | Rd,Rr  | Multiply Signed with Unsigned  | R1:R0 ← Rd x Rr (SU)  | Z,C  | 2  | -  | (1)  | d,r=16..23 |
| 0000 0011 0ddd 1rrr  | FMUL  | Rd,Rr  | Fractional Multiply Unsigned  | R1:R0 ← Rd x Rr<<1 (UU)  | Z,C  | 2  | -  | (1)  |r,d = 16..23 |
| 0000 0011 1ddd 0rrr  | FMULS  | Rd,Rr  | Fractional Multiply Signed  | R1:R0 ← Rd x Rr<<1 (SS)  | Z,C  | 2  | -  | (1)  |r,d = 16..23 |
| 0000 0011 1ddd 1rrr  | FMULSU  | Rd,Rr  | Fractional Multiply Signed with Unsigned  | R1:R0 ← Rd x Rr<<1 (SU)  | Z,C  | 2  | -  | (1)  |r,d = 16..23 |
| 1001 0100 KKKK 1011  | DES  | K  | Data Encryption  | if (H = 0) then R15:R0 ← Encrypt(R15:R0, K) else if (H = 1) then R15:R0 ← Decrypt(R15:R0, K)  | -  | -  | 1/2  |     |     |
| **Branch Instructions**  ||||||||||
| 1100 kkkk kkkk kkkk  | RJMP  | k  | Relative Jump  | PC ← PC+k+1  | None  | 2  | -  |     |     |
| 1001 0100 0000 1001  | IJMP  | Z  | Indirect Jump to (Z)  | PC(15:0) ← Z, PC(21:16) ← 0  | None  | 2  | -  | (1)  | |
| 1001 0100 0001 1001  | EIJMP  | EIND:Z  | Extended Indirect Jump to (Z)  | PC(15:0) ← Z, PC(21:16) ← EIND  | None  | 2  | -  | (1)  | |
| 1001 010k kkkk 110k **K** | JMP  | k  | Jump  | PC ← k  | None  | 3  | -  | (1)  | two words |
| 1101 kkkk kkkk kkkk  | RCALL  | k  | Relative Call Subroutine  | PC ← PC+k+1  | None  | 3 / 4(3)(5)  | 2 / 3(3)  |   | |
| 1001 0101 0000 1001  | ICALL  | Z  | Indirect Call to (Z)  | PC(15:0) ← Z, PC(21:16) ← 0  | None  | 3 / 4(3)  | 2 / 3(3)  | (1)  | |
| 1001 0101 0001 1001  | EICALL  | EIND:Z  | Extended Indirect Call to (Z)  | PC(15:0) ← Z, PC(21:16) ← EIND  | None  | 4 (3)  | 3 (3)  | (1)  | |
| 1001 010k kkkk 111k **K** | CALL  | k  | call Subroutine  | PC ← k, STACK ← PC, SP ← SP-2  | None  | 4 / 5(3)  | 3 / 4(3)  | (1)  |  two words |
| 1001 0101 0000 1000  | RET  | -  | Subroutine Return  | PC ← STACK, SP ← SP+2  | None  | 4 / 5(3)  | -  |     |     |
| 1001 0101 0001 1000  | RETI  | -  | Interrupt Return  | PC ← STACK, SP ← SP+2  | I  | 4 / 5(3)  | -  |     |     |
| 0001 01rd dddd rrrr  | CP  | Rd,Rr  | Compare  | Rd - Rr  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0000 01rd dddd rrrr  | CPC  | Rd,Rr  | Compare with Carry  | Rd - Rr - C  | Z,C,N,V,S,H  | 1  | -  |     |     |
| 0011 KKKK dddd KKKK  | CPI  | Rd,K  | Compare with Immediate  | Rd - K  | Z,C,N,V,S,H  | 1  | -  |     |  d = 16..31 |
| 0001 00rd dddd rrrr  | CPSE  | Rd,Rr  | Compare, Skip if Equal  | if(Rd=Rr)PC ← PC+2or3  | None  | 1/2/3  | -  |     |     |
| 1111 110r rrrr 0bbb  | SBRC  | Rr, b  | Skip if Bit in Register Cleared  | if(Rr(b)=0)PC ← PC+2or3  | None  | 1/2/3  | -  |     |     |
| 1111 111r rrrr 0bbb  | SBRS  | Rr, b  | Skip if Bit in Register Set  | if(Rr(b)=1)PC ← PC+2or3  | None  | 1/2/3  | -  |     |     |
| 1001 1001 AAAA Abbb  | SBIC  | A, b  | Skip if Bit in I/O Register Cleared  | if(I/O(A,b)=0)PC ← PC+2or3  | None  | 1/2/3  | 2/3/4  |     |     |
| 1001 1011 AAAA Abbb  | SBIS  | A, b  | Skip if Bit in I/O Register Set  | If(I/O(A,b)=1)PC ← PC+2or3  | None  | 1/2/3  | 2/3/4  |     |     |
| 1111 00kk kkkk k000  | BRCS  | k  | Branch if Carry Set  | if(C=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k000  | BRLO  | k  | Branch if Lower  | if(C=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k001  | BREQ  | k  | Branch if Equal  | if(Z=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k010  | BRMI  | k  | Branch if Minus  | if(N=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k011  | BRVS  | k  | Branch if Overflow Flag is Set  | if(V=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k100  | BRLT  | k  | Branch if Less Than, Signed  | if(N^V=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k101  | BRHS  | k  | Branch if Half Carry Flag Set  | if(H=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k110  | BRTS  | k  | Branch if T Flag Set  | if(T=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk k111  | BRIE  | k  | Branch if Interrupt Enabled  | if(I=1)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 00kk kkkk ksss  | BRBS  | s, k  | Branch if Status Flag Set  | if(SREG(s)=1)then PC ← PC+k+1  | None  | 1/2  | -  |     |  |
| 1111 01kk kkkk k000  | BRCC  | k  | Branch if Carry Cleared  | if(C=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k000  | BRSH  | k  | Branch if Same or Higher  | if(C=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k001  | BRNE  | k  | Branch if Not Equal  | if(Z=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k010  | BRPL  | k  | Branch if Plus  | if(N=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k011  | BRVC  | k  | Branch if Overflow Flag is Cleared  | if(V=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k100  | BRGE  | k  | Branch if Greater or Equal, Signed  | if(N^V=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k101  | BRHC  | k  | Branch if Half Carry Flag Cleared  | if(H=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k110  | BRTC  | k  | Branch if T Flag Cleared  | if(T=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk k111  | BRID  | k  | Branch if Interrupt Disabled  | if(I=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | _alias_ |
| 1111 01kk kkkk ksss  | BRBC  | s, k  | Branch if Status Flag Cleared  | if(SREG(s)=0)then PC ← PC+k+1  | None  | 1/2  | -  |     | |
| **Data Transfer Instructions**  ||||||||||
| 0010 11rd dddd rrrr  | MOV  | Rd, Rr  | Copy Register  | Rd ← Rr  | None  | 1  | -  |     |     |
| 0000 0001 dddd rrrr  | MOVW  | Rd, Rr  | Copy Register Pair  | Rd+1:Rd ← Rr+1:Rr  | None  | 1  | -  | (1)  | |
| 1110 KKKK dddd KKKK  | LDI  | Rd, K  | Load Immediate  | Rd ← K  | None  | 1  | -  |     | d=16..31 |
| 1001 000d dddd 0000 **K** | LDS  | Rd, k  | Load Direct from data space  | Rd ← (k)  | None  | 1(5)/2(3)  | 2(3)(4)  | (1)  | two words |
| 1010 0kkk dddd kkkk  | LDS  | Rd, k  | Load Direct from data space  | Rd ← (k)  | None  | 1(5)/2(3)  | 2(3)(4)  | (1)  | AVRrc only |
| 1001 000d dddd 1100  | LD  | Rd, X  | Load Indirect  | Rd ← (X)  | None  | 1(5)2(3)  | 1(3)(4)  | (2)  | |
| 1001 000d dddd 1101  | LD  | Rd, X+  | Load Indirect and Post-Increment  | Rd ← (X) X←X+1  | None  | 2(3)  | 1(3)(4)  | (2)  | |
| 1001 000d dddd 1110  | LD  | Rd, -X  | Load Indirect and Pre-Decrement  | X←X-1, ← X-1 Rd←(X) ← (X)  | None  | 2(3)/3(5)  | 2(3)(4)  | (2)  | |
| 1000 000d dddd 1000  | LD  | Rd, Y  | Load Indirect  | Rd←(Y) ← (Y)  | None  | 1(5)/2(3)  | 1(3)(4)  | (2)  | _alias_ for LDD Rd,Y+0 |
| 1001 000d dddd 1001  | LD  | Rd, Y+  | Load Indirect and Post-Increment  | Rd ← (Y) Y←Y+1  | None  | 2(3)  | 1(3)(4)  | (2)  | |
| 1001 000d dddd 1010  | LD  | Rd, -Y  | Load Indirect and Pre-Decrement  | Y←Y-1 Rd ← (Y)  | None  | 2(3)/3(5)  | 2(3)(4)  | (2)  | |
| 10q0 qq0d dddd 1qqq  | LDD  | Rd, Y+q  | Load Indirect with Displacement  | Rd ← (Y+q)  | None  | 2(3)  | 2(3)(4)  | (1)  | |
| 1000 000d dddd 0000  | LD  | Rd, Z  | Load Indirect  | Rd ← (Z)  | None  | 1(5)/2(3)  | 1(3)(4)  | (2)  | _alias_ for LDD Rd,Z+0 |
| 1001 000d dddd 0001  | LD  | Rd, Z+  | Load Indirect and Post-Increment  | Rd ← (Z), Z ← Z+1  | None  | 2(3)  | 1(3)(4)  | (2)  | |
| 1001 000d dddd 0010  | LD  | Rd, -Z  | Load Indirect and Pre-Decrement  | Z ← Z-1, Rd ← (Z)  | None  | 2(3)/3(5)  | 2(3)(4)  | (2)  | |
| 10q0 qq0d dddd 0qqq  | LDD  | Rd, Z+q  | Load Indirect with Displacement  | Rd ← (Z+q)  | None  | 2(3)  | 2(3)(4)  | (1)  | |
| 1001 001d dddd 0000 **K** | STS  | k, Rr  | Store Direct to Data Space  | (k) ← Rd  | None  | 1(5)/2(3)  | 2(3)  | (1)  | two words |
| 1010 1kkk dddd kkkk  | STS  | k, Rr  | Store Direct to Data Space  | (k) ← Rd  | None  | 1(5)/2(3)  | 2(3)  | (1)  | AVRrc only |
| 1001 001r rrrr 1100  | ST  | X, Rr  | Store Indirect  | (X) ← Rr  | None  | 1(5)/2(3)  | 1(3)  | (2)  | |
| 1001 001r rrrr 1101  | ST  | X+, Rr  | Store Indirect and Post-Increment  | (X) ← Rr, X←X+1  | None  | 1(5)/2(3)  | 1(3)  | (2)  | |
| 1001 001r rrrr 1110  | ST  | -X, Rr  | Store Indirect and Pre-Decrement  | X ← X-1, (X) ← Rr  | None  | 2(3)  | 2(3)  | (2)  | |
| 1000 001r rrrr 1000  | ST  | Y, Rr  | Store Indirect  | (Y) ← Rr  | None  | 1(5)/2(3)  | 1(3)  | (2)  | _alias_ for STD Y+0,Rr |
| 1001 001r rrrr 1001  | ST  | Y+, Rr  | Store Indirect and Post-Increment  | (Y) ← Rr, Y←Y+1  | None  | 1(5)/2(3)  | 1(3)  | (2)  | |
| 1001 001r rrrr 1010  | ST  | -Y, Rr  | Store Indirect and Pre-Decrement  | Y ← Y-1, (Y) ← Rr  | None  | 2(3)  | 2(3)  | (2)  | |
| 10q0 qq1r rrrr 1qqq  | STD  | Y+q, Rr  | Store Indirect with Displacement  | (Y+q) ← Rr  | None  | 2(3)  | 2(3)  | (1)  | |
| 1000 001r rrrr 0000  | ST  | Z, Rr  | Store Indirect  | (Z) ← Rr  | None  | 1(5)/2(3)  | 1(3)  | (2)  | _alias_ for STD Z+0,Rr |
| 1001 001r rrrr 0001  | ST  | Z+, Rr  | Store Indirect and Post-Increment  | (Z) ← Rr Z←Z+1  | None  | 1(5)/2(3)  | 1(3)  | (2)  | |
| 1001 001r rrrr 0010  | ST  | -Z, Rr  | Store Indirect and Pre-Decrement  | Z←Z-1  | None  | 2(3)  | 2(3)  | (2)  | |
| 10q0 qq1r rrrr 0qqq  | STD  | Z+q,Rr  | Store Indirect with Displacement  | (Z+q) ← Rr  | None  | 2(3)  | 2(3)  | (1)  | |
| 1001 0101 1100 1000  | LPM  | -  | Load Program Memory  | R0 ← (Z)  | None  | 3  | 3  | (1)(2)  | |
| 1001 000d dddd 0100  | LPM  | Rd, Z  | Load Program Memory  | Rd ← (Z)  | None  | 3  | 3  | (1)(2)  | |
| 1001 000d dddd 0101  | LPM  | Rd, Z+  | Load Program Memory and Post- Increment  | Rd ← (Z), Z←Z+1  | None  | 3  | 3  | (1)(2)  | |
| 1001 0101 1101 1000  | ELPM  | R0,Z  | Extended Load Program Memory  | R0 ← (RAMPZ:Z)  | None  | 3  | -  | (1)  | |
| 1001 000d dddd 0110  | ELPM  | Rd, Z  | Extended Load Program Memory  | Rd ← (RAMPZ:Z)  | None  | 3  | -  | (1)  | |
| 1001 000d dddd 0111  | ELPM  | Rd, Z+  | Extended Load Program Memory and Post-Increment  | Rd ← (RAMPZ:Z), Z←Z+1  | None  | 3  | -  | (1)  | |
| 1001 0101 1110 1000  | SPM  | Z  | Store Program Memory  | (RAMPZ:Z) ← R1:R0  | None  | -  | -  | (1)  | |
| 1001 0101 1111 1000  | ESPM  | Z+  | Store Program Memory and Post- Increment by 2  | (RAMPZ:Z) ← R1:R0, Z←Z+2  | None  | -  | -  | (1)  | |
| 1011 0AAd dddd AAAA  | IN  | Rd, A  | In From I/O Location  | Rd ← I/O(A)  | None  | 1  | -  |     |     |
| 1011 1AAr rrrr AAAA  | OUT  | A, Rr  | Out To I/O Location  | I/O(A) ← Rr  | None  | 1  | -  |     |     |
| 1001 001d dddd 1111  | PUSH  | Rr  | Push Register on Stack  | STACK ← Rr, SP ← SP-1  | None  | 2  | 1(3)  | (1)  | |
| 1001 000d dddd 1111  | POP  | Rd  | Pop Register from Stack  | SP ← SP+1, Rd ← STACK  | None  | 2  | 2(3)  | (1)  | |
| 1001 001r rrrr 0100  | XCH  | Z, Rd  | Exchange  | (Z) ← Rd, Rd ← (Z)  | None  | 1  | -  |     |     |
| 1001 001r rrrr 0101  | LAS  | Z, Rd  | Load and Set  | (Z) ← Rd|(Z) Rd ← (Z)  | None  | 1  | -  |     | 
| 1001 001r rrrr 0110  | LAC  | Z, Rd  | Load and Clear  | (Z) ← ($FF - Rd) & (Z) Rd ← (Z)  | None  | 1  | -  |     |     |
| 1001 001r rrrr 0111  | LAT  | Z, Rd  | Load and Toggle  | (Z) ← Rd^(Z) Rd ← (Z)  | None  | 1  | -  |     |     |
| **Bit and Bit-test Instructions**  ||||||||||
| 0000 11dd dddd dddd  | LSL  | Rd  | Logical Shift Left  | Rd(n+1) ← Rd(n), Rd(0) ← 0, C ← Rd(7)  | Z,C,N,V,H  | 1  | -  |     | _alias_ for ADD Rd,Rd |
| 1001 010d dddd 0110  | LSR  | Rd  | Logical Shift Right  | Rd(n) ← Rd(n+1), Rd(7) ← 0, C ← Rd(0)  | Z,C,N,V  | 1  | -  |     |     |
| 0001 11dd dddd dddd  | ROL  | Rd  | Rotate Left Through Carry  | Rd(0) ← C, Rd(n+1) ← Rd(n), C ← Rd(7)  | Z,C,N,V,H  | 1  | -  |     | _alias_ for ADC Rd,Rd |
| 1001 010d dddd 0111  | ROR  | Rd  | Rotate Right Through Carry  | Rd(7) ← C, Rd(n) ← Rd(n+1), C ← Rd(0)  | Z,C,N,V  | 1  | -  |     |     |
| 1001 010d dddd 0101  | ASR  | Rd  | Arithmetic Shift Right  | Rd(n) ← Rd(n+1), n=0..6, Rd(7) ← Rd(7), C ← Rd(0)  | Z,C,N,V  | 1  | -  |     |     |
| 1001 010d dddd 0010  | SWAP  | Rd  | Swap Nibbles  | Rd(3..0) ↔ Rd(7..4)  | None  | 1  | -  |     |     |
| 1001 0100 0sss 1000  | BSET  | s  | Flag Set  | SREG(s) ← 1  | SREG(s)  | 1  | -  |     |     |
| 1001 0100 1sss 1000  | BCLR  | s  | Flag Clear  | SREG(s) ← 0  | SREG(s)  | 1  | -  |     | s = 0-7 = C,Z,N,V,S,H,T,I |
| 1001 1010 AAAA Abbb  | SBI  | A, b  | Set Bit in I/O Register  | I/O(A,b) ← 1  | None  | 1(5)2  | 1  |     |     |
| 1001 1000 AAAA Abbb  | CBI  | A, b  | Clear Bit in I/O Register  | I/O(A,b) ← 0  | None  | 1(5)/2  | 1  |     |     |
| 1111 101d dddd 0bbb  | BST  | Rr, b  | Bit Store from Register to T  | T ← Rr(b)  | T  | 1  | -  |     |     |
| 1111 100d dddd 0bbb  | BLD  | Rd, b  | Bit load from T to Register  | Rd(b) ← T  | None  | 1  | -  |     |     |
| 1001 0100 0000 1000  | SEC  | -  | Set Carry  | C←1  | C  | 1  | -  |     | _alias_ |
| 1001 0100 1000 1000  | CLC  | -  | Clear Carry  | C←0  | C  | 1  | -  |     | _alias_ |
| 1001 0100 0010 1000  | SEN  | -  | Set Negative Flag  | N←1  | N  | 1  | -  |     | _alias_ |
| 1001 0100 1010 1000  | CLN  | -  | Clear Negative Flag  | N←0  | N  | 1  | -  |     | _alias_ |
| 1001 0100 0001 1000  | SEZ  | -  | Set Zero Flag  | Z←1  | Z  | 1  | -  |     | _alias_ |
| 1001 0100 1001 1000  | CLZ  | -  | Clear Zero Flag  | Z←0  | Z  | 1  | -  |     | _alias_ |
| 1001 0100 0111 1000  | SEI  | -  | Global Interrupt Enable  | I←1  | I  | 1  | -  |     | _alias_ |
| 1001 0100 1111 1000  | CLI  | -  | Global Interrupt Disable  | I←0  | I  | 1  | -  |     | _alias_ |
| 1001 0100 0100 1000  | SES  | -  | Set Signed Test Flag  | S←1  | S  | 1  | -  |     | _alias_ |
| 1001 0100 1100 1000  | CLS  | -  | Clear Signed Test Flag  | S←0  | S  | 1  | -  |     | _alias_ |
| 1001 0100 0011 1000  | SEV  | -  | Set Two's Complement Overflow  | V←1  | V  | 1  | -  |     | _alias_ |
| 1001 0100 1011 1000  | CLV  | -  | Clear Two's Complement Overflow  | V←0  | V  | 1  | -  |     | _alias_ |
| 1001 0100 0110 1000  | SET  | -  | Set T in SREG  | T←1  | T  | 1  | -  |     | _alias_ |
| 1001 0100 1110 1000  | CLT  | -  | Clear T in SREG  | T←0  | T  | 1  | -  |     | _alias_ |
| 1001 0100 0101 1000  | SEH  | -  | Set Half Carry Flag in SREG  | H←1  | H  | 1  | -  |     | _alias_ |
| 1001 0100 1101 1000  | CLH  | -  | Clear Half Carry Flag in SREG  | H←0  | H  | 1  | -  |     | _alias_ |
| **MCU Control Instructions**  ||||||||||
| 1001 0101 1001 1000  | BREAK  | -  | Break  | (See specific descr. for BREAK)  | None  | 1  | -  | (1)  | |
| 0000 0000 0000 0000  | NOP  | -  | No Operation  | -  | None  | 1  | -  |     |     |
| 1001 0101 1000 1000  | SLEEP  | -  | Sleep  | (see specific descr. for Sleep)  | None  | 1  | -  |     |     |
| 1001 0101 1010 1000  | WDR  | -  | Watchdog Reset  | (see specific descr. for WDR)  | None  | 1  | -  |     |     |
| **unallocated codes**  ||||||||||
| 0000 0000 xxxx xxxx  | NOP | | | | | | | (7) |  x!=0 |
| 1001 000x xxxx 0011  | NOP | | | | | | | (7) |   |
| 1001 000x xxxx 1000  | NOP | | | | | | | (7) |   |
| 1001 000x xxxx 1011  | NOP | | | | | | | (7) |   |
| 1001 001x xxxx 0011  | NOP | | | | | | | (7) |   |
| 1001 001x xxxx 1000  | NOP | | | | | | | (7) |   |
| 1001 001x xxxx 1011  | NOP | | | | | | | (7) |   |
| 1001 0101 xxxx 1001  | (E)ICALL | | | | | | | (7) |   x!={0,1}    |
| 1001 0101 0xxx 1000  | (I)RET   | | | | | | | (7) |   x!={0,1}    |
| 1001 0101 1011 1000  | NOP | | | | | | | (7) |   |
| 1001 010x xxxx 0100  | NOP | | | | | | | (7) |   |
| 1001 0101 xxxx 1011  | NOP | | | | | | | (7) |   |
| 1001 0100 xxxx 1001  | (E)IJMP   | | | | | | | (7) |  x!={0,1}    |
| 1111 100x xxxx 1xxx  | BLD Rd,b  | | | | | | | (7) |              |
| 1111 101x xxxx 1xxx  | BST Rd,b  | | | | | | | (7) |              |
| 1111 110x xxxx 1xxx  | SBRC Rr,b | | | | | | | (7) |              |
| 1111 111x xxxx 1xxx  | SBRS Rr,b | | | | | | | (7) |              |


1. This instruction is not available in all devices. Refer to the device specific instruction set summary.
2. Not all variants of this instruction are available in all devices. Refer to the device specific instruction set summary.
3. Cycle times for Data memory accesses assume internal memory accesses, and are not valid for accesses via the external RAM interface.
4. One extra cycle must be added when accessing Internal SRAM.
5. Number of clock cycles for Reduced Core tinyAVR®.

6. SPM should be called ESPM, since it addresses through RAMPZ
7. Some of the unallocated codes are actually aliases for existing instructions.



