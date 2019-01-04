#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "image.h"
#include "cryptography.h"
#include "patterns.h"
#include <string.h>

void again (char* s);
void wait();
void read_file_name (char* s, char* text);
void write_file_name (char* s, char* text, char* type);

int main()
{
    unsigned char**** models = read_models ();
    unsigned char** numbers_border_color = I_want_the_colors();
    Window *f;

    uint32_t h, w, n, k, r0, SV,* R,* perm,* inv;
    char* head, choice, cc, source[260], dest[260], key[260], *af;
    unsigned char ***img, ***img_grayscale, ***c_img;
    FILE *key_file;

    while (1)
    {
        system ("cls");
        printf ("Choose wisely:\n");
        printf ("1. Encrypt\n");
        printf ("2. Decrypt\n");
        printf ("3. Template Matching\n");
        printf ("0. Exit\n");
        printf ("P.S.: Don't forget to press enter\n");
        printf ("\nYour choice: ");
        /// getting the choice
        choice = fgetc (stdin);
        if (choice != '\n')
        {
            cc = fgetc(stdin);
            while (cc != '\n') {cc = fgetc (stdin);}
        }
        if (choice < '0' || '3' < choice)
        {
            again ("I don't know that command, please try anoter one");
            continue;
        }

        if (choice == '0') {return 0;}

        read_file_name (source, "bitmap image");

        img = read_img (source, &head, &h, &w);

        if (choice == '3') ///template matching
        {
            wait();
            img_grayscale = grayscale_img (img, h, w);
            n = 0;
            f = malloc (w * h * sizeof (Window));
            af = calloc (w * h, sizeof (char));

            for (k = 0; k < 10; ++k)
            {
                n += template_matching (img, h, w, k, models[k], 0.5, f + n);
            }

            elim_non_max(f, af, n, h, w);

            for (k = 0; k < n; ++k)
            {
                if (af[k] == 0)
                {
                    color_border (img, f[k].x, f[k].y, numbers_border_color[f[k].number], h, w);
                }
            }

            free (af);
            free (f);
            free_img (img_grayscale, h, w);
        }
        else
        {
            read_file_name (key, "secret_key");
            key_file = fopen (key, "r");
            fscanf (key_file, "%d%d", &r0, &SV);
            fclose (key_file);

            R = generate_rand_secv (r0, 2 * h * w);
            perm = generate_rand_perm(R, h * w);

            if (choice == '1')
            {
                double red_channel, green_channel, blue_channel;
                red_channel = test_chi_img (img, h, w, 2);
                green_channel = test_chi_img (img, h, w, 1);
                blue_channel = test_chi_img (img, h, w, 0);
                printf ("Test Chi^2 on initial image:\n\tR - %.2lf\n\tG - %.2lf\n\tB - %.2f\n", red_channel, green_channel, blue_channel);
                c_img = perm_img (img, perm, h, w);
                free_img (img, h, w);
                img = crypt_img (c_img, R, SV, h, w);
                red_channel = test_chi_img (img, h, w, 2);
                green_channel = test_chi_img (img, h, w, 1);
                blue_channel = test_chi_img (img, h, w, 0);
                printf ("Test Chi^2 on initial image:\n\tR - %.2lf\n\tG - %.2lf\n\tB - %.2f\n", red_channel, green_channel, blue_channel);
                again("");
            }
            else
            {
                inv = inv_perm (perm, h * w);
                c_img = decrypt_img (img, R, SV, h, w);
                free_img (img, h, w);
                img = perm_img (c_img, inv, h, w);
                free (inv);
            }

            free_img (c_img, h, w);
            free (perm);
            free (R);
        }

        write_file_name (dest, "P.S.: The \".bmp\" will be added by me\nThe resoulted image's name (whithout \".bmp\") is: ", ".bmp");

        write_img (img, head, dest, h, w);

        free (head);
        free (img);
        again ("Done");
    }

    free_models (models);
    free_colors (numbers_border_color);

    return 0;
}

void again (char* s)
{
    printf ("\n%s\n", s);
    printf ("Press enter to continue\n");
    char cc = fgetc (stdin);
    while (cc != '\n') {cc = fgetc (stdin);}
}

void wait()
{
    system ("cls");
    printf ("Processing... This might take a while");
}

void read_file_name (char* s, char* text)
{
    FILE *img_path_file = NULL;
    while (img_path_file == NULL)
    {
        system ("cls");
        printf ("The %s's source name is: ", text);
        fgets (s, 255, stdin);
        if (s[strlen (s) - 1] == '\n') {s[strlen (s) - 1] = '\0';}
        img_path_file = fopen (s, "rb");
        if (img_path_file == NULL)
        {
            again ("I couldn't find that file path, please try anoter one");
        }
    }
    fclose (img_path_file);
}

void write_file_name (char* s, char* text, char* type)
{
    char not_allowed[] = "\"\\/:*?<>|^\b\n";
    int i;
    while (1)
    {
        system ("cls");
        printf ("%s", text);
        fgets (s, 250, stdin);
        if (s[strlen (s) - 1] == '\n') {s[strlen (s) - 1] = '\0';}
        i = 0;
        while (strchr(" \b\n", s[i]) != NULL) {++i;} ///in the worst case when s[i] = '\0',the "while" will end
        if (s[i] == '\0') {again ("There must be a name");}
        else if (strpbrk (s, not_allowed) == NULL ) {break;}
        else {again ("This name can't be used for a file");}
    }
    strcat (s, type);
}
