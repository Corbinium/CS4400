/*
  Author: Daniel Kopta
  CS 4400, University of Utah

  * Simulator handout
  * A simple x86-like processor simulator.
  * Read in a binary file that encodes instructions to execute.
  * Simulate a processor by executing instructions one at a time and appropriately 
  * updating register and memory contents.

  * Some code and pseudo code has been provided as a starting point.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "instruction.h"
#include <string.h>

// Forward declarations for helper functions
unsigned int get_file_size(int file_descriptor);
unsigned int* load_file(int file_descriptor, unsigned int size);
instruction_t* decode_instructions(unsigned int* bytes, unsigned int num_instructions);
unsigned int execute_instruction(unsigned int program_counter, instruction_t* instructions, 
				 unsigned int* registers, unsigned char* memory);
void print_instructions(instruction_t* instructions, unsigned int num_instructions);
void error_exit(const char* message);

// 17 registers
#define NUM_REGS 17
// 1024-byte stack
#define STACK_SIZE 1024

int main(int argc, char** argv)
{
  // Make sure we have enough arguments
  if(argc < 2)
    error_exit("must provide an argument specifying a binary file to execute");

  // Open the binary file
  int file_descriptor = open(argv[1], O_RDONLY);
  if (file_descriptor == -1) 
    error_exit("unable to open input file");

  // Get the size of the file
  unsigned int file_size = get_file_size(file_descriptor);
  // Make sure the file size is a multiple of 4 bytes
  // since machine code instructions are 4 bytes each
  if(file_size % 4 != 0)
    error_exit("invalid input file");

  // Load the file into memory
  // We use an unsigned int array to represent the raw bytes
  // We could use any 4-byte integer type
  unsigned int* instruction_bytes = load_file(file_descriptor, file_size);
  close(file_descriptor);

  unsigned int num_instructions = file_size / 4;


  /****************************************/
  /**** Begin code to modify/implement ****/
  /****************************************/

  // Allocate and decode instructions (left for you to fill in)
  instruction_t* instructions = decode_instructions(instruction_bytes, num_instructions);

  // Allocate and initialize registers
  unsigned int* registers = (unsigned int*)malloc(sizeof(unsigned int) * NUM_REGS);
  for (int i = 0; i < NUM_REGS; i++) {
    registers[i] = 0;
  }
  registers[8] = 1024; // stack pointer

  // Stack memory is byte-addressed, so it must be a 1-byte type
  unsigned char* memory = (unsigned char*)malloc(STACK_SIZE);
  for (int i = 0; i < STACK_SIZE; i++) {
    memory[i] = 0;
  }

  // Run the simulation
  unsigned int program_counter = 0;

  // program_counter is a byte address, so we must multiply num_instructions by 4 to get the address past the last instruction
  while(program_counter < num_instructions * 4)
  {
    program_counter = execute_instruction(program_counter, instructions, registers, memory);
  }

  free(instruction_bytes);
  free(memory);
  free(registers);
  free(instructions);
  
  return 0;
}



/*
 * Decodes the array of raw instruction bytes into an array of instruction_t
 * Each raw instruction is encoded as a 4-byte unsigned int
*/
instruction_t* decode_instructions(unsigned int* bytes, unsigned int num_instructions)
{
  instruction_t* retval = (instruction_t*)malloc(sizeof(instruction_t) * num_instructions);
  for (int i = 0; i < num_instructions; i++) {
    retval[i].opcode = (bytes[i] >> 27) & 0x1F;
    retval[i].first_register = (bytes[i] >> 22) & 0x1F;
    retval[i].second_register = (bytes[i] >> 17) & 0x1F;
    retval[i].immediate = bytes[i] & 0xFFFF;
  }
    
  return retval;
}


/*
 * Executes a single instruction and returns the next program counter
*/
unsigned int execute_instruction(unsigned int program_counter, instruction_t* instructions, unsigned int* registers, unsigned char* memory)
{
  unsigned int result;
  int r1;
  int r2;
  // program_counter is a byte address, but instructions are 4 bytes each
  // divide by 4 to get the index into the instructions array
  instruction_t instr = instructions[program_counter / 4];
  
  switch(instr.opcode)
  {
  case subl:
    registers[instr.first_register] = registers[instr.first_register] - (int)instr.immediate;
    return program_counter + 4;

  case addl_reg_reg:
    registers[instr.second_register] = registers[instr.first_register] + registers[instr.second_register];
    return program_counter + 4;

  case addl_imm_reg:
    registers[instr.first_register] = registers[instr.first_register] + (int)instr.immediate;
    return program_counter + 4;;

  case imull:
    registers[instr.second_register] = registers[instr.first_register] * registers[instr.second_register];
    return program_counter + 4;

  case shrl:
    registers[instr.first_register] = registers[instr.first_register] >> 1;
    return program_counter + 4;

  case movl_reg_reg:
    registers[instr.second_register] = registers[instr.first_register];
    return program_counter + 4;

  case movl_deref_reg:
    memcpy(&registers[instr.second_register], memory + registers[instr.first_register] + (int)instr.immediate, 4);
    return program_counter + 4;

  case movl_reg_deref:
    memcpy(memory + registers[instr.second_register] + (int)instr.immediate, &registers[instr.first_register], 4);
    return program_counter + 4;

  case movl_imm_reg:
    registers[instr.first_register] = (int)instr.immediate;
    return program_counter + 4;

  case cmpl:
    result = 0x00000000;
    r2 = registers[instr.second_register];
    r1 = registers[instr.first_register];
    // unsigned overflow, CF
    if ((unsigned int)r2 < (unsigned int)r1) {
      result = result | 0x1;
    }
    // equal values, ZF
    if (r2 == r1) {
      result = result | 0x40;
    }
    // negative result, SF
    if ((int)((unsigned int)r2 - r1) < 0) {
      result = result | 0x80;
    }
    // signed overflow, OF
    if (((long)r2 - (long)r1 > (long)0x7FFFFFFF) || ((long)r2 - (long)r1 < -(long)0x7FFFFFFF)) {
      result = result | 0x800;
    }
    registers[0] = result;
    return program_counter + 4;

  case je:
    if (registers[0] & 0x40) {
      program_counter += (int)instr.immediate;
    }
    return program_counter + 4;

  case jl:
    if (((registers[0] & 0x80) >> 7) ^ ((registers[0] & 0x800) >> 11)) {
      program_counter += (int)instr.immediate;
    }
    return program_counter + 4;

  case jle:
    if ((registers[0] & 0x40) || (((registers[0] & 0x80) >> 7) ^ ((registers[0] & 0x800) >> 11))) {
      program_counter += (int)instr.immediate;
    }
    return program_counter + 4;

  case jge:
    if (~(((registers[0] & 0x80) >> 7) ^ ((registers[0] & 0x800) >> 11)) & 0x1) {
      program_counter += (int)instr.immediate;
    }
    return program_counter + 4;

  case jbe:
    if ((registers[0] & 0x1) || (registers[0] & 0x40)) {
      program_counter += (int)instr.immediate;
    }
    return program_counter + 4;

  case jmp:
    program_counter += (int)instr.immediate;
    return program_counter + 4;

  case call:
    registers[8] -= 4;
    program_counter += 4;
    memcpy(memory + registers[8], &program_counter, 4);
    program_counter += (int)instr.immediate;
    return program_counter;

  case ret:
    if (registers[8] == 1024) {
      return 0xFFFFFFFF;
    }
    else {
      memcpy(&program_counter, memory + registers[8], 4);
      registers[8] += 4;
    }
    return program_counter;

  case pushl:
    registers[8] -= 4;
    memcpy(memory + registers[8], &registers[instr.first_register], 4);
    return program_counter + 4;

  case popl:
    memcpy(&registers[instr.first_register], memory + registers[8], 4);
    registers[8] += 4;
    break;

  case printr:
    printf("%d (0x%x)\n", registers[instr.first_register], registers[instr.first_register]);
    return program_counter + 4;

  case readr:
    scanf("%d", &(registers[instr.first_register]));
    return program_counter + 4;
  }

  return program_counter + 4;
}


/***********************************************/
/**** Begin helper functions. Do not modify ****/
/***********************************************/

/*
 * Returns the file size in bytes of the file referred to by the given descriptor
*/
unsigned int get_file_size(int file_descriptor)
{
  struct stat file_stat;
  fstat(file_descriptor, &file_stat);
  return file_stat.st_size;
}

/*
 * Loads the raw bytes of a file into an array of 4-byte units
*/
unsigned int* load_file(int file_descriptor, unsigned int size)
{
  unsigned int* raw_instruction_bytes = (unsigned int*)malloc(size);
  if(raw_instruction_bytes == NULL)
    error_exit("unable to allocate memory for instruction bytes (something went really wrong)");

  int num_read = read(file_descriptor, raw_instruction_bytes, size);

  if(num_read != size)
    error_exit("unable to read file (something went really wrong)");

  return raw_instruction_bytes;
}

/*
 * Prints the opcode, register IDs, and immediate of every instruction, 
 * assuming they have been decoded into the instructions array
*/
void print_instructions(instruction_t* instructions, unsigned int num_instructions)
{
  printf("instructions: \n");
  unsigned int i;
  for(i = 0; i < num_instructions; i++)
  {
    printf("op: %d, reg1: %d, reg2: %d, imm: %d\n", 
	   instructions[i].opcode,
	   instructions[i].first_register,
	   instructions[i].second_register,
	   instructions[i].immediate);
  }
  printf("--------------\n");
}


/*
 * Prints an error and then exits the program with status 1
*/
void error_exit(const char* message)
{
  printf("Error: %s\n", message);
  exit(1);
}
