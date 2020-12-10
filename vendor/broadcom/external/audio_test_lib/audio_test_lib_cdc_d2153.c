/* audio_test_lib_cdc_d2153.c
 *
 * Copyright (C) 2012-2013 Renesas Mobile Corp.
 * All rights reserved.
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

/*!
  @file		audio_test_lib_cdc_d2153.c

  @brief	Audio test command library for D2153 source file.
*/



/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "audio_test_lib_def.h"

/*---------------------------------------------------------------------------*/
/* typedef declaration (private)                                             */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* define macro declaration (private)                                        */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* define function macro declaration (private)                               */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* enum declaration (private)                                                */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* structure declaration (private)                                           */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* prototype declaration (private)                                           */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* global variable declaration                                               */
/*---------------------------------------------------------------------------*/
/*!
  @brief	Codec table.
*/
const struct audio_test_codec_ctrl audio_test_tbl_clear[] = {
	/* name					element */
	{"ADC Activate",			{  0, 0 } },
	{"DAC Activate",			{  0, 0 } },
};

/*---------------------------------------------------------------------------*/
/* extern variable declaration                                               */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* extern function declaration                                               */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* inline function implementation                                            */
/*---------------------------------------------------------------------------*/
/* none */



/*---------------------------------------------------------------------------*/
/* function implementation                                                   */
/*---------------------------------------------------------------------------*/
/*------------------------------------*/
/* for private function               */
/*------------------------------------*/
/* none */

/*------------------------------------*/
/* for public function                */
/*------------------------------------*/
uint audio_test_get_tbl_clear_array_size(void)
{
	return AUDIO_TEST_ARRAY_SIZE(audio_test_tbl_clear);
}
