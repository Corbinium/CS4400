/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Corbin Baraney",     /* Full name */
  "u1066089@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}

char unrolled_complex_dscr[] = "unrolled_complex: Combine a tile and unrolling algorithm to modify the image in patches that are 32row x 8col";
void unrolled_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;
  int ii, jj;

  int srcI, destI;
  int gray;

  for(i = 0; i < dim; i += 32) {
    for(j = 0; j < dim; j += 8) {
      for(ii = i; ii < i + 32; ii++) {
        srcI = RIDX(ii, j, dim);
        destI = RIDX(dim - j - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+1;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+2;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+3;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+4;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+5;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+6;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;

        jj = j+7;
        srcI = RIDX(ii, jj, dim);
        destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
        gray = ((unsigned int)src[srcI].red + (unsigned int)src[srcI].green + (unsigned int)src[srcI].blue) / 3;
        dest[destI].red = gray;
        dest[destI].green = gray;
        dest[destI].blue = gray;
      }
    }
  }
}


/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  unrolled_complex(dim, src, dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  // add_complex_function(&tile04_complex, tile04_complex_descr);
  // add_complex_function(&tile08_complex, tile08_complex_descr);
  // add_complex_function(&tile16_complex, tile16_complex_descr);
  // add_complex_function(&tile32_complex, tile32_complex_descr);
  // add_complex_function(&precalculate_complex, precalculate_complex_descr);
  // add_complex_function(&combo32_complex, combo32_complex_descr);
  // add_complex_function(&combo16_complex, combo16_complex_descr);
  // add_complex_function(&combo8_complex, combo8_complex_descr);
  // add_complex_function(&combo4_complex, combo4_complex_descr);
  // add_complex_function(&combo2_complex, combo2_complex_descr);
  // add_complex_function(&unrolled_complex, unrolled_complex_dscr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
        num_neighbors++;
        red += (int) src[RIDX(i+ii,j+jj,dim)].red;
        green += (int) src[RIDX(i+ii,j+jj,dim)].green;
        blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_9square(int dim, int i, int j, pixel *src) 
{
  int i00, i01, i02, i10, i11, i12, i20, i21, i22;
  pixel current_pixel;
  int red, green, blue;

  i00 = RIDX(i, j, dim);
  i01 = i00+1;
  i02 = i00+2;
  i10 = i00+dim;
  i11 = i10+1;
  i12 = i10+2;
  i20 = i10+dim;
  i21 = i20+1;
  i22 = i20+2;

  red = src[i00].red + src[i01].red + src[i02].red;
  green = src[i00].green + src[i01].green + src[i02].green;
  blue = src[i00].blue + src[i01].blue + src[i02].blue;

  red += src[i10].red + src[i11].red + src[i12].red;
  green += src[i10].green + src[i11].green + src[i12].green;
  blue += src[i10].blue + src[i11].blue + src[i12].blue;

  red += src[i20].red + src[i21].red + src[i22].red;
  green += src[i20].green + src[i21].green + src[i22].green;
  blue += src[i20].blue + src[i21].blue + src[i22].blue;

  red = red / 9;
  green = green / 9;
  blue = blue / 9;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_4square(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = i0+dim;

  red = (src[i0].red + src[i0+1].red +
         src[i1].red + src[i1+1].red) / 4;
  green = (src[i0].green + src[i0+1].green +
           src[i1].green + src[i1+1].green) / 4;
  blue = (src[i0].blue + src[i0+1].blue +
          src[i1].blue + src[i1+1].blue) / 4;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_6rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1, i2;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = i0+dim;
  i2 = i0+dim+dim;

  red = (src[i0].red + src[i0+1].red +
         src[i1].red + src[i1+1].red +
         src[i2].red + src[i2+1].red) / 6;
  green = (src[i0].green + src[i0+1].green +
           src[i1].green + src[i1+1].green +
           src[i2].green + src[i2+1].green) / 6;
  blue = (src[i0].blue + src[i0+1].blue + 
          src[i1].blue + src[i1+1].blue +
          src[i2].blue + src[i2+1].blue) / 6;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_3rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1, i2;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = i0+dim;
  i2 = i0+dim+dim;

  red = (src[i0].red +
         src[i1].red +
         src[i2].red) / 3;
  green = (src[i0].green +
           src[i1].green +
           src[i2].green) / 3;
  blue = (src[i0].blue +
          src[i1].blue +
          src[i2].blue) / 3;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_2rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = i0+dim;

  red = (src[i0].red +
         src[i1].red) / 2;
  green = (src[i0].green +
           src[i1].green) / 2;
  blue = (src[i0].blue +
          src[i1].blue) / 2;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_6rectDown(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = i0+dim;

  red = (src[i0].red + src[i0+1].red + src[i0+2].red +
         src[i1].red + src[i1+1].red + src[i1+2].red) / 6;
  green = (src[i0].green + src[i0+1].green + src[i0+2].green +
           src[i1].green + src[i1+1].green + src[i1+2].green) / 6;
  blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue +
          src[i1].blue + src[i1+1].blue + src[i1+2].blue) / 6;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_3rectDown(int dim, int i, int j, pixel *src) 
{
  int i0;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);

  red = (src[i0].red + src[i0+1].red + src[i0+2].red) / 3;
  green = (src[i0].green + src[i0+1].green + src[i0+2].green) / 3;
  blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue) / 3;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

__attribute__((always_inline)) static pixel weighted_combo_2rectDown(int dim, int i, int j, pixel *src) {
  int i0;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);

  red = (src[i0].red + src[i0+1].red) / 2;
  green = (src[i0].green + src[i0+1].green) / 2;
  blue = (src[i0].blue + src[i0+1].blue) / 2;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}

char tile_motion_descr[] = "tile_motion: Implementation that separates the edge cases out and tiles everything.";
void tile_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim-4; i += 4) {
    for (j = 0; j < dim-2; j++) {
      dst[RIDX(i, j, dim)] = weighted_combo_9square(dim, i, j, src);
      dst[RIDX(i+1, j, dim)] = weighted_combo_9square(dim, i+1, j, src);
      dst[RIDX(i+2, j, dim)] = weighted_combo_9square(dim, i+2, j, src);
      dst[RIDX(i+3, j, dim)] = weighted_combo_9square(dim, i+3, j, src);
    }
    dst[RIDX(i, j, dim)] = weighted_combo_6rectRight(dim, i, j, src);
    dst[RIDX(i, j+1, dim)] = weighted_combo_3rectRight(dim, i, j+1, src);

    dst[RIDX(i+1, j, dim)] = weighted_combo_6rectRight(dim, i+1, j, src);
    dst[RIDX(i+1, j+1, dim)] = weighted_combo_3rectRight(dim, i+1, j+1, src);

    dst[RIDX(i+2, j, dim)] = weighted_combo_6rectRight(dim, i+2, j, src);
    dst[RIDX(i+2, j+1, dim)] = weighted_combo_3rectRight(dim, i+2, j+1, src);

    dst[RIDX(i+3, j, dim)] = weighted_combo_6rectRight(dim, i+3, j, src);
    dst[RIDX(i+3, j+1, dim)] = weighted_combo_3rectRight(dim, i+3, j+1, src);
  }
  
  for (j = 0; j < dim-2; j++) {
    dst[RIDX(i, j, dim)] = weighted_combo_9square(dim, i, j, src);
    dst[RIDX(i+1, j, dim)] = weighted_combo_9square(dim, i+1, j, src);
    dst[RIDX(i+2, j, dim)] = weighted_combo_6rectDown(dim, i+2, j, src);
    dst[RIDX(i+3, j, dim)] = weighted_combo_3rectDown(dim, i+3, j, src);
  }

  // i = dim-8;
  // j = dim-2;
  dst[RIDX(i, j, dim)] = weighted_combo_6rectRight(dim, i, j, src);
  dst[RIDX(i, j+1, dim)] = weighted_combo_3rectRight(dim, i, j+1, src);

  dst[RIDX(i+1, j, dim)] = weighted_combo_6rectRight(dim, i+1, j, src);
  dst[RIDX(i+1, j+1, dim)] = weighted_combo_3rectRight(dim, i+1, j+1, src);

  dst[RIDX(i+2, j, dim)] = weighted_combo_4square(dim, i+2, j, src);
  dst[RIDX(i+2, j+1, dim)] = weighted_combo_2rectRight(dim, i+2, j+1, src);

  dst[RIDX(i+3, j, dim)] = weighted_combo_2rectDown(dim, i+3, j, src);
  i = RIDX(i+3, j+1, dim);
  dst[i] = src[i];
}

/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  tile_motion(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  // add_motion_function(&edgeCase_motion, edgeCase_motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
