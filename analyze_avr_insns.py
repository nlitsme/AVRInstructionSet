# script which analyzes the AVR instructionset, finding unallocated codes, and marking aliases.
# aliases are marked by a list of semicolon separated instructions.
# unallocated codes don't have an instruction.

# the list as it is below already has all aliases removed, and all unallocated instructions marked.
# showing that this is the complete list, all bitpatters are accounted for.

insnlist = """
0000 0000 0000 0000 NOP
1001 0100 0000 1001 IJMP Z
1001 0100 0001 1001 EIJMP Z

1001 0101 0000 1000 RET
1001 0101 0000 1001 ICALL Z
1001 0101 0001 1000 RETI
1001 0101 0001 1001 EICALL Z
1001 0101 1000 1000 SLEEP
1001 0101 1001 1000 BREAK
1001 0101 1010 1000 WDR
1001 0101 1100 1000 LPM R0, Z
1001 0101 1101 1000 ELPM R0, Z
1001 0101 1110 1000 SPM Z             R0:R1 = data
1001 0101 1111 1000 SPM Z+

0000 0011 0xxx 1xxx FMUL Rd,Rr     r,d = 16..23
0000 0011 1xxx 0xxx FMULS Rd,Rr    r,d = 16..23
0000 0011 1xxx 1xxx FMULSU Rd,Rr    r,d = 16..23
1111 100x xxxx 0xxx BLD Rd,b
1111 101x xxxx 0xxx BST Rd,b
1111 110x xxxx 0xxx SBRC Rr,b
1111 111x xxxx 0xxx SBRS Rr,b

1001 0100 0xxx 1000 BSET s
1001 0100 1xxx 1000 BCLR s            s = 0-7 = C,Z,N,V,S,H,T,I
1001 0100 xxxx 1011 DES K

0000 0001 xxxx xxxx MOV Rd,Rr
0000 0010 xxxx xxxx MULS Rd,Rr     d,r=16..31
0000 0011 0xxx 0xxx MULSU Rd,Rr     d,r=16..31

0000 01xx xxxx xxxx CPC Rd,Rr
0000 10xx xxxx xxxx SBC Rd,Rr
0000 11xx xxxx xxxx ADD Rd,Rr
0001 00xx xxxx xxxx CPSE Rd,Rr
0001 01xx xxxx xxxx CP Rd,Rr
0001 10xx xxxx xxxx SUB Rd,Rr
0001 11xx xxxx xxxx ADC Rd,Rr
0010 00xx xxxx xxxx AND Rd,Rr
0010 01xx xxxx xxxx EOR Rd,Rr
0010 10xx xxxx xxxx OR Rd,Rr
0010 11xx xxxx xxxx MOV Rd,Rr
1001 11xx xxxx xxxx MUL Rd,Rr
1111 00xx xxxx xxxx BRBS s,k
1111 01xx xxxx xxxx BRBC s,k

0011 xxxx xxxx xxxx CPI Rd,K         16<=d<=31
0100 xxxx xxxx xxxx SBCI Rd,K      d=16..31
0110 xxxx xxxx xxxx ORI Rd,K
0111 xxxx xxxx xxxx ANDI Rd,K         16<=d<=31
1100 xxxx xxxx xxxx RJMP k
1101 xxxx xxxx xxxx RCALL k
1110 xxxx xxxx xxxx LDI Rd,K          d=16..31

1001 000x xxxx 0000 kkkk kkkk kkkk kkkk   LDS  Rd,k
1001 000x xxxx 0001 LD Rd,Z+
1001 000x xxxx 0010 LD Rd,-Z
1001 000x xxxx 0100 LPM Rd, Z
1001 000x xxxx 0101 LPM Rd, Z+
1001 000x xxxx 0110 ELPM Rd, Z
1001 000x xxxx 0111 ELPM Rd, Z+
1001 000x xxxx 1001 LD Rd,Y+
1001 000x xxxx 1010 LD Rd,-Y
1001 000x xxxx 1100 LD Rd,X
1001 000x xxxx 1101 LD Rd,X+
1001 000x xxxx 1110 LD Rd,-X
1001 000x xxxx 1111 POP Rd
1001 001x xxxx 0000 kkkk kkkk kkkk kkkk  STS k,Rr
1001 001x xxxx 0001 ST Z+,Rr
1001 001x xxxx 0010 ST -Z,Rr
1001 001x xxxx 0100 XCH Z,Rd
1001 001x xxxx 0101 LAS Z,Rd
1001 001x xxxx 0110 LAC Z,Rd
1001 001x xxxx 0111 LAT Z,Rd
1001 001x xxxx 1001 ST Y+,Rr
1001 001x xxxx 1010 ST -Y,Rr
1001 001x xxxx 1100 ST X,Rr
1001 001x xxxx 1101 ST X+,Rr
1001 001x xxxx 1110 ST -X,Rr
1001 001x xxxx 1111 PUSH Rd
1001 010x xxxx 0000 COM Rd
1001 010x xxxx 0001 NEG Rd
1001 010x xxxx 0010 SWAP Rd
1001 010x xxxx 0011 INC Rd
1001 010x xxxx 0101 ASR Rd
1001 010x xxxx 0110 LSR Rd
1001 010x xxxx 0111 ROR Rd
1001 010x xxxx 1010 DEC Rd
1001 010x xxxx 110x kkkk kkkk kkkk kkkk  JMP k
1001 010x xxxx 111x kkkk kkkk kkkk kkkk  CALL k

1001 0110 xxxx xxxx ADIW Rd+1:Rd,K    d in {24,26,28,30}
1001 0111 xxxx xxxx SBIW Rd+1:Rd,K    d in {24,26,28,30}
1001 1000 xxxx xxxx CBI A,b
1001 1001 xxxx xxxx SBIC A,b
1001 1010 xxxx xxxx SBI A,b
1001 1011 xxxx xxxx SBIS A,b

1011 0xxx xxxx xxxx IN Rd,A
1011 1xxx xxxx xxxx OUT A,Rr
0101 xxxx xxxx xxxx SUBI Rd,Rr

0000 0000 xxxx xxxx ???00xx  x!=0
1001 000x xxxx 0011 ???90x3
1001 000x xxxx 1000 ???90x8
1001 000x xxxx 1011 ???90xb
1001 001x xxxx 0011 ???92x3
1001 001x xxxx 1000 ???92x8
1001 001x xxxx 1011 ???92xb
1001 0101 1xxx 1001 ???9589
1001 0101 01xx 1001 ???9549
1001 0101 001x 1001 ???9529
1001 0101 01xx 1000 ???9548
1001 0101 001x 1000 ???9528
1001 0101 1011 1000 ???95b8
1001 010x xxxx 0100 ???94x4
1001 0101 xxxx 1011 ???95xb
1001 0100 1xxx 1001 ???9489
1001 0100 01xx 1001 ???9449
1001 0100 001x 1001 ???9429
1111 100x xxxx 1xxx ???f8xx
1111 101x xxxx 1xxx ???faxx
1111 110x xxxx 1xxx ???fcxx
1111 111x xxxx 1xxx ???fexx

"""

attiny = """
1010 0xxx xxxx xxxx LDS Rd,k
1010 1xxx xxxx xxxx STS k,Rd       d=16..31
1000 xxxx xxxx xxxx ????
"""
atmega = """
10x0 xx0x xxxx 0xxx LDD Rd,Z+q
10x0 xx0x xxxx 1xxx LDD Rd,Y+q
10x0 xx1x xxxx 0xxx STD Z+q,Rr
10x0 xx1x xxxx 1xxx STD Y+q,Rr
"""

def decodebits(bits):
    m = 0
    v = 0
    for c in bits:
        if c in "01x":
            m *= 2
            v *= 2
            if c in "1":
                v += 1
            if c in "01":
                m += 1
    return m, v
def decodeinsn(i, insns):
    found = []
    for m, v, insn in insns:
        if (i&m)==v:
            found.append(insn)
    return found
insns = []
for line in (insnlist+atmega).strip("\n").split("\n"):
    if line:
        bits, insn = line[:19], line[20:]
        mask, value = decodebits(bits)
        insns.append((mask, value, insn))

for i in range(0x10000):
    insn = decodeinsn(i, insns)
    if not insn:
         print "%04x" % i
    else:
         print "%04x %s" % (i, " ; ".join(insn))

