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


// char precalculate_complex_descr[] = "precalculate_complex: Pre-calculate the values of the complex kernel";
// void precalculate_complex(int dim, pixel *src, pixel *dest)
// {
//   int i, j;

//   int srcI, destI;
//   int gray;

//   for(i = 0; i < dim; i++)
//     for(j = 0; j < dim; j++)
//     {
//       srcI = RIDX(i, j, dim);
//       destI = RIDX(dim - j - 1, dim - i - 1, dim);
//       gray = (unsigned int)src[srcI].red;
//       gray += (unsigned int)src[srcI].green;
//       gray += (unsigned int)src[srcI].blue;
//       gray /= 3;

//       dest[destI].red = gray;
//       dest[destI].green = gray;
//       dest[destI].blue = gray;
//     }
// }

// void tileAdjustable_complex(int dim, pixel *src, pixel *dest, int w)
// {
//   int i, j;
//   int ii, jj;

//   for(i = 0; i < dim; i += w)
//     for(j = 0; j < dim; j += w)
//     {
//       for(ii = i; ii < i + w; ii++) {
//         for(jj = j; jj < j + w; jj++) {

//           dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].red = ((int)src[RIDX(ii, jj, dim)].red +
//               (int)src[RIDX(ii, jj, dim)].green +
//               (int)src[RIDX(ii, jj, dim)].blue) / 3;

//           dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].green = ((int)src[RIDX(ii, jj, dim)].red +
//               (int)src[RIDX(ii, jj, dim)].green +
//               (int)src[RIDX(ii, jj, dim)].blue) / 3;

//           dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].blue = ((int)src[RIDX(ii, jj, dim)].red +
//               (int)src[RIDX(ii, jj, dim)].green +
//               (int)src[RIDX(ii, jj, dim)].blue) / 3;

//         }
//       }
//     }
// }

// char tile32_complex_descr[] = "tile32_complex: tile the loop of the naive implementation with w=32";
// void tile32_complex(int dim, pixel *src, pixel *dest)
// {
//   tileAdjustable_complex(dim, src, dest, 32);
// }

// char tile16_complex_descr[] = "tile16_complex: tile the loop of the naive implementation with w=16";
// void tile16_complex(int dim, pixel *src, pixel *dest)
// {
//   tileAdjustable_complex(dim, src, dest, 16);
// }

// char tile08_complex_descr[] = "tile08_complex: tile the loop of the naive implementation with w=08";
// void tile08_complex(int dim, pixel *src, pixel *dest)
// {
//   tileAdjustable_complex(dim, src, dest, 8);
// }

// char tile04_complex_descr[] = "tile04_complex: tile the loop of the naive implementation with w=04";
// void tile04_complex(int dim, pixel *src, pixel *dest)
// {
//   tileAdjustable_complex(dim, src, dest, 4);
// }

// void comboAdjustable_complex(int dim, pixel *src, pixel *dest, int w)
// {
//   int i, j;
//   int ii, jj;

//   int srcI, destI;
//   int gray;

//   for(i = 0; i < dim; i += w) {
//     for(j = 0; j < dim; j += w) {
//       for(ii = i; ii < i + w; ii++) {
//         for(jj = j; jj < j + w; jj++) {
//           srcI = RIDX(ii, jj, dim);
//           destI = RIDX(dim - jj - 1, dim - ii - 1, dim);
//           gray = (unsigned int)src[srcI].red;
//           gray += (unsigned int)src[srcI].green;
//           gray += (unsigned int)src[srcI].blue;
//           gray /= 3;

//           dest[destI].red = gray;
//           dest[destI].green = gray;
//           dest[destI].blue = gray;
//         }
//       }
//     }
//   }
// }

// char combo32_complex_descr[] = "combo32_complex: Combine the precalculate and tile32 implementations";
// void combo32_complex(int dim, pixel *src, pixel *dest)
// {
//   comboAdjustable_complex(dim, src, dest, 32);
// }

// char combo16_complex_descr[] = "combo16_complex: Combine the precalculate and tile16 implementations";
// void combo16_complex(int dim, pixel *src, pixel *dest)
// {
//   comboAdjustable_complex(dim, src, dest, 16);
// }

// char combo8_complex_descr[] = "combo8_complex: Combine the precalculate and tile8 implementations";
// void combo8_complex(int dim, pixel *src, pixel *dest)
// {
//   comboAdjustable_complex(dim, src, dest, 8);
// }

// char combo4_complex_descr[] = "combo4_complex: Combine the precalculate and tile4 implementations";
// void combo4_complex(int dim, pixel *src, pixel *dest)
// {
//   comboAdjustable_complex(dim, src, dest, 4);
// }

// char combo2_complex_descr[] = "combo2_complex: Combine the precalculate and tile2 implementations";
// void combo2_complex(int dim, pixel *src, pixel *dest)
// {
//   comboAdjustable_complex(dim, src, dest, 2);
// }

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

static pixel weighted_combo_9square(int dim, int i, int j, pixel *src) 
{
  int i0, i1, i2;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);
  i2 = RIDX(i+2, j, dim);

  red = (src[i0].red + src[i0+1].red + src[i0+2].red +
         src[i1].red + src[i1+1].red + src[i1+2].red +
         src[i2].red + src[i2+1].red + src[i2+2].red) / 9;
  green = (src[i0].green + src[i0+1].green + src[i0+2].green +
           src[i1].green + src[i1+1].green + src[i1+2].green +
           src[i2].green + src[i2+1].green + src[i2+2].green) / 9;
  blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue +
          src[i1].blue + src[i1+1].blue + src[i1+2].blue +
          src[i2].blue + src[i2+1].blue + src[i2+2].blue) / 9;

  current_pixel.red = (unsigned short) red;
  current_pixel.green = (unsigned short) green;
  current_pixel.blue = (unsigned short) blue;
  
  return current_pixel;
}

static pixel weighted_combo_4square(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);

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

static pixel weighted_combo_6rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1, i2;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);
  i2 = RIDX(i+2, j, dim);

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

static pixel weighted_combo_3rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1, i2;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);
  i2 = RIDX(i+2, j, dim);

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

static pixel weighted_combo_2rectRight(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);

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

static pixel weighted_combo_6rectDown(int dim, int i, int j, pixel *src) 
{
  int i0, i1;
  pixel current_pixel;
  int red, green, blue;
  red = green = blue = 0;

  i0 = RIDX(i, j, dim);
  i1 = RIDX(i+1, j, dim);

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

static pixel weighted_combo_3rectDown(int dim, int i, int j, pixel *src) 
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

static pixel weighted_combo_2rectDown(int dim, int i, int j, pixel *src) 
{
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

char edgeCase_motion_descr[] = "edgeCase_motion: Implementation that separates the edge cases out.";
void edgeCase_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim-2; i++) {
    for (j = 0; j < dim-2; j++) {
      dst[RIDX(i, j, dim)] = weighted_combo_9square(dim, i, j, src);
    }
  }

  i = dim-2;
  for (j = 0; j < dim-2; j++) {
    dst[RIDX(i, j, dim)] = weighted_combo_6rectDown(dim, i, j, src);
  }

  i = dim-1;
  for (j = 0; j < dim-2; j++) {
    dst[RIDX(i, j, dim)] = weighted_combo_3rectDown(dim, i, j, src);
  }

  j = dim-2;
  for (i = 0; i < dim-2; i++) {
    dst[RIDX(i, j, dim)] = weighted_combo_6rectRight(dim, i, j, src);
  }

  j = dim-1;
  for (i = 0; i < dim-2; i++) {
    dst[RIDX(i, j, dim)] = weighted_combo_3rectRight(dim, i, j, src);
  }

  i = dim-2;
  j = dim-2;
  dst[RIDX(i, j, dim)] = weighted_combo_4square(dim, i, j, src);

  i = dim-1;
  j = dim-2;
  dst[RIDX(i, j, dim)] = weighted_combo_2rectDown(dim, i, j, src);

  i = dim-2;
  j = dim-1;
  dst[RIDX(i, j, dim)] = weighted_combo_2rectRight(dim, i, j, src);

  i = RIDX(dim-1, dim-1, dim);
  dst[i] = src[i];
}

char tile_motion_descr[] = "tile_motion: Implementation that separates the edge cases out and tiles everything.";
void tile_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim-2; i++) {
    for (j = 0; j < dim-2; j++) {
      dst[RIDX(i, j, dim)] = weighted_combo_9square(dim, i, j, src);
    }
  }

  i = dim-2;
  for (j = 0; j < dim-2; j++) {
    dst[RIDX(i, j, dim)] = weighted_combo_6rectDown(dim, i, j, src);
    dst[RIDX(i+1, j, dim)] = weighted_combo_3rectDown(dim, i+1, j, src);
  }

  j = dim-2;
  for (i = 0; i < dim-2; i++) {
    dst[RIDX(i, j, dim)] = weighted_combo_6rectRight(dim, i, j, src);
    dst[RIDX(i, j+1, dim)] = weighted_combo_3rectRight(dim, i, j+1, src);
  }

  i = dim-2;
  j = dim-2;
  dst[RIDX(i, j, dim)] = weighted_combo_4square(dim, i, j, src);

  i = dim-1;
  j = dim-2;
  dst[RIDX(i, j, dim)] = weighted_combo_2rectDown(dim, i, j, src);

  i = dim-2;
  j = dim-1;
  dst[RIDX(i, j, dim)] = weighted_combo_2rectRight(dim, i, j, src);

  i = RIDX(dim-1, dim-1, dim);
  dst[i] = src[i];
}

// char inlineEdgeCase_motion_descr[] = "inlineEdgeCase_motion: Implementation that separates the edge cases out and inlines them.";
// void inlineEdgeCase_motion(int dim, pixel *src, pixel *dst) 
// {
//   int i, j;
//   int i0, i1, i2;
//   int red, green, blue;
//   int current;
//
//   for (i = 0; i < dim-2; i++) {
//     for (j = 0; j < dim-2; j++) {
//       current = RIDX(i, j, dim);
//       i0 = RIDX(i, j, dim);
//       i1 = RIDX(i+1, j, dim);
//       i2 = RIDX(i+2, j, dim);
//       red = (src[i0].red + src[i0+1].red + src[i0+2].red +
//             src[i1].red + src[i1+1].red + src[i1+2].red +
//             src[i2].red + src[i2+1].red + src[i2+2].red) / 9;
//       green = (src[i0].green + src[i0+1].green + src[i0+2].green +
//               src[i1].green + src[i1+1].green + src[i1+2].green +
//               src[i2].green + src[i2+1].green + src[i2+2].green) / 9;
//       blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue +
//               src[i1].blue + src[i1+1].blue + src[i1+2].blue +
//               src[i2].blue + src[i2+1].blue + src[i2+2].blue) / 9;
//       dst[current].red = (unsigned short) red;
//       dst[current].green = (unsigned short) green;
//       dst[current].blue = (unsigned short) blue;
//     }
//   }
//
//   i = dim-2;
//   for (j = 0; j < dim-2; j++) {
//     current = RIDX(i, j, dim);
//     i0 = RIDX(i, j, dim);
//     i1 = RIDX(i+1, j, dim);
//     red = (src[i0].red + src[i0+1].red + src[i0+2].red +
//           src[i1].red + src[i1+1].red + src[i1+2].red) / 6;
//     green = (src[i0].green + src[i0+1].green + src[i0+2].green +
//             src[i1].green + src[i1+1].green + src[i1+2].green) / 6;
//     blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue +
//             src[i1].blue + src[i1+1].blue + src[i1+2].blue) / 6;
//     dst[current].red = (unsigned short) red;
//     dst[current].green = (unsigned short) green;
//     dst[current].blue = (unsigned short) blue;
//   }
//
//   i = dim-1;
//   for (j = 0; j < dim-2; j++) {
//     current = RIDX(i, j, dim);
//     i0 = RIDX(i, j, dim);
//     red = (src[i0].red + src[i0+1].red + src[i0+2].red) / 3;
//     green = (src[i0].green + src[i0+1].green + src[i0+2].green) / 3;
//     blue = (src[i0].blue + src[i0+1].blue + src[i0+2].blue) / 3;
//     dst[current].red = (unsigned short) red;
//     dst[current].green = (unsigned short) green;
//     dst[current].blue = (unsigned short) blue;
//   }
//
//   j = dim-2;
//   for (i = 0; i < dim-2; i++) {
//     current = RIDX(i, j, dim);
//     i0 = RIDX(i, j, dim);
//     i1 = RIDX(i+1, j, dim);
//     i2 = RIDX(i+2, j, dim);
//     red = (src[i0].red + src[i0+1].red +
//           src[i1].red + src[i1+1].red +
//           src[i2].red + src[i2+1].red) / 6;
//     green = (src[i0].green + src[i0+1].green +
//             src[i1].green + src[i1+1].green +
//             src[i2].green + src[i2+1].green) / 6;
//     blue = (src[i0].blue + src[i0+1].blue + 
//             src[i1].blue + src[i1+1].blue +
//             src[i2].blue + src[i2+1].blue) / 6;
//     dst[current].red = (unsigned short) red;
//     dst[current].green = (unsigned short) green;
//     dst[current].blue = (unsigned short) blue;
//   }
//
//   j = dim-1;
//   for (i = 0; i < dim-2; i++) {
//     current = RIDX(i, j, dim);
//     i0 = RIDX(i, j, dim);
//     i1 = RIDX(i+1, j, dim);
//     i2 = RIDX(i+2, j, dim);
//     red = (src[i0].red +
//           src[i1].red +
//           src[i2].red) / 3;
//     green = (src[i0].green +
//             src[i1].green +
//             src[i2].green) / 3;
//     blue = (src[i0].blue +
//             src[i1].blue +
//             src[i2].blue) / 3;
//     dst[current].red = (unsigned short) red;
//     dst[current].green = (unsigned short) green;
//     dst[current].blue = (unsigned short) blue;
//   }
//
//   i = dim-2;
//   j = dim-2;
//   current = RIDX(i, j, dim);
//   i0 = RIDX(i, j, dim);
//   i1 = RIDX(i+1, j, dim);
//   red = (src[i0].red + src[i0+1].red +
//         src[i1].red + src[i1+1].red) / 4;
//   green = (src[i0].green + src[i0+1].green +
//           src[i1].green + src[i1+1].green) / 4;
//   blue = (src[i0].blue + src[i0+1].blue +
//           src[i1].blue + src[i1+1].blue) / 4;
//   dst[current].red = (unsigned short) red;
//   dst[current].green = (unsigned short) green;
//   dst[current].blue = (unsigned short) blue;
//
//   i = dim-1;
//   j = dim-2;
//   current = RIDX(i, j, dim);
//   i0 = RIDX(i, j, dim);
//   red = (src[i0].red + src[i0+1].red) / 2;
//   green = (src[i0].green + src[i0+1].green) / 2;
//   blue = (src[i0].blue + src[i0+1].blue) / 2;
//   dst[current].red = (unsigned short) red;
//   dst[current].green = (unsigned short) green;
//   dst[current].blue = (unsigned short) blue;
//
//   i = dim-2;
//   j = dim-1;
//   current = RIDX(i, j, dim);
//   i0 = RIDX(i, j, dim);
//   i1 = RIDX(i+1, j, dim);
//   red = (src[i0].red +
//          src[i1].red) / 2;
//   green = (src[i0].green +
//            src[i1].green) / 2;
//   blue = (src[i0].blue +
//           src[i1].blue) / 2;
//   dst[current].red = (unsigned short) red;
//   dst[current].green = (unsigned short) green;
//   dst[current].blue = (unsigned short) blue;
//
//   i = RIDX(dim-1, dim-1, dim);
//   dst[i] = src[i];
// }


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  naive_motion(dim, src, dst);
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
  add_motion_function(&edgeCase_motion, edgeCase_motion_descr);
  // add_motion_function(&inlineEdgeCase_motion, inlineEdgeCase_motion_descr);
  add_motion_function(&tile_motion, tile_motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
