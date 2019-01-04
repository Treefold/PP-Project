#ifndef patterns_h
#define patterns_h

typedef struct Window
{
    uint32_t x, y, number;
    double precizion;
} Window;

void elim_non_max (Window* f, char* af, uint32_t n, uint32_t h, uint32_t w);

double overlap (Window a, Window b, uint32_t h, uint32_t w);

int comparatorWindow (const void *p, const void *q); /// https://stackoverflow.com/questions/20584499/why-qsort-from-stdlib-doesnt-work-with-double-values-c

double media (unsigned char*** img, uint32_t x, uint32_t y, uint32_t h, uint32_t w);

double deviation (unsigned char*** img, uint32_t x, uint32_t y, uint32_t h, uint32_t w, double cu_bara);

double cross_correlation (unsigned char*** img, uint32_t x, uint32_t y, unsigned char*** model, uint32_t h, uint32_t w, double S_cu_bara);

uint32_t template_matching (unsigned char*** img, uint32_t h, uint32_t w, uint32_t k, unsigned char*** model, double ps, Window* f);

#endif // patterns_h
