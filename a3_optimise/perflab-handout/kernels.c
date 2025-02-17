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
  "Corbin Barney",     /* Full name */
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

char precalculate_complex_descr[] = "precalculate_complex: Pre-calculate the values of the complex kernel";
void precalculate_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  int srcI, destI;
  short gray;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {
      srcI = RIDX(i, j, dim);
      destI = RIDX(dim - j - 1, dim - i - 1, dim);
      gray = (short)src[srcI].red;
      gray += (short)src[srcI].green;
      gray += (short)src[srcI].blue;
      gray /= 3;

      dest[destI].red = gray;
      dest[destI].green = gray;
      dest[destI].blue = gray;
    }
}

void tileAdjustable_complex(int dim, pixel *src, pixel *dest, int w)
{
  int i, j;
  int ii, jj;

  for(i = 0; i < dim; i += w)
    for(j = 0; j < dim; j += w)
    {
      for(ii = i; ii < i + w; ii++) {
        for(jj = j; j < j+w; j++) {

          dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].red = ((int)src[RIDX(ii, jj, dim)].red +
              (int)src[RIDX(ii, jj, dim)].green +
              (int)src[RIDX(ii, jj, dim)].blue) / 3;

          dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].green = ((int)src[RIDX(ii, jj, dim)].red +
              (int)src[RIDX(ii, jj, dim)].green +
              (int)src[RIDX(ii, jj, dim)].blue) / 3;

          dest[RIDX(dim - jj - 1, dim - ii - 1, dim)].blue = ((int)src[RIDX(ii, jj, dim)].red +
              (int)src[RIDX(ii, jj, dim)].green +
              (int)src[RIDX(ii, jj, dim)].blue) / 3;

        }
      }
    }
}

char tile32_complex_descr[] = "tile32_complex: tile the loop of the naive implementation with w=32";
void tile32_complex(int dim, pixel *src, pixel *dest)
{
  tileAdjustable_complex(dim, src, dest, 32);
}

char tile16_complex_descr[] = "tile16_complex: tile the loop of the naive implementation with w=16";
void tile16_complex(int dim, pixel *src, pixel *dest)
{
  tileAdjustable_complex(dim, src, dest, 16);
}

char tile08_complex_descr[] = "tile08_complex: tile the loop of the naive implementation with w=08";
void tile08_complex(int dim, pixel *src, pixel *dest)
{
  tileAdjustable_complex(dim, src, dest, 8);
}

char tile04_complex_descr[] = "tile04_complex: tile the loop of the naive implementation with w=04";
void tile04_complex(int dim, pixel *src, pixel *dest)
{
  tileAdjustable_complex(dim, src, dest, 4);
}



/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  precalculate_complex(dim, src, dest);
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
  // add_complex_function(&precalculate_complex, precalculate_complex_descr);
  // add_complex_function(&tile32_complex, tile32_complex_descr);
  // add_complex_function(&tile16_complex, tile16_complex_descr);
  // add_complex_function(&tile08_complex, tile08_complex_descr);
  // add_complex_function(&tile04_complex, tile04_complex_descr);
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
  add_motion_function(&naive_motion, naive_motion_descr);
}
