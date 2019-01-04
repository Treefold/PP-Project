#ifndef image_h
#define image_h

unsigned char*** read_img (char* source_name, char* *head, unsigned int *h, unsigned int *w);

void write_img (unsigned char*** img, char* head, char* destination_name, uint32_t h, uint32_t w);

unsigned char*** empty_img (uint32_t h, uint32_t w);

void free_img (unsigned char*** img, uint32_t h, uint32_t w);

unsigned char*** duplicate_img (unsigned char*** img, uint32_t h, uint32_t w);

unsigned char*** perm_img (unsigned char*** img, uint32_t* rand_perm, uint32_t h, uint32_t w);

unsigned char*** crypt_img (unsigned char*** img, uint32_t* R, uint32_t SV, uint32_t h, uint32_t w);

unsigned char*** decrypt_img (unsigned char*** img, uint32_t* R, uint32_t SV, uint32_t h, uint32_t w);

double test_chi_img (unsigned char*** img, uint32_t h, uint32_t w, uint32_t chanel);

unsigned char*** grayscale_img (unsigned char*** img, uint32_t h, uint32_t w);

unsigned char**** read_models ();

void free_models (unsigned char**** models);

unsigned char** I_want_the_colors();

void free_colors (unsigned char** colors);

void color_border (unsigned char*** img, uint32_t x, uint32_t y, unsigned char* C, uint32_t h, uint32_t w);

#endif // image_h
