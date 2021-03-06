#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/multiply.h"

static char spec[] = "multiply";

int tests_run = 0;

void setup() {
    initialize();
}

void tear_down() {
}

static int test_normal() {
    set_register(2, 0xfffffffd);
    set_register(5, 0x0000000f);

    union decoded_instr instr;
    instr.mul.acc      = 0;
    instr.mul.rs       = 5;
    instr.mul.rd       = 10;
    instr.mul.set_cond = 1;
    instr.mul.rm       = 2;

    mul_exec(&instr);
    uint32_t result = get_register(10);
    mu_assert(result == 0xffffffd3);
    return 0;
}

static int test_acc() {
    set_register(2, 0xfffffffd);
    set_register(5, 0x0000000f);
    set_register(9, 0x0000001f);

    union decoded_instr instr;
    instr.mul.acc      = 1;
    instr.mul.rs       = 5;
    instr.mul.rd       = 10;
    instr.mul.rn       = 9;
    instr.mul.set_cond = 1;
    instr.mul.rm       = 2;

    mul_exec(&instr);
    uint32_t result = get_register(10);
    mu_assert(result == 0xfffffff2);
    return 0;
}

static int test_cpsr() {
    set_register(2, 0xfffffffd);
    set_register(5, 0x7fffffff);
    set_register(9, 0x0000001f);

    union decoded_instr instr;
    instr.mul.acc      = 1;
    instr.mul.rs       = 5;
    instr.mul.rd       = 10;
    instr.mul.rn       = 9;
    instr.mul.set_cond = 1;
    instr.mul.rm       = 2;

    mul_exec(&instr);
    mu_assert(!get_zflag);
    mu_assert(get_nflag);
    return 0;
}

static int test_cpsr_zer() {
    set_register(2, 0x00000003);
    set_register(5, 0x0000001f);
    set_register(9, 0xffffffa3);

    union decoded_instr instr;
    instr.mul.acc      = 1;
    instr.mul.rs       = 5;
    instr.mul.rd       = 10;
    instr.mul.rn       = 9;
    instr.mul.set_cond = 1;
    instr.mul.rm       = 2;

    mul_exec(&instr);
    mu_assert(get_zflag);
    mu_assert(!get_nflag);
    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_normal);
    mu_run_test(test_acc);
    mu_run_test(test_cpsr);
    mu_run_test(test_cpsr_zer);

    return 0;
}

int main() {
    int res = test_all();

    if (res != 0) {
        printf("TEST #%d FAILED.\n", res);
    } else {
        printf("%d passed\n", tests_run);
    }

    return res != 0;
}
