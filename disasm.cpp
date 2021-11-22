#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <unordered_map>
#include <stack>
using namespace std;
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
/*
* var <name>=<number>;
* <function>(arg1, arg2, argn);    function call
* if (<var>==<number>) block end
*/
char byte, bytex, bytexx;
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
    void i_leq(char res, char left, char right) {
        byte = INSTR_LEQ;
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
void disasm(const char* file_name) {
    FILE* file=fopen(file, "rb");
    while (true) {
        fread(&byte, 1, 1, file);
        switch (byte) {
        case INSTR_PRINT_REG:
            fread(&byte, 1, 1, file);
            cout << "PRINT REG " << int(byte);
            break;
        case INSTR_MOV:
            fread(&byte, 1, 1, file);
            fread(&bytex, 1, 1, file);
            cout << "MOV " << int(byte) << ' ' << bytex << '\n';
            break;
        case INSTR_EOF:
            fclose(file);
            return;
        }
    }
}
int main()
{

	return 0;
}
