/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "liangke",              /* Team name */

    "liangke",     /* First member full name */
    "lk",  /* First member email address */

    "liangke",                   /* Second member full name (leave blank if none) */
    "2504095682@qq.com"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/


/* 
 * naive_rotate - The naive baseline version of rotate 
 */

char naive_rotate_descr[] ="naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src,pixel *dst)
{
   int i, j;
 
   for (i = 0; i < dim; i++)
         for(j = 0; j < dim; j++)
             dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j,dim)];
}//第一处编写


/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
//宏定义一个复制函数，方便程序编写
#define COPY(d,s) *(d)=*(s)
char rotate_descr[] = "rotate: Currentworking version";

void rotate( int dim,pixel *src,pixel *dst)
{
   int i,j;
   for(i=0;i<dim;i+=32)//32路循环展开，32个像素依次存储
         for(j=dim-1;j>=0;j-=1)
{       
         pixel*dptr=dst+RIDX(dim-1-j,i,dim);
         pixel*sptr=src+RIDX(i,j,dim);
//进行复制操作
         COPY(dptr,sptr);sptr+=dim;
         COPY(dptr+1,sptr);sptr+=dim;
         COPY(dptr+2,sptr);sptr+=dim;
         COPY(dptr+3,sptr);sptr+=dim;
         COPY(dptr+4,sptr);sptr+=dim;
         COPY(dptr+5,sptr);sptr+=dim;
         COPY(dptr+6,sptr);sptr+=dim;
         COPY(dptr+7,sptr);sptr+=dim;
         COPY(dptr+8,sptr);sptr+=dim;
         COPY(dptr+9,sptr);sptr+=dim;
         COPY(dptr+10,sptr);sptr+=dim;
         COPY(dptr+11,sptr);sptr+=dim;
         COPY(dptr+12,sptr);sptr+=dim;
         COPY(dptr+13,sptr);sptr+=dim;
         COPY(dptr+14,sptr);sptr+=dim;
         COPY(dptr+15,sptr);sptr+=dim;
         COPY(dptr+16,sptr);sptr+=dim;
         COPY(dptr+17,sptr);sptr+=dim;
         COPY(dptr+18,sptr);sptr+=dim;
         COPY(dptr+19,sptr);sptr+=dim;
         COPY(dptr+20,sptr);sptr+=dim;
         COPY(dptr+21,sptr);sptr+=dim;
         COPY(dptr+22,sptr);sptr+=dim;
         COPY(dptr+23,sptr);sptr+=dim;
         COPY(dptr+24,sptr);sptr+=dim;
         COPY(dptr+25,sptr);sptr+=dim;
         COPY(dptr+26,sptr);sptr+=dim;
         COPY(dptr+27,sptr);sptr+=dim;
         COPY(dptr+28,sptr);sptr+=dim;
         COPY(dptr+29,sptr);sptr+=dim;
         COPY(dptr+30,sptr);sptr+=dim;
         COPY(dptr+31,sptr);
}
}//第二处编写

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] ="naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src,pixel *dst)
{
   int i, j;
 
   for (i = 0; i < dim; i++)
   for (j = 0; j < dim; j++)
       dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}//第三处编写

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
//第四处编写
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i,j;
    //no using avg()

    //corners
//the top left corner
    dst[RIDX(0,0,dim)].red=(src[RIDX(0,0,dim)].red+src[RIDX(1,0,dim)].red+src[RIDX(0,1,dim)].red+src[RIDX(1,1,dim)].red)>>2;
    dst[RIDX(0,0,dim)].blue=(src[RIDX(0,0,dim)].blue+src[RIDX(1,0,dim)].blue+src[RIDX(0,1,dim)].blue+src[RIDX(1,1,dim)].blue)>>2;
    dst[RIDX(0,0,dim)].green=(src[RIDX(0,0,dim)].green+src[RIDX(1,0,dim)].green+src[RIDX(0,1,dim)].green+src[RIDX(1,1,dim)].green)>>2;
//the top right corner
    dst[RIDX(0,dim-1,dim)].red=(src[RIDX(0,dim-1,dim)].red+src[RIDX(1,dim-1,dim)].red+src[RIDX(0,dim-2,dim)].red+src[RIDX(1,dim-2,dim)].red)>>2;
    dst[RIDX(0,dim-1,dim)].blue=(src[RIDX(0,dim-1,dim)].blue+src[RIDX(1,dim-1,dim)].blue+src[RIDX(0,dim-2,dim)].blue+src[RIDX(1,dim-2,dim)].blue)>>2;
    dst[RIDX(0,dim-1,dim)].green=(src[RIDX(0,dim-1,dim)].green+src[RIDX(1,dim-1,dim)].green+src[RIDX(0,dim-2,dim)].green+src[RIDX(1,dim-2,dim)].green)>>2;
//the bottom left corner
    dst[RIDX(dim-1,0,dim)].red=(src[RIDX(dim-1,0,dim)].red+src[RIDX(dim-2,0,dim)].red+src[RIDX(dim-1,1,dim)].red+src[RIDX(dim-2,1,dim)].red)>>2;
    dst[RIDX(dim-1,0,dim)].blue=(src[RIDX(dim-1,0,dim)].blue+src[RIDX(dim-2,0,dim)].blue+src[RIDX(dim-1,1,dim)].blue+src[RIDX(dim-2,1,dim)].blue)>>2;
    dst[RIDX(dim-1,0,dim)].green=(src[RIDX(dim-1,0,dim)].green+src[RIDX(dim-2,0,dim)].green+src[RIDX(dim-1,1,dim)].green+src[RIDX(dim-2,1,dim)].green)>>2;
//the bottom right corner
    dst[RIDX(dim-1,dim-1,dim)].red=(src[RIDX(dim-1,dim-1,dim)].red+src[RIDX(dim-1,dim-2,dim)].red+src[RIDX(dim-2,dim-1,dim)].red+src[RIDX(dim-2,dim-2,dim)].red)>>2;
    dst[RIDX(dim-1,dim-1,dim)].blue=(src[RIDX(dim-1,dim-1,dim)].blue+src[RIDX(dim-1,dim-2,dim)].blue+src[RIDX(dim-2,dim-1,dim)].blue+src[RIDX(dim-2,dim-2,dim)].blue)>>2;
    dst[RIDX(dim-1,dim-1,dim)].green=(src[RIDX(dim-1,dim-1,dim)].green+src[RIDX(dim-1,dim-2,dim)].green+src[RIDX(dim-2,dim-1,dim)].green+src[RIDX(dim-2,dim-2,dim)].green)>>2;
    
	//boarder
//left
    for(i=1;i<dim-1;i++){
        dst[RIDX(i,0,dim)].red=(src[RIDX(i,0,dim)].red+src[RIDX(i-1,0,dim)].red+src[RIDX(i-1,1,dim)].red+src[RIDX(i,1,dim)].red+src[RIDX(i+1,0,dim)].red+src[RIDX(i+1,1,dim)].red)/6;
        dst[RIDX(i,0,dim)].blue=(src[RIDX(i,0,dim)].blue+src[RIDX(i-1,0,dim)].blue+src[RIDX(i-1,1,dim)].blue+src[RIDX(i,1,dim)].blue+src[RIDX(i+1,0,dim)].blue+src[RIDX(i+1,1,dim)].blue)/6;
        dst[RIDX(i,0,dim)].green=(src[RIDX(i,0,dim)].green+src[RIDX(i-1,0,dim)].green+src[RIDX(i-1,1,dim)].green+src[RIDX(i,1,dim)].green+src[RIDX(i+1,0,dim)].green+src[RIDX(i+1,1,dim)].green)/6;
    }
//right
    for(i=1;i<dim-1;i++){
        dst[RIDX(i,dim-1,dim)].red=(src[RIDX(i,dim-1,dim)].red+src[RIDX(i-1,dim-1,dim)].red+src[RIDX(i-1,dim-2,dim)].red+src[RIDX(i,dim-2,dim)].red+src[RIDX(i+1,dim-1,dim)].red+src[RIDX(i+1,dim-2,dim)].red)/6;
        dst[RIDX(i,dim-1,dim)].blue=(src[RIDX(i,dim-1,dim)].blue+src[RIDX(i-1,dim-1,dim)].blue+src[RIDX(i-1,dim-2,dim)].blue+src[RIDX(i,dim-2,dim)].blue+src[RIDX(i+1,dim-1,dim)].blue+src[RIDX(i
		+1,dim-2,dim)].blue)/6;
        dst[RIDX(i,dim-1,dim)].green=(src[RIDX(i,dim-1,dim)].green+src[RIDX(i-1,dim-1,dim)].green+src[RIDX(i-1,dim-2,dim)].green+src[RIDX(i,dim-2,dim)].green+src[RIDX(i+1,dim-1,dim)].green+src[RIDX(
		i+1,dim-2,dim)].green)/6;
    }
//up
    for(j=1;j<dim-1;j++){
        dst[RIDX(0,j,dim)].red=(src[RIDX(0,j,dim)].red+src[RIDX(0,j-1,dim)].red+src[RIDX(1,j-1,dim)].red+src[RIDX(1,j,dim)].red+src[RIDX(0,j+1,dim)].red+src[RIDX(1,j+1,dim)].red)/6;
        dst[RIDX(0,j,dim)].blue=(src[RIDX(0,j,dim)].blue+src[RIDX(0,j-1,dim)].blue+src[RIDX(1,j-1,dim)].blue+src[RIDX(1,j,dim)].blue+src[RIDX(0,j+1,dim)].blue+src[RIDX(1,j+1,dim)].blue)/6;
        dst[RIDX(0,j,dim)].green=(src[RIDX(0,j,dim)].green+src[RIDX(0,j-1,dim)].green+src[RIDX(1,j-1,dim)].green+src[RIDX(1,j,dim)].green+src[RIDX(0,j+1,dim)].green+src[RIDX(1,j+1,dim)].green)/6;
    }

//down
    for(j=1;j<dim-1;j++){
        dst[RIDX(dim-1,j,dim)].red=(src[RIDX(dim-1,j,dim)].red+src[RIDX(dim-1,j+1,dim)].red+src[RIDX(dim-1,j-1,dim)].red+src[RIDX(dim-2,j,dim)].red+src[RIDX(dim-2,j+1,dim)].red+src[RIDX(dim-2,j-1,dim)].red)/6;
        dst[RIDX(dim-1,j,dim)].blue=(src[RIDX(dim-1,j,dim)].blue+src[RIDX(dim-1,j+1,dim)].blue+src[RIDX(dim-1,j-1,dim)].blue+src[RIDX(dim-2,j,dim)].blue+src[RIDX(dim-2,j+1,dim)].blue+src[RIDX
		(dim-2,j-1,dim)].blue)/6;
        dst[RIDX(dim-1,j,dim)].green=(src[RIDX(dim-1,j,dim)].green+src[RIDX(dim-1,j+1,dim)].green+src[RIDX(dim-1,j-1,dim)].green+src[RIDX(dim-2,j,dim)].green+src[RIDX(dim-2,j+1,dim)].green+src[RIDX
		(dim-2,j-1,dim)].green)/6;
	}

	//common
	for(i=1;i<dim-1;i++)
		for(j=1;j<dim-1;j++){

			dst[RIDX(i,j,dim)].red=(src[RIDX(i,j,dim)].red+src[RIDX(i+1,j,dim)].red+src[RIDX(i-1,j,dim)].red+src[RIDX(i,j-1,dim)].red+src[RIDX(i+1,j-1,dim)].red+src[RIDX(i-1,j-1,dim)].red+src[RIDX(i,j
			+1,dim)].red+src[RIDX(i+1,j+1,dim)].red+src[RIDX(i-1,j+1,dim)].red)/9;

			dst[RIDX(i,j,dim)].blue=(src[RIDX(i,j,dim)].blue+src[RIDX(i+1,j,dim)].blue+src[RIDX(i-1,j,dim)].blue+src[RIDX(i,j-1,dim)].blue+src[RIDX(i+1,j-1,dim)].blue+src[RIDX(i-1,j-1,dim)].blue+src[RIDX(i,
			j+1,dim)].blue+src[RIDX(i+1,j+1,dim)].blue+src[RIDX(i-1,j+1,dim)].blue)/9;

			dst[RIDX(i,j,dim)].green=(src[RIDX(i,j,dim)].green+src[RIDX(i+1,j,dim)].green+src[RIDX(i-1,j,dim)].green+src[RIDX(i,j-1,dim)].green+src[RIDX(i+1,j-1,dim)].green+src[RIDX(i-1,j-1,dim)].green+src
			[RIDX(i,j+1,dim)].green+src[RIDX(i+1,j+1,dim)].green+src[RIDX(i-1,j+1,dim)].green)/9;

		}


}
/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

