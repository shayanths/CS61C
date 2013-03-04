#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

/* Pointer to simulator memory */
uint8_t *mem;

/* Called by program loader to initialize memory. */
uint8_t *init_mem() {
  assert (mem == NULL);
  mem = calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  return mem;
}

/* Returns 1 if memory access is ok, otherwise 0 */
int access_ok(uint32_t mipsaddr, mem_unit_t size) {
	if (mipsaddr > MEM_SIZE || mipsaddr < 1){
		return 0;
	}
	else if (mipsaddr%size != 0){
		return 0;
	}
  	return 1;
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
	int i;
	uint32_t temp = value;
	for(i = 0; i < size; i++){
		*(mem + mipsaddr + i) = temp & 0x000000FF;
		temp = temp >> 8;
	}

  /* TODO YOUR CODE HERE */

}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
	if (size == SIZE_BYTE) {
  		return *(uint8_t*)(mem + mipsaddr);
    } else if (size == SIZE_HALF_WORD) {
  		return *(uint16_t*)(mem + mipsaddr);
    } else {
  		return *(uint32_t*)(mem + mipsaddr);
    }
}
