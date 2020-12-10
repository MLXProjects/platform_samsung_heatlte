/* audio_test_lib_def.h
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
 *
 */

/*!
  @file		audio_test_lib_def.h

  @brief	Audio test command library define header file.
*/

#ifndef __AUDIO_TEST_LIB_DEF_H__
#define __AUDIO_TEST_LIB_DEF_H__

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include <sys/types.h>
#include <audio_test_lib.h>

/*---------------------------------------------------------------------------*/
/* typedef declaration                                                       */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* define macro declaration                                                  */
/*---------------------------------------------------------------------------*/
/*!
  @brief	Hook switch file name.
*/
#define AUDIO_TEST_HOOKSW_NAME		"\"d2153-aad\""

#define AUDIO_TEST_AMHAL_SOCKET_FNAME	"amhal_test"

#define AUDIO_TEST_PCM_NAME_BM_SP	"AndroidPlayback_pMSMicSpeaker_incall"
#define AUDIO_TEST_PCM_NAME_BM_EP	"AndroidPlayback_pMSMicEarpiece_incall"
#define AUDIO_TEST_PCM_NAME_BM_HP	"AndroidPlayback_pMSMicHeadphone_incall"
#define AUDIO_TEST_PCM_NAME_MM_SP	"AndroidPlayback_pMMicSpeaker_incall"
#define AUDIO_TEST_PCM_NAME_MM_HP	"AndroidPlayback_pMMicHeadphone_incall"
#define AUDIO_TEST_PCM_NAME_MM_EP	"AndroidPlayback_pMMicEarpiece_incall"
#define AUDIO_TEST_PCM_NAME_SM_SP	"AndroidPlayback_pSMicSpeaker_incall"
#define AUDIO_TEST_PCM_NAME_SM_EP	"AndroidPlayback_pSMicEarpiece_incall"
#define AUDIO_TEST_PCM_NAME_SM_HP	"AndroidPlayback_pSMicHeadphone_incall"
#define AUDIO_TEST_PCM_NAME_HM_SP	"AndroidPlayback_pHMicSpeaker_incall"
#define AUDIO_TEST_PCM_NAME_HM_EP	"AndroidPlayback_pHMicEarpiece_incall"
#define AUDIO_TEST_PCM_NAME_HM_HP	"AndroidPlayback_pHMicHeadphone_incall"
#define AUDIO_TEST_PCM_NAME_SP		"AndroidPlayback_Speaker_normal"
#define AUDIO_TEST_PCM_NAME_HP		"AndroidPlayback_Headphone_normal"
#define AUDIO_TEST_PCM_NAME_EP		"AndroidPlayback_Earpiece_normal"


#define AUDIO_TEST_MAX_1LINE_LEN	128
#define AUDIO_TEST_MAX_EVTNUM_LEN	2

#define AUDIO_TEST_LEN_JACK_STATE	2

#define AUDIO_TEST_VAL_JACK_STATE_IN	1
#define AUDIO_TEST_LEN_JACK_STATE_INMIC	2

#define AUDIO_TEST_IFLB_PRM		0x0A
#define AUDIO_TEST_PCMLB_PRM		0x0B
#define AUDIO_TEST_PKTLB_PRM		0x0C
#define AUDIO_TEST_1KHZTONE_PRM		0x10
#define AUDIO_TEST_VOLTENB_PRM		0x11
#define AUDIO_TEST_VOLTEWB_PRM		0x12

#define AUDIO_TEST_EV_KEY		0x01

/*---------------------------------------------------------------------------*/
/* define function macro declaration                                         */
/*---------------------------------------------------------------------------*/
#define AUDIO_TEST_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#define AUDIO_TEST_EVIOCGBIT(ev,len)	_IOC(_IOC_READ, 'E', 0x20 + (ev), len)	/* get event bits */
#define AUDIO_TEST_EVIOCGKEY(len)	_IOC(_IOC_READ, 'E', 0x18, len)		/* get global key state */

/*---------------------------------------------------------------------------*/
/* enum declaration                                                          */
/*---------------------------------------------------------------------------*/
/*!
  @brief	SW gain.
*/
enum audio_test_sw_gain_level {
	AUDIO_TEST_SW_GAIN_LEVEL0,	/**< Level 0 */
	AUDIO_TEST_SW_GAIN_LEVEL1,	/**< Level 1 */
	AUDIO_TEST_SW_GAIN_LEVEL2,	/**< Level 2 */
	AUDIO_TEST_SW_GAIN_LEVEL3,	/**< Level 3 */
	AUDIO_TEST_SW_GAIN_LEVEL4,	/**< Level 4 */
	AUDIO_TEST_SW_GAIN_LEVEL5,	/**< Level 5 */
	AUDIO_TEST_SW_GAIN_MAX,
	AUDIO_TEST_SW_GAIN_NONE
};

/*---------------------------------------------------------------------------*/
/* structure declaration                                                     */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* extern variable declaration                                               */
/*---------------------------------------------------------------------------*/
extern const struct audio_test_codec_ctrl audio_test_tbl_clear[];
extern const struct audio_test_codec_ctrl audio_test_tbl_dac_adc_on[];

/*---------------------------------------------------------------------------*/
/* extern function declaration                                               */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* prototype declaration                                                     */
/*---------------------------------------------------------------------------*/
uint audio_test_get_tbl_clear_array_size(void);
uint audio_test_get_tbl_dac_adc_on_array_size(void);

/*---------------------------------------------------------------------------*/
/* inline function implementation                                            */
/*---------------------------------------------------------------------------*/
/* none */

#endif /* __AUDIO_TEST_LIB_DEF_H__ */
