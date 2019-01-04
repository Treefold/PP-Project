#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "patterns.h"

void elim_non_max (Window* f, char* af, uint32_t n, uint32_t h, uint32_t w)
{
    qsort (f, n, sizeof (Window), comparatorWindow);

    unsigned int register i, j;
    for (i = 0; i < n; ++i)
    {
        for (j = i + 1; j < n; ++j)
        {
            if (af[i] == 0 && af[j] == 0 && overlap (f[i], f[j], h, w) >= 0.2)
            {
                af[j] = 1;
            }
        }
    }
}

double miOver (double a, double b)
{
    return (a < b) ? a : b;
}

double overlap (Window a, Window b, uint32_t h, uint32_t w)
{
    int a_up, a_down, a_left, a_right, b_up, b_down, b_left, b_right;
    a_up = (a.x > 8) ? a.x - 7 : 0;
    a_down = (a.x + 7 < h) ? a.x + 7 : h;
    a_left = (a.y > 6) ? a.y - 5 : 0;
    a_right = (a.y + 5 < w) ? a.y + 5 : w;
    b_up = (b.x > 8) ? b.x - 7 : 0;
    b_down = (b.x + 7 < h) ? b.x + 7 : h;
    b_left = (b.y > 6) ? b.y - 5 : 0;
    b_right = (b.y + 5 < w) ?  b.y + 5 : w;

    if (a_down <= b_up || b_down <= a_up || a_right <= b_left || b_right <= a_left) {return 0;}

    double numitor, numarator;
    numitor = miOver (a_down - b_up, b_down - a_up) * miOver (a_right - b_left, b_right - a_left);
    numarator = (a_down - a_up) * (a_right - a_left) + (b_down - b_up) * (b_right - b_left) - numitor;
    return numitor / numarator;
}

int comparatorWindow (const void *p, const void *q)
{
    double a = ((struct Window *)p)->precizion;
    double b = ((struct Window *)q)->precizion;
    return (a < b) ? 1 : (a > b) ? -1 : 0;
}

uint32_t template_matching (unsigned char*** img, uint32_t h, uint32_t w, uint32_t k, unsigned char*** model, double ps, Window* f)
{
    double S_cu_bara = media (model, 7, 5, 15, 11), corr;
    uint32_t n = 0;
    int register i, j;
    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            corr = cross_correlation (img, i, j, model, h, w, S_cu_bara);
            if (corr >= ps)
            {
                f[n].number = k;
                f[n].precizion = corr;
                f[n].x = i;
                f[n].y = j;
                ++n;
            }
        }
    }
    return n;
}

double cross_correlation (unsigned char*** img, uint32_t x, uint32_t y, unsigned char*** model, uint32_t h, uint32_t w, double S_cu_bara)
{
    double sum = 0, f_cu_bara = media (img, x, y, h, w);
    double numitor = 165 * deviation (img, x, y, h, w, f_cu_bara) * deviation (model, 7, 5, 15, 11, S_cu_bara);
    int i, j, f_fara_bara, S_fara_bara;
    for (i = - 7; i < 8; ++i)
    {
        for (j = -5; j < 6; ++j)
        {
            f_fara_bara = (x + i < 0 || y + j < 0 || x + i >= h || y + j >= w) ? 0 : img[x + i][y + j][0];
            S_fara_bara = model[7 + i][5 + j][0];
            sum += (f_fara_bara - f_cu_bara) * (S_fara_bara - S_cu_bara);
        }
    }

    return sum / numitor;
}

double deviation (unsigned char*** img, uint32_t x, uint32_t y, uint32_t h, uint32_t w, double cu_bara)
{
    double dev = 0, fara_bara;
    int register i, j;
    for (i = -7; i < 8; ++i)
    {
        for (j = -5; j < 6; ++j)
        {
            fara_bara = (x + i < 0 || y + j < 0 || x + i >= h || y + j >= w) ? 0 : img[x + i][y + j][0];
            dev += (fara_bara - cu_bara) * (fara_bara - cu_bara);
        }
    }
    if (dev <= 0) {return 1;}
    return sqrt(dev / 164);
}

double media (unsigned char*** img, uint32_t x, uint32_t y, uint32_t h, uint32_t w)
{
    double m = 0;
    int i, j;
    for (i = -7; i < 8; ++i)
    {
        for (j = -5; j < 6; ++j)
        {
            m += (x + i < 0 || y + j < 0 || x + i >= h || y + j >= w) ? 0 : img[x + i][y + j][0];
        }
    }
    return m / 165;
}
