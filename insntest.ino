// Arduino project for investigating unallocated AVR opcodes.
// Since there is only a limited space available in an avr chip, you have to comment/uncomment sections
// of the code manually to try all parts.

#define DECLARE_OPCODE_FUNCTION(code, description) \
    static const char *descopcode ## code = "code " #code " " description;   \
    static uint16_t opcode ## code (uint8_t arg0, uint8_t arg1) {  \
        register uint8_t r0 asm("r18") = arg0;  \
        register uint8_t r1 asm("r19") = arg1;  \
        asm volatile (    \
            "movw r0, r18\n"           \
            "set\n" \
            ".hword 0x" # code "\n"    \
            "movw r18,r0\n"            \
            "clr r1\n"                 \
            : "=r" (r0), "=r" (r1)     \
            : "r" (r0), "r" (r1)       \
            );                         \
        return r0+(r1<<8);             \
    }
#define EXECOPCODE(code) testopcode(descopcode##code, opcode##code)



DECLARE_OPCODE_FUNCTION(0000, "nop")
DECLARE_OPCODE_FUNCTION(0008, "invalid nop")
DECLARE_OPCODE_FUNCTION(0080, "invalid nop")
DECLARE_OPCODE_FUNCTION(8000, "ld r0, Z")
DECLARE_OPCODE_FUNCTION(9403, "inc r0")
DECLARE_OPCODE_FUNCTION(940a, "dec r0")
DECLARE_OPCODE_FUNCTION(9413, "inc r1")
DECLARE_OPCODE_FUNCTION(941a, "dec r1")
DECLARE_OPCODE_FUNCTION(95b8, "wdr+1")

DECLARE_OPCODE_FUNCTION(9003, "1001 000x xxxx 0011")
DECLARE_OPCODE_FUNCTION(9013, "1001 000x xxxx 0011")
DECLARE_OPCODE_FUNCTION(9008, "1001 000x xxxx 1000")
DECLARE_OPCODE_FUNCTION(9018, "1001 000x xxxx 1000")
DECLARE_OPCODE_FUNCTION(900b, "1001 000x xxxx 1011")
DECLARE_OPCODE_FUNCTION(901b, "1001 000x xxxx 1011")
DECLARE_OPCODE_FUNCTION(9203, "1001 001x xxxx 0011")
DECLARE_OPCODE_FUNCTION(9213, "1001 001x xxxx 0011")
DECLARE_OPCODE_FUNCTION(9208, "1001 001x xxxx 1000")
DECLARE_OPCODE_FUNCTION(9218, "1001 001x xxxx 1000")
DECLARE_OPCODE_FUNCTION(920b, "1001 001x xxxx 1011")
DECLARE_OPCODE_FUNCTION(921b, "1001 001x xxxx 1011")
/*
DECLARE_OPCODE_FUNCTION(9529, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9539, "1001 0101 xxxx 1001  x!={0,1}")

DECLARE_OPCODE_FUNCTION(9549, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9559, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9569, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9579, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9589, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9599, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95a9, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95b9, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95c9, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95d9, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95e9, "1001 0101 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(95f9, "1001 0101 xxxx 1001  x!={0,1}")

DECLARE_OPCODE_FUNCTION(9528, "1001 0101 0xxx 1000  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9538, "1001 0101 0xxx 1000  x!={0,1}")

DECLARE_OPCODE_FUNCTION(9548, "1001 0101 0xxx 1000  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9558, "1001 0101 0xxx 1000  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9568, "1001 0101 0xxx 1000  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9578, "1001 0101 0xxx 1000  x!={0,1}")
*/
DECLARE_OPCODE_FUNCTION(9404, "1001 010x xxxx 0100")
DECLARE_OPCODE_FUNCTION(9414, "1001 010x xxxx 0100")
DECLARE_OPCODE_FUNCTION(950b, "1001 0101 xxxx 1011")
DECLARE_OPCODE_FUNCTION(951b, "1001 0101 xxxx 1011")
/*
DECLARE_OPCODE_FUNCTION(9429, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9439, "1001 0100 xxxx 1001  x!={0,1}")

DECLARE_OPCODE_FUNCTION(9449, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9459, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9469, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9479, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9489, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(9499, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94a9, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94b9, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94c9, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94d9, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94e9, "1001 0100 xxxx 1001  x!={0,1}")
DECLARE_OPCODE_FUNCTION(94f9, "1001 0100 xxxx 1001  x!={0,1}")
*/
DECLARE_OPCODE_FUNCTION(f808, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f809, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f80a, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f80b, "1111 100x xxxx 1xxx")

DECLARE_OPCODE_FUNCTION(f818, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f819, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f81a, "1111 100x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(f81b, "1111 100x xxxx 1xxx")

DECLARE_OPCODE_FUNCTION(fa08, "1111 101x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(fc08, "1111 110x xxxx 1xxx")
DECLARE_OPCODE_FUNCTION(fe08, "1111 111x xxxx 1xxx")



void testopcode(const char*tag, uint16_t (*fn)(uint8_t r0, uint8_t r1))
{
  Serial.print(tag);                Serial.print(" -> ");
  delay(200);
  Serial.print((int)fn(0,   0));    Serial.print(" : ");
  delay(100);
  Serial.print((int)fn(0x55,0));    Serial.print(" : ");
  delay(100);
  Serial.print((int)fn(0x55,0x55)); Serial.print(" : ");
  delay(100);
  Serial.print((int)fn(0,   0x55)); Serial.println();
  delay(200);
}

#define DECLARE_RET_FUNCTION(code, description) \
    static const char *descret ## code = "ret " #code " " description;   \
    uint16_t testret ## code ()              \
    {                                       \
        register uint8_t r0 asm("r18") = 0; \
        asm volatile (                 \
            "brie 4f\n"                \
            "ori r18, 8  ; I==0\n"     \
            "4:\n"                     \
            "cli\n"                    \
            "rcall 1f\n"               \
            "ori r18, 1  ; returned\n" \
            "rjmp 2f\n"                \
            "1:\n"                     \
            ".hword 0x" #code "\n"     \
            "ori r18, 2  ; not a ret insn\n"                  \
            "sbrc r18, 1\n"            \
            "ret\n"                    \
            "2:\n"                     \
            "brie 3f\n"                \
            "ori r18, 4  ; I==0\n"     \
            "3:\n"                     \
            "ori r18, 16\n"            \
            : "=r" (r0)                \
            : "r" (r0)                 \
            );                         \
        return r0;                     \
    }
#define EXECRET(code) testret(descret##code, testret##code)



void testret(const char*tag, uint16_t (*fn)())
{
  Serial.print(tag);                Serial.print(" -> ");
  delay(200);
  Serial.println(fn());
}
DECLARE_RET_FUNCTION(0000, "nop")
DECLARE_RET_FUNCTION(9508, "ret")
DECLARE_RET_FUNCTION(9518, "reti")
DECLARE_RET_FUNCTION(9528, "?")
DECLARE_RET_FUNCTION(9538, "?")
DECLARE_RET_FUNCTION(9548, "?")
DECLARE_RET_FUNCTION(9558, "?")
DECLARE_RET_FUNCTION(9568, "?")
DECLARE_RET_FUNCTION(9578, "?")



#define DECLARE_JMP_FUNCTION(code, description) \
    static const char *descjmp ## code = "jmp " #code " " description;   \
    uint16_t testjmp ## code ()              \
    {                                       \
        register uint8_t r0 asm("r18") = 0; \
        asm volatile (                    \
            "call 1f\n"                   \
            "1:\n"                        \
            "pop r31   ; Z = PC\n"        \
            "pop r30\n"                   \
            "adiw r30, 6  ; pc+6 = label 2\n"              \
            ".hword 0x" #code " ; attempt to jump to 2\n"  \
            "ori r18,1   ; jump did not happen\n"          \
            "jmp 3f\n"                   \
            "2:\n"                       \
            "ori r18,2   ; we jumped\n"  \
            "3:\n"                       \
            "ori r18,4   ; always\n"     \
            "\n"                         \
            : "=r" (r0)                  \
            : "r" (r0)                   \
            );                           \
        return r0;                       \
    }
#define EXECJMP(code) testjmp(descjmp##code, testjmp##code)



void testjmp(const char*tag, uint16_t (*fn)())
{
  Serial.print(tag);                Serial.print(" -> ");
  delay(200);
  Serial.println(fn());
}
/*
1001 0100 0001 1001  EIJMP Z
1001 0100 0000 1001  IJMP Z
1001 010k kkkk 110k kkkk kkkk kkkk kkkk  JMP k
1100 kkkk kkkk kkkk  RJMP k

1001 0101 0001 1001  EICALL Z
1001 0101 0000 1001  ICALL Z
1001 010k kkkk 111k kkkk kkkk kkkk kkkk  CALL k
1101 kkkk kkkk kkkk  RCALL k
*/
/*
DECLARE_JMP_FUNCTION(0000, "nop")
DECLARE_JMP_FUNCTION(9409, "ijmp")
DECLARE_JMP_FUNCTION(9419, "eijmp")
DECLARE_JMP_FUNCTION(c000, "jmp+0")
DECLARE_JMP_FUNCTION(c001, "jmp+1")
DECLARE_JMP_FUNCTION(c002, "jmp+2")
DECLARE_JMP_FUNCTION(9429, "?")
DECLARE_JMP_FUNCTION(9439, "?")
DECLARE_JMP_FUNCTION(9449, "?")
DECLARE_JMP_FUNCTION(9459, "?")
DECLARE_JMP_FUNCTION(9469, "?")
DECLARE_JMP_FUNCTION(9479, "?")
DECLARE_JMP_FUNCTION(9489, "?")
DECLARE_JMP_FUNCTION(9499, "?")
DECLARE_JMP_FUNCTION(94a9, "?")
DECLARE_JMP_FUNCTION(94b9, "?")
DECLARE_JMP_FUNCTION(94c9, "?")
DECLARE_JMP_FUNCTION(94d9, "?")
DECLARE_JMP_FUNCTION(94e9, "?")
DECLARE_JMP_FUNCTION(94f9, "?")
*/

#define DECLARE_CALL_FUNCTION(code, description) \
    static const char *desccall ## code = "call " #code " " description;   \
    uint16_t testcall ## code ()              \
    {                                       \
        register uint8_t r0 asm("r18") = 0; \
        asm volatile (                    \
            "call 1f\n"                   \
            "1:\n"                        \
            "pop r31   ; Z = PC\n"        \
            "pop r30\n"                   \
            "adiw r30, 6  ; pc+6 = label 2\n"              \
            ".hword 0x" #code " ; attempt to call to 2\n"  \
            "ori r18,1   ; call did not happen\n"          \
            "rjmp 3f\n"                   \
            "2:\n"                        \
            "ori r18,2   ; called\n"      \
            "pop r0      ; pop retaddr\n" \
            "pop r0\n"                   \
            "3:\n"                       \
            "ori r18,4   ; always\n"     \
            "\n"                         \
            : "=r" (r0)                  \
            : "r" (r0)                   \
            );                           \
        return r0;                       \
    }
#define EXECCALL(code) testcall(desccall##code, testcall##code)



void testcall(const char*tag, uint16_t (*fn)())
{
  Serial.print(tag);                Serial.print(" -> ");
  delay(200);
  Serial.println(fn());
}
/*
1001 0101 0001 1001  EICALL Z
1001 0101 0000 1001  ICALL Z
1001 010k kkkk 111k kkkk kkkk kkkk kkkk  CALL k
1101 kkkk kkkk kkkk  RCALL k
*/
DECLARE_CALL_FUNCTION(0000, "nop")
DECLARE_CALL_FUNCTION(9509, "icall")
DECLARE_CALL_FUNCTION(9519, "eicall")
DECLARE_CALL_FUNCTION(d000, "call+0")
DECLARE_CALL_FUNCTION(d001, "call+1")
DECLARE_CALL_FUNCTION(d002, "call+2")

DECLARE_CALL_FUNCTION(9529, "?")
DECLARE_CALL_FUNCTION(9539, "?")
DECLARE_CALL_FUNCTION(9549, "?")
DECLARE_CALL_FUNCTION(9559, "?")
DECLARE_CALL_FUNCTION(9569, "?")
DECLARE_CALL_FUNCTION(9579, "?")
DECLARE_CALL_FUNCTION(9589, "?")
DECLARE_CALL_FUNCTION(9599, "?")
DECLARE_CALL_FUNCTION(95a9, "?")
DECLARE_CALL_FUNCTION(95b9, "?")
DECLARE_CALL_FUNCTION(95c9, "?")
DECLARE_CALL_FUNCTION(95d9, "?")
DECLARE_CALL_FUNCTION(95e9, "?")
DECLARE_CALL_FUNCTION(95f9, "?")



void voidfn() { }

#define PART 1
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EXECOPCODE(0000);
/*
  EXECOPCODE(f808);
  EXECOPCODE(f809);
  EXECOPCODE(f80a);
  EXECOPCODE(f80b);

  EXECOPCODE(f818);
  EXECOPCODE(f819);
  EXECOPCODE(f81a);
  EXECOPCODE(f81b);
*/
EXECCALL(0000);
EXECCALL(9509);
EXECCALL(9519);
EXECCALL(d000);
EXECCALL(d001);
EXECCALL(d002);

EXECCALL(9529);
EXECCALL(9539);
EXECCALL(9549);
EXECCALL(9559);
EXECCALL(9569);
EXECCALL(9579);
EXECCALL(9589);
EXECCALL(9599);
EXECCALL(95a9);
EXECCALL(95b9);
EXECCALL(95c9);
EXECCALL(95d9);
EXECCALL(95e9);
EXECCALL(95f9);
/*
EXECJMP(0000);
EXECJMP(9409);
EXECJMP(9419);
EXECJMP(c000);
EXECJMP(c001);
EXECJMP(c002);
EXECJMP(9429);
EXECJMP(9439);
EXECJMP(9449);
EXECJMP(9459);
EXECJMP(9469);
EXECJMP(9479);
EXECJMP(9489);
EXECJMP(9499);
EXECJMP(94a9);
EXECJMP(94b9);
EXECJMP(94c9);
EXECJMP(94d9);
EXECJMP(94e9);
EXECJMP(94f9);
*/
  EXECRET(9508);
  EXECRET(9518);
  EXECRET(9528);
  EXECRET(9538);
  EXECRET(9548);
  EXECRET(9558);
  EXECRET(9568);
  EXECRET(9578);
  //EXECRET(0000);
  /*  
  EXECOPCODE(9403);
  EXECOPCODE(940a);
  EXECOPCODE(9413);
  EXECOPCODE(941a);
  EXECOPCODE(0008);
  EXECOPCODE(8000);
  EXECOPCODE(95b8);
  */

  
Serial.println("========= done ==========");
}

void loop() {
  // put your main code here, to run repeatedly:

}
