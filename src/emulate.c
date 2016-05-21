#include <stdlib.h>
#include <stdint.h>
#include "emulator/reader.h"
#include "emulator/pipeline.h"
#include "emulator/printer.h"
#include "emulator/arm11.h"

#define MEMORY_SIZE (1 << 16)
#define NUM_OF_REGISTERS 17

int main(int argc, char **argv) {

    initialize();

    bin_to_mem(argv);

    emulate();

    print_registers();

    return EXIT_SUCCESS;
}
