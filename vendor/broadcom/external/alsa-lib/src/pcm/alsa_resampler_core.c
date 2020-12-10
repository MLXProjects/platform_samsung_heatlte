/* alsa_resampler_core.c
 *
 * Copyright (C) 2013 Renesas Mobile Corp.
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/* #define RESAMP_WINDOWS */// REmove this when testing on board and

#ifdef RESAMP_WINDOWS
#include <stdlib.h>
#include "main.h"
#else

#endif

#include <stdlib.h>
#include "alsa_resampler.h"


#define SH_RESAMPLER_MAX_S32 (2147483647)
#define SH_RESAMPLER_MIN_S32 (-2147483647-1)
#define SH_RESAMPLER_MAX_S16 (32767)
#define SH_RESAMPLER_MIN_S16 (-32767-1)


#define DOWNSAMPLE_6 6
#define DOWNSAMPLE_3 3
#define DOWNSAMPLE_2 2

/* Filter size definition:
!!!! Update the FILTER_MAX_LG accordingly !!! */
#define FILTERx6 969
#define FILTERx2 353
#define FILTERx3 32
#define FILTER_MAX_LG FILTERx6


/* Filters taps */
short x6[FILTERx6] = {
8, -19, -31, -50, -73, -96, -118, -134, -141, -138, -124, -99, -68, -32, 2, 31, 51, 59, 56, 42, 22, 0, -21, -35, -41, -38, -27, -11, 6, 21, 30, 32, 26, 15, 1, -13, -22, -27, -24, -16, -5, 7, 17, 23, 22, 17, 7, -4, -13, -20, -21, -17, -9, 1, 11, 17, 19, 16, 10, 0, -9, -16, -18, -16, -10, -2, 7, 14, 17, 16, 11, 3, -6, -13, -17, -16, -11, -4, 5, 12, 16, 16, 12, 5, -4, -11, -16, -16, -12, -5, 3, 11, 15, 16, 13, 6, -2, -10, -15, -17, -14, -7, 1, 9, 15, 17, 14, 8, 0, -9, -15, -17, -15, -9, 0, 8, 15, 17, 16, 10, 1, -7, -14, -18, -16, -11, -2, 7, 14, 18, 17, 12, 3, -6, -14, -18, -18, -13, -4, 5, 14, 19, 19, 14, 6, -5, -13, -19, -20, -15, -7, 4, 13, 19, 20, 16, 8, -3, -13, -19, -21, -18, -9, 2, 12, 20, 22, 19, 11, 0, -12, -20, -23, -20, -12, -1, 11, 20, 24, 21, 13, 2, -10, -20, -24, -23, -15, -3, 9, 20, 25, 24, 17, 5, -8, -19, -26, -25, -18, -6, 7, 19, 26, 26, 20, 8, -6, -19, -27, -28, -22, -10, 5, 18, 27, 29, 23, 12, -3, -18, -27, -30, -25, -14, 2, 17, 28, 32, 27, 16, 0, -16, -28, -33, -29, -18, -1, 15, 28, 34, 31, 20, 3, -14, -28, -35, -33, -22, -5, 13, 28, 36, 35, 24, 8, -12, -28, -37, -37, -27, -10, 10, 28, 38, 39, 29, 12, -8, -27, -39, -41, -32, -15, 7, 27, 40, 43, 35, 18, -5, -26, -41, -45, -38, -20, 2, 25, 41, 47, 41, 24, 0, -24, -42, -49, -44, -27, -3, 23, 42, 51, 47, 30, 6, -21, -42, -53, -50, -34, -9, 19, 43, 55, 54, 38, 12, -17, -43, -57, -57, -42, -15, 15, 43, 59, 61, 46, 19, -13, -42, -61, -64, -51, -24, 10, 42, 63, 68, 55, 28, -7, -41, -65, -72, -61, -33, 4, 40, 67, 76, 66, 39, 0, -39, -68, -81, -72, -44, -5, 37, 70, 85, 78, 51, 9, -35, -72, -90, -85, -58, -15, 33, 73, 95, 92, 66, 21, -30, -75, -100, -100, -74, -28, 27, 76, 106, 109, 84, 35, -23, -77, -113, -119, -94, -44, 19, 79, 120, 130, 107, 54, -14, -80, -127, -143, -121, -66, 7, 81, 136, 157, 137, 80, 1, -82, -146, -174, -156, -97, -10, 83, 158, 194, 180, 118, 22, -84, -172, -218, -209, -143, -37, 85, 190, 250, 246, 177, 57, -85, -213, -291, -296, -222, -84, 86, 245, 349, 367, 288, 125, -86, -292, -438, -478, -392, -190, 87, 372, 591, 675, 584, 315, -87, -538, -925, -1132, -1060, -653, 87, 1097, 2255, 3406, 4382, 5035, 5265, 5035, 4382, 3406, 2255, 1097, 87, -653, -1060, -1132, -925, -538, -87, 315, 584, 675, 591, 372, 87, -190, -392, -478, -438, -292, -86, 125, 288, 367, 349, 245, 86, -84, -222, -296, -291, -213, -85, 57, 177, 246, 250, 190, 85, -37, -143, -209, -218, -172, -84, 22, 118, 180, 194, 158, 83, -10, -97, -156, -174, -146, -82, 1, 80, 137, 157, 136, 81, 7, -66, -121, -143, -127, -80, -14, 54, 107, 130, 120, 79, 19, -44, -94, -119, -113, -77, -23, 35, 84, 109, 106, 76, 27, -28, -74, -100, -100, -75, -30, 21, 66, 92, 95, 73, 33, -15, -58, -85, -90, -72, -35, 9, 51, 78, 85, 70, 37, -5, -44, -72, -81, -68, -39, 0, 39, 66, 76, 67, 40, 4, -33, -61, -72, -65, -41, -7, 28, 55, 68, 63, 42, 10, -24, -51, -64, -61, -42, -13, 19, 46, 61, 59, 43, 15, -15, -42, -57, -57, -43, -17, 12, 38, 54, 55, 43, 19, -9, -34, -50, -53, -42, -21, 6, 30, 47, 51, 42, 23, -3, -27, -44, -49, -42, -24, 0, 24, 41, 47, 41, 25, 2, -20, -38, -45, -41, -26, -5, 18, 35, 43, 40, 27, 7, -15, -32, -41, -39, -27, -8, 12, 29, 39, 38, 28, 10, -10, -27, -37, -37, -28, -12, 8, 24, 35, 36, 28, 13, -5, -22, -33, -35, -28, -14, 3, 20, 31, 34, 28, 15, -1, -18, -29, -33, -28, -16, 0, 16, 27, 32, 28, 17, 2, -14, -25, -30, -27, -18, -3, 12, 23, 29, 27, 18, 5, -10, -22, -28, -27, -19, -6, 8, 20, 26, 26, 19, 7, -6, -18, -25, -26, -19, -8, 5, 17, 24, 25, 20, 9, -3, -15, -23, -24, -20, -10, 2, 13, 21, 24, 20, 11, -1, -12, -20, -23, -20, -12, 0, 11, 19, 22, 20, 12, 2, -9, -18, -21, -19, -13, -3, 8, 16, 20, 19, 13, 4, -7, -15, -20, -19, -13, -5, 6, 14, 19, 19, 14, 5, -4, -13, -18, -18, -14, -6, 3, 12, 17, 18, 14, 7, -2, -11, -16, -18, -14, -7, 1, 10, 16, 17, 15, 8, 0, -9, -15, -17, -15, -9, 0, 8, 14, 17, 15, 9, 1, -7, -14, -17, -15, -10, -2, 6, 13, 16, 15, 11, 3, -5, -12, -16, -16, -11, -4, 5, 12, 16, 16, 12, 5, -4, -11, -16, -17, -13, -6, 3, 11, 16, 17, 14, 7, -2, -10, -16, -18, -16, -9, 0, 10, 16, 19, 17, 11, 1, -9, -17, -21, -20, -13, -4, 7, 17, 22, 23, 17, 7, -5, -16, -24, -27, -22, -13, 1, 15, 26, 32, 30, 21, 6, -11, -27, -38, -41, -35, -21, 0, 22, 42, 56, 59, 51, 31, 2, -32, -68, -99, -124, -138, -141, -134, -118, -96, -73, -50, -31, -19, 8};

/* Filters taps */
short x2[FILTERx2] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -1, 0, 1, 0, -1, -1, 1, 1, -1, -1, 1, 2, -1, -2, 1, 2, -1, -3, 0, 3, 0, -4, -1, 4, 1, -4, -2, 5, 3, -5, -4, 5, 5, -5, -6, 5, 8, -4, -9, 4, 11, -3, -12, 2, 14, 0, -15, -2, 16, 4, -18, -7, 18, 10, -19, -13, 19, 17, -19, -21, 18, 25, -16, -30, 14, 34, -11, -39, 7, 43, -2, -47, -3, 50, 10, -53, -18, 56, 26, -57, -35, 57, 45, -56, -56, 53, 67, -49, -78, 43, 90, -35, -101, 25, 112, -13, -122, -1, 131, 18, -139, -36, 145, 57, -149, -80, 151, 106, -150, -133, 145, 161, -136, -192, 124, 223, -106, -256, 83, 289, -54, -322, 19, 356, 24, -389, -76, 421, 138, -452, -212, 481, 301, -508, -411, 533, 548, -556, -726, 576, 970, -592, -1335, 605, 1964, -614, -3381, 620, 10306, 15598, 10306, 620, -3381, -614, 1964, 605, -1335, -592, 970, 576, -726, -556, 548, 533, -411, -508, 301, 481, -212, -452, 138, 421, -76, -389, 24, 356, 19, -322, -54, 289, 83, -256, -106, 223, 124, -192, -136, 161, 145, -133, -150, 106, 151, -80, -149, 57, 145, -36, -139, 18, 131, -1, -122, -13, 112, 25, -101, -35, 90, 43, -78, -49, 67, 53, -56, -56, 45, 57, -35, -57, 26, 56, -18, -53, 10, 50, -3, -47, -2, 43, 7, -39, -11, 34, 14, -30, -16, 25, 18, -21, -19, 17, 19, -13, -19, 10, 18, -7, -18, 4, 16, -2, -15, 0, 14, 2, -12, -3, 11, 4, -9, -4, 8, 5, -6, -5, 5, 5, -4, -5, 3, 5, -2, -4, 1, 4, -1, -4, 0, 3, 0, -3, -1, 2, 1, -2, -1, 2, 1, -1, -1, 1, 1, -1, -1, 0, 1, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

short x3[FILTERx3] = {
-41, -91, -103, -18, 174, 378, 416, 130, -467, -1098, -1282, -560, 1219, 3714, 6158, 7672, 7672, 6158, 3714, 1219, -560, -1282, -1098, -467, 130, 416, 378, 174, -18, -103, -91, -41};

/* FIR data */
int		rmc_resampler_ptrdelay1_p;
short  *rmc_resampler_pdelaystart1_p;

short	rmc_resampler_delay1_ap[2*(FILTER_MAX_LG+1)];

int		rmc_resampler_ptrdelay2_p;
short  *rmc_resampler_pdelaystart2_p;

short	rmc_resampler_delay2_ap[2*(FILTER_MAX_LG+1)];

/* Intermediate buffers */
short *rmc_resampler_in_stage_1 = NULL;
short *rmc_resampler_in_stage_2 = NULL;
short *rmc_resampler_in_stage_3 = NULL;


int rmc_resampler_init(
		int input_buffer
		)
{
	/* Init the FIR stuctures */
	rmc_resampler_fir_init();

	rmc_resampler_close();

	/* Allocation of the intermediate buffers */
	rmc_resampler_in_stage_1 = malloc(input_buffer*sizeof(short));
	if(rmc_resampler_in_stage_1 == NULL){
		rmc_resampler_close();
		return RMC_RESAMPLER_ERR_1;
	}
    rmc_resampler_in_stage_2 = malloc(input_buffer*sizeof(short));
	if(rmc_resampler_in_stage_2 == NULL){
		rmc_resampler_close();
		return RMC_RESAMPLER_ERR_1;
	}
	rmc_resampler_in_stage_3 = malloc(input_buffer*sizeof(short));
	if(rmc_resampler_in_stage_3 == NULL){
		rmc_resampler_close();
		return RMC_RESAMPLER_ERR_1;
	}

	return RMC_RESAMPLER_NO_ERR;
}

int rmc_resampler_close(void)
{

	/* Downsampling */
	if (rmc_resampler_in_stage_1 != NULL)
		free(rmc_resampler_in_stage_1);

    if (rmc_resampler_in_stage_2 != NULL)
		free(rmc_resampler_in_stage_2);

    if (rmc_resampler_in_stage_3 != NULL)
		free(rmc_resampler_in_stage_3);
	
	rmc_resampler_in_stage_1 = rmc_resampler_in_stage_2 = rmc_resampler_in_stage_3 = NULL;
	
	return RMC_RESAMPLER_NO_ERR;
}

int rmc_resample(   
		short *output_buffer,
		short *input_buffer,
		int input_buffer_size,
		int mode
		)
{

	int i;	
	int error=0;


	switch (mode) {

    case RMC_SRC6_M:		/*  */
		if (omxSP_FIR_Direct_S16_poly(
				input_buffer,
				rmc_resampler_in_stage_1,
				input_buffer_size,
				&x3[0],
				FILTERx3,
				rmc_resampler_pdelaystart1_p,
				&rmc_resampler_ptrdelay1_p, 0, 1) != 0) {
			error = RMC_RESAMPLER_ERR_1;
		}

        /* Downsampling of 3 */
		for (i = 0; i < input_buffer_size/DOWNSAMPLE_3; i++)
			rmc_resampler_in_stage_1[i] = rmc_resampler_in_stage_1[i*DOWNSAMPLE_3];

        if (omxSP_FIR_Direct_S16_poly(
				rmc_resampler_in_stage_1,
				rmc_resampler_in_stage_2,
				input_buffer_size/DOWNSAMPLE_3,
				&x2[0],
				FILTERx2,
				rmc_resampler_pdelaystart2_p,
				&rmc_resampler_ptrdelay2_p, 0, 1) != 0) {
			error = RMC_RESAMPLER_ERR_1;
		}
		/* Downsampling of 2 */
		for (i = 0; i < input_buffer_size/(DOWNSAMPLE_2*DOWNSAMPLE_3); i++)
			output_buffer[i] = rmc_resampler_in_stage_2[i*DOWNSAMPLE_2];

		break;


	default:
		/* Exit code */
			break;
	}

	return error;
}



int rmc_resampler_fir_init(void)
{
	int i;


	rmc_resampler_ptrdelay1_p	    = 0;
	rmc_resampler_pdelaystart1_p	= &rmc_resampler_delay1_ap[0];

    rmc_resampler_ptrdelay2_p	    = 0;
	rmc_resampler_pdelaystart2_p	= &rmc_resampler_delay2_ap[0];


	for (i = 0; i < 2*(FILTER_MAX_LG+1); i++)	{
		rmc_resampler_pdelaystart1_p[i] = 0;
        rmc_resampler_pdelaystart2_p[i] = 0;
	}

	return 0;
}




int omxSP_FIR_Direct_S16_poly(
	const short *pSrc,
	short *pDst,
	int sampLen,
	const short *pTapsQ15,
	int tapsLen,
	short *pDelayLine,
	int *pDelayLineIndex,
	int offset,
	int step
)
{
	int  Count;
	int  Result = 0;

	int index;
	unsigned long offset_loc;
	long accum;
	short *pDelayCurrent;


	for (Count = 0; Count < (int)sampLen; Count++)	{
		/* Input parameter check */
		/* Update local pos of the offset */
		offset_loc = Count%step;

		/* Update the delay state */
		pDelayCurrent = &pDelayLine[*pDelayLineIndex];

		/* Copy input to current delay line position */
		pDelayCurrent[0] = pDelayCurrent[tapsLen] = pSrc[Count];

		accum = 0;
		for (index = offset_loc; index < (int) tapsLen; index += step)
			accum += (long)pTapsQ15[index] * pDelayCurrent[index];

		if (--(*pDelayLineIndex) < 0)
			*pDelayLineIndex = tapsLen - 1;


		/* Store the result */
		accum = armSatAdd_S32(accum, 16384);
		*(pDst+Count) = (short)(accum >> 15);

	}

	return 0;
}


/**
 * Function:  omxSP_FIROne_Direct_S16   (2.2.3.1.2)
 *
 * Description:
 * Single-sample FIR filtering for 16-bit data type. These functions apply
 * the FIR filter defined by the coefficient vector pTapsQ15 to a single
 * sample of input data. The result is saturated with rounding if the
 * operation produces a value outside the range of a signed 16-bit integer.
 * Rounding behavior is defined in:
 *       section 1.6.7 "Integer Scaling and Rounding Conventions".
 * The internal accumulator width must be at least 32 bits.  The result is
 * undefined if any of the partially accumulated values exceeds the range of a
 * signed 32-bit integer.
 *
 * Input Arguments:
 *
 *   val      - the single input sample to which the filter is
 *            applied.
 *   pTapsQ15 - pointer to the vector that contains the filter coefficients,
 *            represented in Q0.15 format (as defined in section 1.6.5). Given
 *            that:
 *                    -32768 = pTapsQ15(k) < 32768,
 *                         0 = k < tapsLen,
 *            the range on the actual filter coefficients is -1 = bK <1, and
 *            therefore coefficient normalization may be required during the
 *            filter design process.
 *   tapsLen - the number of taps, or, equivalently, the filter order + 1
 *   pDelayLine - pointer to the 2.tapsLen -element filter memory buffer
 *            (state). The user is responsible for allocation, initialization,
 *            and de-allocation. The filter memory elements are initialized to
 *            zero in most applications.
 *   pDelayLineIndex - pointer to the filter memory index that is maintained
 *            internally by the function. The user should initialize the value
 *            of this index to zero.
 *
 * Output Arguments:
 *
 *   pResult - pointer to the filtered output sample
 *
 * Return Value:
 *
 *    OMX_Sts_NoErr - no error
 *    OMX_Sts_BadArgErr - bad arguments; returned if one or more of the
 *              following is true:
 *    -    One or more of the following pointers is NULL:
 *            -  pResult,
 *            -  pTapsQ15,
 *            -  pDelayLine, or
 *            -  pDelayLineIndex
 *    -    tapslen < 1
 *    -    *pDelayLineIndex < 0 or *pDelayLineIndex >= (2 * tapslen)
 *
 */

int omxSP_FIROne_Direct_S16_poly(
		short val,
		short *pResult,
		const short *pTapsQ15,
		int tapsLen,
		short *pDelayLine,
		int *pDelayLineIndex,
		int offset,
		int step
	)
	{
	unsigned long index;
	unsigned long offset_loc;
	long accum;
	short *pDelayCurrent;

	/* Input parameter check */
	/* Update local pos of the offset */
	offset_loc = offset%step;

	/* Update the delay state */
	pDelayCurrent = &pDelayLine[*pDelayLineIndex];

	/* Copy input to current delay line position */
	pDelayCurrent[0] = pDelayCurrent[tapsLen] = val;

	accum = 0;
	for (index = offset_loc; index < (unsigned long)tapsLen; index += step)
		accum += (long)pTapsQ15[index] * (long)pDelayCurrent[index];

	if (--(*pDelayLineIndex) < 0)
		*pDelayLineIndex = tapsLen - 1;


	/* Store the result */
	*pResult = (short)armSatRoundLeftShift_S32(accum, -15);
	return 0;
}



/**
 * Function :armSatRoundLeftShift_S32()
 *
 * Description :
 *     Returns the result of saturating left-shift operation on input
 *     Or rounded Right shift if the input Shift is negative.
 *
 * Parametrs:
 * [in] Value        Operand
 * [in] Shift        Operand for shift operation
 *
 * Return:
 * [out]             Result of operation
 *
 **/

long armSatRoundLeftShift_S32(long Value, int Shift)
{
	int i;

	if (Shift < 0)    {
		Shift = -Shift;
		Value = armSatAdd_S32(Value, (1 << (Shift - 1)));
		Value = Value >> Shift;
	}	else	{
	for (i = 0; i < Shift; i++)
		Value = armSatAdd_S32(Value, Value);
	}
	return Value;
}

/***********************************************************************/
	/* Saturating Arithmetic operations */

/**
 * Function :armSatAdd_S32()
 *
 * Description :
 *   Returns the result of saturated addition of the two inputs Value1, Value2
 *
 * Parametrs:
 * [in] Value1       First Operand
 * [in] Value2       Second Operand
 *
 * Return:
 * [out]             Result of operation
 *
 *
 **/

long armSatAdd_S32(long Value1, long Value2)
{
	long Result;

	Result = Value1 + Value2;

	if ((Value1^Value2) >= 0)	{
		/*Same sign*/
	if ((Result^Value1) >= 0)	{
		/*Result has not saturated*/
		return	Result;
	}	else	{
	if (Value1 >= 0)	{
		/*Result has saturated in positive side*/
		return	SH_RESAMPLER_MAX_S32;
	}	else	{
	/*Result has saturated in negative side*/
	return	SH_RESAMPLER_MIN_S32;
	}
	}
	}	else	{
	return	Result;
	}

}
