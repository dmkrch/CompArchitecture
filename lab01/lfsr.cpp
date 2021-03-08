#include <iostream>
#include "lfsr.h"

// Возвращает бит номер n числа x.
// Предполагается 0 <= n <= 31
unsigned get_bit(unsigned x, unsigned n) 
{
    return (x >> n) & 1;
}

// Выставляет значение v в бите номер n
// Предполагается что 0 <= n <= 31 и v равно 0 или 1
void set_bit(unsigned * x, unsigned n, 
    unsigned v) 
{
    *x ^= (-v ^ *x) & (1 << n);
}

// Меняет значение бита номер n на противоположное.
// Предполагается что 0 <= n <= 31
void flip_bit(unsigned * x, unsigned n) 
{
    *x = *x ^ (1 << n);
}

void lfsr_calculate(uint16_t *reg) 
{
    // xor 0 and 2
    // xor .. and 3
    // xor .. and 5
    // every bit we need to shift to right by 1
    // result of .. write to 15 bit

    uint16_t first_temp_bit;
    uint16_t second_temp_bit;

    first_temp_bit = get_bit(*reg, 0);
    second_temp_bit = get_bit(*reg, 2);

    first_temp_bit ^= second_temp_bit;
    // now in first_temp_bit 0^2

    second_temp_bit = get_bit(*reg, 3);
    first_temp_bit ^= second_temp_bit;
    // now in first_temp_bit 0^2^3


    second_temp_bit = get_bit(*reg, 5);
    first_temp_bit ^= second_temp_bit;
    // now in first_temp_bit 0^2^3^5

    *reg >>= 1;
    set_bit((unsigned*)reg, 15, first_temp_bit);
}

