#include <iostream>
#include <stdio.h>
using namespace std;
#define STACK_SIZE 4
typedef char BYTE;
enum INSTR_TYPE {
    INSTR_EOF,
    INSTR_MOV,
    INSTR_PRINT_REG,
    INSTR_MOVQ,
    INSTR_ADD,
    INSTR_SUB,
    INSTR_MUL,
    INSTR_EQ,
    INSTR_NEQ,
    INSTR_LE,//TEST!!
    INSTR_GR,//TODO
    INSTR_LEQ,//TODO
    INSTR_GRQ,//TODO
    INSTR_JMP_IF,
    INSTR_JMP,
    INSTR_LOAD32,
    INSTR_STORE32,
};
enum REG {
    EA0 = 0,
    EA1,
    EA2,
    EA3,
    EA4,
    EA5,
    EA6,
    EA7,
    COND,
};
char stk[STACK_SIZE];
char byte, bytex, bytexx;
int32_t byte4;
uint32_t ubyte4;
int tick = 0;
void* memory=stk;
int32_t reg_ea[9]{ 0 };
void debug_reg() {
    cout << "Tick " << tick << '\n';
    for (int i = 0; i < 8; i++) {
        cout << "Reg " << i << ":" << reg_ea[i] << '\n';
    }
}
inline void execute_bytecode_unsafe(const char* file_name) {
    FILE* file = fopen(file_name, "rb");
    while (true) {
        fread(&byte, 1, 1, file);
        if (byte == INSTR_STORE32) {
            fread(&byte4, 4, 1, file);
            fread(&byte, 1, 1, file);
                //                      ----VISUAL STUDIO-----
            *static_cast<int*>((int32_t*)memory + byte4) = reg_ea[byte];
        }
        else if (byte == INSTR_LOAD32) {
            fread(&byte, 1, 1, file);
            fread(&byte4, 4, 1, file);
                //                            ----VISUAL STUDIO-----
            reg_ea[byte] = *static_cast<int*>((int32_t*)memory + byte4);
        }
        else if (byte == INSTR_MOV) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            reg_ea[byte] = reg_ea[bytex];
        }
        else if (byte==INSTR_MOVQ) {
            fread(&byte, 1, 1, file);
            fread(&byte4, 4, 1, file);
            reg_ea[byte] = byte4;
        }
        else if (byte == INSTR_PRINT_REG) {
            fread(&byte, 1, 1, file);
            cout << reg_ea[byte] << '\n';
        }
        else if (byte == INSTR_SUB) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = reg_ea[bytex] - reg_ea[bytexx];
        }
        else if (byte == INSTR_MUL) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = reg_ea[bytex] * reg_ea[bytexx];
        }
        else if (byte == INSTR_ADD) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = reg_ea[bytex] + reg_ea[bytexx];
        }
        else if (byte == INSTR_JMP_IF) {
            fread(&byte, 1, 1, file);
            fread(&byte4, 4, 1, file);
            if (reg_ea[byte]) fseek(file, byte4, SEEK_SET);
        }
        else if (byte == INSTR_EQ) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] == reg_ea[bytexx]);
        }
        else if (byte == INSTR_LEQ) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] <= reg_ea[bytexx]);
        }
        else if (byte == INSTR_LE) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] < reg_ea[bytexx]);
        }
        else if (byte == INSTR_JMP) {
            fread(&byte4, 4, 1, file);
            fseek(file, byte4, SEEK_SET);
        }
        else if (byte==INSTR_NEQ) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] != reg_ea[bytexx]);
        }
        else if (byte == INSTR_GRQ) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] >= reg_ea[bytexx]);
        }
        else if (byte == INSTR_GR) {
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            fread(&bytexx, 1, 1, file);
            reg_ea[byte] = (reg_ea[bytex] > reg_ea[bytexx]);
        }
        else if (byte==INSTR_EOF) return;
        else {
            cout << "Not implemented error! " << int(byte);
            return;
        }
    }
    fclose(file);
}
int main()
{
    execute_bytecode_unsafe("out.sb");
    return 0;
}
