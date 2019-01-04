# PP-project
A program that can encrypt and decrypt a ".bmp" image using only 2 numbers and can find numbers in a ".bmp" image using Template-Matching

University of Bucharest, Faculty of Mathematics and Informatics

Subject: Procedural Programming

Class Teacher: Bogdan Alexe

Supervisor: Mihai Ghidoveanu

Author: Mihai-Daniel Surcea, Class 132

Semestrial Project Documentation

Main:
- again: might display a message and makes the user press enter to continue
- wait: just informs the user that the current task might take some time to complete
- read_file_name: a loop which won’t finish until the user provides a good source (a good source means an existing source)
- write_file_name: a loop which won’t finish until the user provides a good file name (a good file name doesn’t contain restricted characters)
- main: a menu (a kind of a user interface)

Image:
- an Image in used by the program - a 3D array & an image is a file existing in the program file with the .bmp extension
- all _img are programs that are using Images or images
- all img or img_ are 3D arrays or arrays of 3D arrays
- the program file is the file where the program was ran or will be ran
- read_img: reads an image (.bmp) and returns an Image (a 3D array)
- write_img: receives an Image and creates or overwrites an image in the program file
- empty_img: a kind of a constructor for the Image (the 3D array) and returns an Image corresponding to a black image (initialized with 0 - is using calloc)
 	- the Image is allocated like a 2D array with 3 collons (a 1D array of pixels) and can be viewed as a 2D array of pixels (corresponding to an image)
- free_img: deletes all the memory allocated for an Image
- duplicate_img: receives an Image and returns an identical Image (a duplicate - allocating new space)
- perm_img: viewing an Image like a 1D array of pixels, the program is multiplying this array with a permutation creating a new Image
- crypt_img: using a symmetric encryption with a key formed from 2 numbers, r0 that generates the pseudo random number secvention (named R), and SV the seed that encrypts our Image, and the XOR (^) proprietes; viewing the Image like a 1D array of pixels, I = I[1], I[2], I[3], ... , I[H * W - 1], where H is the height of the Image and W is the width of the Image, we create a new Image C = C[0], C[1], ... , C[H * W] according to:
	- k = 0 : C[0] = SV ^ I[0] ^ R[H * W]
	- 0 < k < H * W : C[k] = C[k-1] ^ I[k] ^ R[H * W + k]
- decrypt_img: the reverse operation of encryption uses the same key to decrypt
- test_chi_img: returns a real number telling the distribution of pixels of the histogram on a specific channel (Red, Green, Blue)
- grayscale_img: receives an Image and returns na Image where all channels of each pixel is transformed according to 0.299 * red_channel + 0.587 * green_channel + 0.114 * blue_channel
- read_models: returns an array of Images that were read from the program file and each Image represents a model for template matching (10 models for all single digit numbers)
- free_models: delete all the memory allocated for each model
- border_img: modifies a big Image (probably more than 100x100 pxl) - creates a border for a 15x11 pxl small Image with the center of the small Image (the 7 line and 5 collon pxl) overlapping the big Image x line y collon pxl (x and y are received as parameters)

Cryptography:
- xorshift32: https://en.wikipedia.org/wiki/Xorshift
- generate_rand_secv: using the idea from xorshift32, it generates an array containing pseudo random values
- xor_swap: just swaps 2 values
- generate_rand_perm: array from generate_rand_secv and extracts the pseudo generated numbers to create a permutation (https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle - modern version)
- inv_perm: receiving a permutation, returns that permutation at power (-1) (just math)
- verif_perm: a valid permutation should have all numbers from 1 to n (or 0 to n-1) only once

Patterns (Template Matching):
- uses some models to find patterns through an Image and colors the border of a match
- multiple matching might overlap so the function overlap calculates the overlap between two matches
- you might not want to see multiple matches overlapping so elim_non_max eliminates the overlap with more than 20% overlapping
- to find the matches between the Image with all the models, the function template_matching provides an array containing the x line and y column of the center of all the matches for a specific model from the Image
- uses the function cross_correlation returns the percentage of symmetry between a model and a 15x11 pxl part of an Image (50% in for all cases, it can be modified, but not by the user)
- the rest of the functions will just implement the mathematical part of the Cross-Correlation (https://en.wikipedia.org/wiki/Cross-correlation)
