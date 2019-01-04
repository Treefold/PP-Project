#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include "image.h"
#include "cryptography.h"
#include <string.h>

void color_border (unsigned char*** img, uint32_t x, uint32_t y, unsigned char* C, uint32_t h, uint32_t w)
{
    if (x - 6 > h || y - 4 > w){return;}
    int register i , j;
    int up = (x > 7) ? x - 7 : 0, down = (x + 8 < h) ? x + 7 : h - 1, left = (y > 5) ? y - 5 : 0, right = (y + 6 < w) ? y + 5 : w - 1;
    for (i = up; i <= down; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            img[i][left][j] = img[i][right][j] = C[j];
        }
    }
    for (i = left; i <= right; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            img[up][i][j] = img[down][i][j] = C[j];
        }
    }
}

void free_colors (unsigned char** colors)
{
    int i;
    for (i = 0; i < 10; ++i)
    {
        free (colors[i]);
    }
    free (colors);
}

unsigned char** I_want_the_colors()
{
    unsigned char** colors = malloc (10 * sizeof (unsigned char*));
    int i;
    for (i = 0; i < 10; ++i)
    {
        colors[i] = calloc (3, sizeof (unsigned char));
    }
    colors[0][2] = 255;
    colors[1][2] = colors[1][1] = 255;
    colors[2][1] = 255;
    colors[3][1] = colors[3][0] = 255;
    colors[4][2] = colors[4][0] = 255;
    colors[5][0] = 255;
    colors[6][2] = colors[6][1] = colors[6][0] = 192;
    colors[7][2] = 255, colors[7][1] = 140;
    colors[8][2] = colors[8][0] = 128;
    colors[9][2] = 128;
    return colors;
}

void free_models (unsigned char**** models)
{
    int i;
    for (i = 0; i < 10; ++i)
    {
        free_img (models[i], 15, 11);
    }
    free (models);
}

unsigned char**** read_models ()
{
    uint32_t h, w;
    unsigned char*** model = malloc(sizeof (unsigned char***)),**** models_grayscale = malloc(10 * sizeof (unsigned char***));
    unsigned int register nr;
    char model_file[13] = "cifra0.bmp", *head;
    FILE *in;
    for (nr = 0; nr < 10; ++nr)
    {
        in = fopen (model_file, "rb");
        if (in == NULL)
        {
            models_grayscale[nr] = empty_img (15, 11); /// no image, no problem; it means it's just a blank model
        }
        else
        {
            model = read_img (model_file, &head, &h, &w);
            models_grayscale[nr] = grayscale_img (model, h, w);
            free_img (model, h, w);
            free (head);
        }
        fclose (in);
        ++model_file[5]; /// from "cifra6.bmp" to "cifra7.bmp"
    }
    return models_grayscale;
}

unsigned char*** grayscale_img (unsigned char*** img, uint32_t h, uint32_t w)
{
    uint32_t size_of_img = h * w;
    unsigned char*** img_grayscale = empty_img (h, w);
    register int i, j;
    unsigned char aux;
    for (i = 0; i < size_of_img; ++i)
    {
        aux = 0.299 * img[0][i][2] + 0.587 * img[0][i][1] + 0.114 * img[0][i][0];
        for (j = 0; j < 3; ++j)
        {
            img_grayscale[i / w][i % w][j] = aux;
        }
    }
    return img_grayscale;
}

double test_chi_img (unsigned char*** img, uint32_t h, uint32_t w, uint32_t chanel)
{
    double chi = 0, ff = h * w / 256;
    uint32_t* f = calloc(256, sizeof (uint32_t));
    register int i, size_of_img = h * w;
    for (i = 0; i < size_of_img; ++i)
    {
        f[(unsigned char) img[0][i][chanel]]++;
    }
    for (i = 0; i < 256; ++i)
    {
        chi += (f[i] - ff) * (f[i] - ff) / ff;
    }
    free (f);
    return chi;
}

unsigned char*** decrypt_img (unsigned char*** img, uint32_t* R, uint32_t SV, uint32_t h, uint32_t w)
{
    unsigned char*** img_decrypt = empty_img (h, w);
    uint32_t size_of_img = h * w;
    register int i, j;
    for (i = size_of_img - 1; i > 0; --i)
    {   for (j = 0; j < 3; ++j)
        {
            img_decrypt[0][i][j] = img[0][i - 1][j] ^ img[0][i][j] ^ R[size_of_img + i];
        }
    }
    for (j = 0; j < 3; ++j)
    {
        img_decrypt[0][0][j] = SV ^ img[0][0][j] ^ R[w * h];
    }
    return img_decrypt;
}

unsigned char*** crypt_img (unsigned char*** img, uint32_t* R, uint32_t SV, uint32_t h, uint32_t w)
{
    unsigned char*** img_crypt = empty_img (h, w);
    uint32_t size_of_img = h * w;
    register int i, j;
    for (j = 0; j < 3; ++j)
    {
        img_crypt[0][0][j] = SV ^ img[0][0][j] ^ R[w * h];
    }
    for (i = 1; i < size_of_img; ++i)
    {   for (j = 0; j < 3; ++j)
        {
            img_crypt[0][i][j] = img_crypt[0][i - 1][j] ^ img[0][i][j] ^ R[size_of_img + i];
        }
    }
    return img_crypt;
}

unsigned char*** perm_img (unsigned char*** img, uint32_t* rand_perm, uint32_t h, uint32_t w)
{
    unsigned char*** img_perm = empty_img (h, w);
    uint32_t size_of_img = h * w;
    register int i, j;

    for (i = 0; i < size_of_img; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            img_perm[0][rand_perm[i]][j] = img[0][i][j];
        }
    }

    return img_perm;
}

unsigned char*** duplicate_img (unsigned char*** img, uint32_t h, uint32_t w)
{
    unsigned char*** img_duplicate = empty_img (h, w);
    unsigned int register i, j, c;
    for (c = 0; c < 3; ++c)
    {
        for (i = 0; i < h; ++i)
        {
            for (j = 0; j < w; ++j)
            {
                img_duplicate[i][j][c] = img[i][j][c];
            }
        }
    }
    return img_duplicate;
}

void free_img (unsigned char*** img, uint32_t h, uint32_t w)
{
    if (img == NULL)
    {
        return;
    }
    register int i, j;
    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            free (img[i][j]);
        }
    }
    free (img[0]);
    free (img);
}

unsigned char*** empty_img (uint32_t h, uint32_t w)
{
    unsigned char*** img = malloc (h * sizeof (unsigned char**));
    register int i;
    unsigned char** aux = malloc (w * h * sizeof(unsigned char*));
    for (i = 0; i < h * w; ++i)
    {
        aux[i] = calloc (3, sizeof (unsigned char));
    }
    for (i = 0; i < h; ++i)
    {
        img[i] = aux + i * w;
    }
    return img;
}

void write_img (unsigned char*** img, char* head, char* destination_name, uint32_t h, uint32_t w)
{
    FILE *destination = fopen (destination_name, "wb");
    fwrite (head, 1, 54, destination);
    uint32_t padding = (w % 4 != 0) ? (4 - (3 * w) % 4) : 0;

    register int i, j;
    for (i = 0; i + 1 < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            fwrite (img[i][j], 3, 1, destination);
        }
        fseek (destination, padding, SEEK_CUR);
    }
    for (j = 0; j < w; ++j)
    {
        fwrite (img[i][j], 3, 1, destination);
    }
    fflush (destination);
    fclose (destination);
}

unsigned char*** read_img (char* source_name, char* *head, unsigned int *h, unsigned int *w)
{
    FILE *source = fopen (source_name, "rb");
    if (source == NULL)
    {
        printf ("Not a good source");
        return malloc(sizeof(unsigned char));
    }
    *head = malloc (54 * sizeof(char));
    fread (*head, sizeof (char), 54, source);
    fseek (source, 18, SEEK_SET);
    fread (w, sizeof (unsigned int), 1, source);
    fread (h, sizeof (unsigned int), 1, source);
    unsigned int padding = ((*w) % 4 != 0) ? (4 - (3 * (*w) % 4)) : 0;

    unsigned char*** img = empty_img (*h, *w);

    register int i, j;
    fseek (source, 54, SEEK_SET);

    for (i = 0; i < *h; ++i)
    {
        for (j = 0; j < *w; ++j)
        {
            fread (img[i][j], 3, 1, source);
        }
        fseek (source, padding, SEEK_CUR);
    }

    fclose (source);
    return img;
}
