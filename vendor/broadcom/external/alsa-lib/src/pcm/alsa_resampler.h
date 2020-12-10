/* alsa_resampler.h
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
 *
 */

#ifndef SH_RESAMPLER_H_
#define SH_RESAMPLER_H_


/*
 * prototype declaration
 */

/**
 * @brief	initialization function.
 *
 * @param	i_buffer_size	iniput buffer size
 *
 * @retval	0 if success.
 */
int rmc_resampler_init(int i_buffer_size);

/**
 * @brief	close function.
 *
 * @retval	0 if success.
 */
int rmc_resampler_close(void);

/**
 * @brief	resampler work function.
 *
 * @param	output_buffer	pointer to output buffer.
 * @param	input_buffer	pointer to input buffer.
 * @param	input_buffer_size	number of sample available in output buffer
 * @param	mode of resampling
 *
 * @retval	number of samples available in buffer_out.
 */
int rmc_resample(
		short *output_buffer,
		short *input_buffer,
		int input_buffer_size,
		int mode);

/*
 * define macro declaration
 */
/* High performance */
#define RMC_SRC6 0
#define RMC_SRC6_M 1
#define RMC_SRC6_L 2






/* Error management */
#define RMC_RESAMPLER_ERR_BASE 0
#define RMC_RESAMPLER_NO_ERR 0
#define RMC_RESAMPLER_ERR_1 (RMC_RESAMPLER_ERR_BASE-1)



/* Other functions */
int rmc_resampler_fir_init(
		void
		);
short *rmc_resampler_cyclic_add(
		short	*p,
		int offs,
		short	*start,
		int len
		);
void rmc_resampler_zeropadd(
		short *sample_in,
		short *sample_out,
		int length_in,
		int length_out,
		int rate
		);
int omxSP_FIROne_Direct_S16_poly(
		short val,
		short *pResult,
		const short	*pTapsQ15,
		int tapsLen,
		short *pDelayLine,
		int *pDelayLineIndex,
		int offset,
		int step
		);
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
		);
long armSatRoundLeftShift_S32(
		long Value,
		int Shift
		);
long armSatAdd_S32(
		long Value1,
		long Value2
		);




#endif /* SH_RESAMPLER_H_ */
