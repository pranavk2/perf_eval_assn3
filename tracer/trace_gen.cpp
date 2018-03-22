#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>

#define NUM_INSTR_DESTINATIONS 2
#define NUM_INSTR_SOURCES 4

using namespace std;

typedef struct trace_instr_format {
    unsigned long long int ip;  // instruction pointer (program counter) value

    unsigned char is_branch;    // is this branch
    unsigned char branch_taken; // if so, is this taken

    unsigned char destination_registers[NUM_INSTR_DESTINATIONS]; // output registers
    unsigned char source_registers[NUM_INSTR_SOURCES];           // input registers

    unsigned long long int destination_memory[NUM_INSTR_DESTINATIONS]; // output memory
    unsigned long long int source_memory[NUM_INSTR_SOURCES];           // input memory
} trace_instr_format_t;

/* ================================================================== */
// Global variables 
/* ================================================================== */


trace_instr_format_t curr_instr;

int main(int argc, char *argv[])
{
    unsigned long long int paddr;
    int rw;
    ifstream f_addr(argv[1]);
    FILE* f_out = fopen(argv[2], "ab");;

    curr_instr.ip = 0xFEEDBEE;
    curr_instr.is_branch = 0;
    curr_instr.branch_taken = 0;
    for(int i=0; i<NUM_INSTR_DESTINATIONS; i++) 
    {
        curr_instr.destination_registers[i] = 0;
        curr_instr.destination_memory[i] = 0;
    }
    for(int i=0; i<NUM_INSTR_SOURCES; i++) 
    {
        curr_instr.source_registers[i] = 1;
        curr_instr.source_memory[i] = 0;
    }
    int inst_cnt = 0;

    while(f_addr >> rw >> paddr){
    
        curr_instr.ip += inst_cnt * 4;
        if(rw == 0) {
            curr_instr.source_memory[0] = paddr;
        }
        else {
            curr_instr.destination_memory[0] = paddr;
        }

        fwrite(&curr_instr, sizeof(trace_instr_format_t), 1, f_out);
        cout << curr_instr.destination_memory[0] << " " << curr_instr.source_memory[0] << endl;
        curr_instr.destination_memory[0] = 0;
        curr_instr.source_memory[0] = 0;

    }                                       
    return 0;
}
