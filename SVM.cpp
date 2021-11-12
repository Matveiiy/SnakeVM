#include <iostream>
#include <stdio.h>
using namespace std;
#define STACK_SIZE 100
enum INSTR_TYPE {
    INSTR_EOF,
    INSTR_MOV,
    INSTR_PRINT_REG,
    INSTR_MOVQ,
    INSTR_ADD,
    INSTR_SUB,//TODO
    INSTR_MUL,//TODO
    INSTR_EQ,
    INSTR_NEQ,
    INSTR_LE,//TODO
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
};
struct SVM {
    char stk[STACK_SIZE];
private:
    char byte, bytex, bytexx;
    int32_t byte4;
    int tick = 0;
    void* mem_top;
    void* memory;
public:
    void debug_reg() {
        cout << "Tick " << tick << '\n';
        for (int i = 0; i < 8; i++) {
            cout << "Reg " << i << ":" << reg_ea[i] << '\n';
        }
    }
    int32_t reg_ea[8]{ 0 };
    void execute_bytecode(const char* file_name) {
        FILE* file = fopen(file_name, "rb");
        //debug_reg();

        while (true) {
            fread(&byte, 1, 1, file);
            //cout << int(byte) << '\n';
            switch (byte) {
            case INSTR_STORE32:
                fread(&byte4, 4, 1, file);
                fread(&byte, 1, 1, file);
                //cout << byte4 << ' ' << int(byte);
                //                            ----VISUAL STUDIO-----
                *static_cast<int*>((int32_t*)memory+ byte4) = reg_ea[byte];
                break;
            case INSTR_LOAD32:
                fread(&byte, 1, 1, file);
                fread(&byte4, 4, 1, file);
                //                            ----VISUAL STUDIO-----
                reg_ea[byte] = *static_cast<int*>((int32_t*)memory + byte4);
                break;
            case INSTR_MOV:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                reg_ea[byte] = reg_ea[bytex];
                break;
            case INSTR_MOVQ:
                fread(&byte, 1, 1, file);
                fread(&byte4, 4, 1, file);
                reg_ea[byte] = byte4;
                break;
            case INSTR_PRINT_REG:
                fread(&byte, 1, 1, file);
                cout << reg_ea[byte] << '\n';
                break;
            case INSTR_SUB:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                fread(&bytexx, 1, 1, file);
                reg_ea[byte] = reg_ea[bytex] - reg_ea[bytexx];
                break;
            case INSTR_MUL:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                fread(&bytexx, 1, 1, file);
                reg_ea[byte] = reg_ea[bytex] * reg_ea[bytexx];
                break;
            case INSTR_ADD:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                fread(&bytexx, 1, 1, file);
                reg_ea[byte] = reg_ea[bytex] + reg_ea[bytexx];
                break;
            case INSTR_JMP_IF:
                fread(&byte, 1, 1, file);
                fread(&byte4, 4, 1, file);
                if (reg_ea[byte]) fseek(file, byte4, SEEK_SET);
                break;
            case INSTR_EQ:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                fread(&bytexx, 1, 1, file);
                reg_ea[byte] = (reg_ea[bytex] == reg_ea[bytexx]);
                break;
            case INSTR_JMP:
                fread(&byte4, 4, 1, file);
                fseek(file, byte4, SEEK_SET);
                break;
            case INSTR_NEQ:
                fread(&byte, 1, 1, file);
                fread(&bytex, 1, 1, file);
                fread(&bytexx, 1, 1, file);
                reg_ea[byte] = (reg_ea[bytex] != reg_ea[bytexx]);
                break;
            case INSTR_EOF: return;
            default:
                cout << "Not implemented error! " << int(byte);
                return;
            }
            ++tick;

            //debug_reg();
            if (tick >= 100)  return;
        }
        fclose(file);
    }
    SVM() {
        memory = stk;
        mem_top = memory;
    }
    ~SVM() {}
};
inline void usage() {
    printf("Usage: [OPTIONS] FILE.sb\n");
}
int main(int argc, char** argv)
{
    if (argc <= 1) { usage(); return 0; }
    SVM vm;
    vm.execute_bytecode(argv[1]);
    return 0;
}
