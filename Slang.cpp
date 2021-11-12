
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
class ByteCode {
    char byte;
    int bytes = 0;
public:
    FILE* file;
    void i_store32(int32_t mem, int32_t reg) {
        byte = INSTR_STORE32;
        fwrite(&byte, 1, 1, file);
        fwrite(&mem, 4, 1, file);
        fwrite(&reg, 1, 1, file);
        bytes += 6;
    }
    void i_load32(char reg, int32_t mem) {
        byte = INSTR_LOAD32;
        fwrite(&byte, 1, 1, file);
        fwrite(&reg, 1, 1, file);
        fwrite(&mem, 4, 1, file);
        bytes += 6;
    }
    void i_neq(char res, char left, char right) {
        byte = INSTR_NEQ;
        fwrite(&byte, 1, 1, file);
        fwrite(&res, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 4;
    }
    void i_eq(char res, char left, char right) {
        byte = INSTR_EQ;
        fwrite(&byte, 1, 1, file);
        fwrite(&res, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 4;
    }
    int label() { return bytes; }
    void change_jmp(int32_t from, int32_t where) {
        mov(from);
        byte = INSTR_JMP;
        fwrite(&byte, 1, 1, file);
        fwrite(&where, 4, 1, file);
    }
    void i_jmp(int32_t offset) {
        byte = INSTR_JMP;
        fwrite(&byte, 1, 1, file);
        fwrite(&offset, 4, 1, file);
        bytes += 5;
    }
    void i_jmp_if(char cond, int32_t offset) {
        byte = INSTR_JMP_IF;
        fwrite(&byte, 1, 1, file);
        fwrite(&cond, 1, 1, file);
        fwrite(&offset, 4, 1, file);
        bytes += 6;
    }
    void i_mov(char left, char right) {
        byte = INSTR_MOV;
        fwrite(&byte, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 3;
    }
    void i_movq(char left, int32_t right) {
        byte = INSTR_MOVQ;
        fwrite(&byte, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 4, 1, file);
        bytes += 6;
    }
    void i_print_reg(char reg) {
        byte = INSTR_PRINT_REG;
        fwrite(&byte, 1, 1, file);
        fwrite(&reg, 1, 1, file);
        bytes += 2;
    }
    void i_sub(char res, char left, char right) {
        byte = INSTR_SUB;
        fwrite(&byte, 1, 1, file);
        fwrite(&res, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 4;
    }
    void i_mul(char res, char left, char right) {
        byte = INSTR_MUL;
        fwrite(&byte, 1, 1, file);
        fwrite(&res, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 4;
    }
    void i_add(char res, char left, char right) {
        byte = INSTR_ADD;
        fwrite(&byte, 1, 1, file);
        fwrite(&res, 1, 1, file);
        fwrite(&left, 1, 1, file);
        fwrite(&right, 1, 1, file);
        bytes += 4;
    }
    void mov(int32_t pos) { fseek(file, pos, SEEK_SET); }
    void close() {
        if (file) {
            byte = INSTR_EOF;
            fwrite(&byte, 1, 1, file);
            fclose(file); file = NULL;
        }
    }
    ByteCode(const char* file_name) { file = fopen(file_name, "wb"); }
    ~ByteCode() { close(); }
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
int main()
{
    ByteCode bc("out.sb");
    bc.i_movq(EA1, 1);
    bc.i_movq(EA2, 2);
    bc.i_movq(EA3, 3);
    bc.i_sub(EA1, EA1, EA2);
    bc.i_print_reg(EA1);
    bc.close();
    SVM vm;
    vm.execute_bytecode("out.sb");
    return 0;
}
