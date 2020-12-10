/* audio_test_lib_wrap.c
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
  @file		audio_test_lib_wrap.c

  @brief	Audio test command library wrapper source file.
*/



/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <cutils/log.h>

#include <audio_test_lib.h>
#include "audio_test_lib_wrap.h"

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
/* none */

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
/*!
  @brief	Setup input device and output device.

  @param	in_device [i] Input device info.
  @param	out_device [i] Output device info.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_set_device(struct audio_test_in_device_info *in_device,
			struct audio_test_out_device_info *out_device)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_set_device(in_device, out_device, AUDIO_TEST_STATE_ON);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Start SCUW loopback [AudioIC->FSI->SCUW->FSI->AudioIC].

  @param	fsi_port [i] FSI port.
				Port A : AUDIO_TEST_FSI_PORTA.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_start_loopback(u_int fsi_port)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_start_loopback(fsi_port);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop SCUW loopback [AudioIC->FSI->SCUW->FSI->AudioIC].

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_stop_loopback(void)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

    ret = _audio_test_stop_loopback();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Detect jack.

  @param	state [o] Jack state.
			Insert : AUDIO_TEST_STATE_ON.
			Remove : AUDIO_TEST_STATE_OFF.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_detect_jack(u_int *state)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

    ret = _audio_test_detect_jack(state);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Detect key.

  @param	state [o] Key state.
			Push : AUDIO_TEST_STATE_ON.
			Release : AUDIO_TEST_STATE_OFF.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_detect_key(u_int *state)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

    ret = _audio_test_detect_key(state);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Start tone.

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_start_tone(void)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

    ret = _audio_test_start_tone();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop tone.

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_stop_tone(void)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_stop_tone();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Start SPUV loopback.
			[AudioIC->FSI->SCUW->SPUV->SCUW->FSI->AudioIC].

  @param	fsi_port [i] FSI port.
				Port A : AUDIO_TEST_FSI_PORTA.
  @param	vqa_val [i] VQA valid.
				ON : AUDIO_TEST_STATE_ON.
				OFF : AUDIO_TEST_STATE_OFF.
  @param	delay_val [i] Delay valid.
				ON : AUDIO_TEST_STATE_ON.
				OFF : AUDIO_TEST_STATE_OFF.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_start_pcm_loopback(u_int fsi_port, u_int vqa_val,
						u_int delay_val)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_start_pcm_loopback(
			fsi_port, vqa_val,
			delay_val, AUDIO_TEST_VOLTE_NONE);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop SPUV loopback.
			[AudioIC->FSI->SCUW->SPUV->SCUW->FSI->AudioIC].

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_stop_pcm_loopback(void)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

    ret = _audio_test_stop_pcm_loopback();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Setup output device for playing.

  @param	out_device [i] Output device info.
  @param	state [i] Device setting state.
			AUDIO_TEST_STATE_ON.
			AUDIO_TEST_STATE_OFF.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_change_playback_device(
	struct audio_test_out_device_info *out_device,
	u_int state)
{
	int ret = 0;
	struct audio_test_in_device_info in_device;

	ALOGD("%s() in", __func__);

	in_device.device_type = AUDIO_TEST_IN_DEV_NONE;

	ret = _audio_test_set_device(&in_device, out_device, state);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
		goto error;
	}

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}


/*!
  @brief	Start SPUV loopback VoLTE.
			[AudioIC->FSI->SCUW->SPUV->Modem->SPUV->SCUW->FSI->AudioIC].

  @param	fsi_port [i] FSI port.
				Port A : AUDIO_TEST_FSI_PORTA.
  @param	vqa_val [i] VQA valid.
				ON : AUDIO_TEST_STATE_ON.
				OFF : AUDIO_TEST_STATE_OFF.
  @param	delay_val [i] Delay valid.
				ON : AUDIO_TEST_STATE_ON.
				OFF : AUDIO_TEST_STATE_OFF.
  @param	volte_val [i] VoLTE flag.
				0 : AUDIO_TEST_VOLTE_NONE.
				1 : AUDIO_TEST_VOLTE_NB.
				2 : AUDIO_TEST_VOLTE_WB.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_start_volte_loopback(u_int fsi_port,
					u_int vqa_val,
					u_int delay_val,
					u_int volte_type)
{
	int ret = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_start_pcm_loopback(fsi_port, vqa_val,
						delay_val, volte_type);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop SPUV loopback VoLTE.
			[AudioIC->FSI->SCUW->SPUV->SCUW->FSI->AudioIC].

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int audio_test_stop_volte_loopback(void)
{
	int ret = 0;
	int irev = 0;

	ALOGD("%s() in", __func__);

	ret = _audio_test_stop_pcm_loopback();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! Main process", __func__);
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}
