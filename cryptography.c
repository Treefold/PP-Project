#include "cryptography.h"
#include <stdlib.h>
#include <stdio.h>

int verif_perm (uint32_t* perm, uint32_t size_of_perm)
{
    int* v = calloc (size_of_perm, sizeof (int));
    int register i;
    for (i = 0; i < size_of_perm; ++i)
    {
        if (perm[i] >= size_of_perm || v[perm[i]] != 0){printf("!!");}
        v[perm[i]]++;
    }
    free (v);
}

uint32_t* inv_perm (uint32_t* perm, uint32_t size_of_perm)
{
    uint32_t *inv = malloc (size_of_perm * sizeof (uint32_t));
    register int i = 0;
    while (i < size_of_perm)
    {
        inv[perm[i]] = i;
        ++i;
    }
    return inv;
}

uint32_t* generate_rand_perm (uint32_t *R, uint32_t size_of_perm)
{
    uint32_t* perm = (uint32_t*) malloc (size_of_perm * sizeof (uint32_t));
    register uint32_t i;
    uint32_t j, nr = 0, x;
    for (i = 0; i < size_of_perm; ++i)
    {
        perm[i] = i;
    }
    /// i = size_of_perm;
    while (--i)
    {
        j = R[++nr] % (i + 1);
        xor_swap (&perm[i], &perm[j]);
    }
    return perm;
}

void xor_swap (uint32_t *x, uint32_t *y)
{
    if (*x == *y) {return;}
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
}

uint32_t* generate_rand_secv (uint32_t r0, uint32_t size_of_R)
{
    uint32_t* R = (uint32_t*) malloc (size_of_R * sizeof (uint32_t));
    R[0] = r0;
    register int i = 1;
    while (i < size_of_R)
    {
        R[i] = R[i - 1];
        R[i] ^= R[i] << 13;
        R[i] ^= R[i] >> 17;
        R[i] ^= R[i] << 5;
        ++i;
    }
    return R;
}

uint32_t xorshift32 (uint32_t state[static 1])
{
	/** Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = state[0];
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	state[0] = x;
	return x;
}
