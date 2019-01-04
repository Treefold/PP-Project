#ifndef cryptography_h
#define cryptography_h
#include <stdint.h>
#define sp(a, b)

uint32_t xorshift32 (uint32_t state[static 1]);

uint32_t* generate_rand_secv (uint32_t r0, uint32_t size_of_R);

void xor_swap (uint32_t *a, uint32_t *b);

uint32_t* generate_rand_perm (uint32_t* R, uint32_t size_of_perm);

uint32_t* inv_perm (uint32_t* perm, uint32_t size_of_perm);

int verif_perm (uint32_t* perm, uint32_t size_of_perm);

#endif // cryptography_h
