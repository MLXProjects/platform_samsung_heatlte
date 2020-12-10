/* audio_test_lib.h
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
  @file		audio_test_lib.h

  @brief	Audio test command library header file.
*/

#ifndef __AUDIO_TEST_LIB_H__
#define __AUDIO_TEST_LIB_H__

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* typedef declaration                                                       */
/*---------------------------------------------------------------------------*/
/* none */

#ifndef __AUDIO_TEST_EXTERN_H__
/*---------------------------------------------------------------------------*/
/* enum declaration                                                          */
/*---------------------------------------------------------------------------*/
/*!
  @brief	ON/OFF type.
*/
enum audio_test_state_type {
	AUDIO_TEST_DRV_STATE_OFF,	/**< 0x00: OFF. */
	AUDIO_TEST_DRV_STATE_ON,	/**< 1x01: ON. */
	AUDIO_TEST_DRV_STATE_MAX
};

/*!
  @brief	Input device type.
*/
enum audio_test_in_device_type {
	AUDIO_TEST_DRV_IN_MIC,		/**< 0x00: Mic. */
	AUDIO_TEST_DRV_IN_MAINMIC,	/**< 0x01: Main mic. */
	AUDIO_TEST_DRV_IN_SUBMIC,	/**< 0x02: Sub mic. */
	AUDIO_TEST_DRV_IN_HEADSETMIC,	/**< 0x03: Headset mic. */
	AUDIO_TEST_DRV_IN_MAX,
	AUDIO_TEST_DRV_IN_DEV_NONE = 0xFFFFFFFF
};

/*!
  @brief	Output device type.
*/
enum audio_test_out_device_type {
	AUDIO_TEST_DRV_OUT_SPEAKER,	/**< 0x00: Speaker. */
	AUDIO_TEST_DRV_OUT_HEADPHONE,	/**< 0x01: Headphone. */
	AUDIO_TEST_DRV_OUT_EARPIECE,	/**< 0x02: Earpiece. */
	AUDIO_TEST_DRV_OUT_MAX,
	AUDIO_TEST_DRV_OUT_DEV_NONE = 0xFFFFFFFF
};

/*!
  @brief	Output LR type.
*/
enum audio_test_out_LR_type {
	AUDIO_TEST_DRV_LR_L,		/**< 0x00: Use L. */
	AUDIO_TEST_DRV_LR_R,		/**< 0x01: Use R. */
	AUDIO_TEST_DRV_LR_LR,		/**< 0x02: Use LR. */
	AUDIO_TEST_DRV_LR_MAX
};

/*!
  @brief	FSI Port.
*/
enum audio_test_fsi_port {
	AUDIO_TEST_DRV_FSI_PORTA,	/**< 0x00: Port A. */
	AUDIO_TEST_DRV_FSI_PORTB,	/**< 0x01: Port B. */
	AUDIO_TEST_DRV_FSI_MAX
};
/*!
  @brief	direction.
*/
enum audio_test_pcm_direction {
	AUDIO_TEST_DRV_PCMDIR_PLAYBACK,	/**< 0x00: Playback. */
	AUDIO_TEST_DRV_PCMDIR_CAPTURE,	/**< 0x01: Capture. */
	AUDIO_TEST_DRV_PCMDIR_MAX
};

/*!
  @brief	PCM status.
*/
enum audio_test_pcm_state {
	AUDIO_TEST_DRV_PCMSTATE_OPEN,	/**< 0x00: PCM open. */
	AUDIO_TEST_DRV_PCMSTATE_CLOSE,	/**< 0x01: PCM close. */
	AUDIO_TEST_DRV_PCMSTATE_MAX
};
/*!
  @brief	PCM type.
*/
enum audio_test_pcm_type {
	AUDIO_TEST_DRV_PCMTYPE_NORMAL,	/**< 0x00: Normal */
	AUDIO_TEST_DRV_PCMTYPE_PT,	/**< 0x01: PT */
	AUDIO_TEST_DRV_PCMTYPE_MAX
};
#endif

/*!
  @brief	VoLTE type.
*/
enum audio_test_volte_type {
	AUDIO_TEST_VOLTE_NONE = 0,
	AUDIO_TEST_VOLTE_NB,          // VOLTE VE loopback NB_AMR
	AUDIO_TEST_VOLTE_WB,          // VOLTE VE loopback WB_AMR
};

/*!
  @brief	Client type.
*/
enum audio_test_client_type {
	AUDIO_TEST_CLIENT_ALSA = 0,   // To alsa-lib client From ALSA
	AUDIO_TEST_CLIENT_PT,         // To alsa-lib client From PT
	AUDIO_TEST_CLIENT_MAX
};

/*---------------------------------------------------------------------------*/
/* define macro declaration                                                  */
/*---------------------------------------------------------------------------*/
/*!
  @brief	Debug.
*/
/* #define AUDIO_TEST_DEBUG */

/*!
  @brief	Device file.
*/
#define AUDIO_TEST_FILE			"/dev/audio_test"

/*!
  @brief	State type.
*/
#define AUDIO_TEST_STATE_OFF		AUDIO_TEST_DRV_STATE_OFF
	/**< 0: OFF. */
#define AUDIO_TEST_STATE_ON		AUDIO_TEST_DRV_STATE_ON
	/**< 1: ON. */

/*!
  @brief	Input device type.
*/
#define AUDIO_TEST_IN_MIC		AUDIO_TEST_DRV_IN_MIC
	/**< 0x00: Mic. */
#define AUDIO_TEST_IN_MAINMIC		AUDIO_TEST_DRV_IN_MAINMIC
	/**< 0x01: Main Mic. */
#define AUDIO_TEST_IN_SUBMIC		AUDIO_TEST_DRV_IN_SUBMIC
	/**< 0x02: Sub Mic. */
#define AUDIO_TEST_IN_HEADSETMIC	AUDIO_TEST_DRV_IN_HEADSETMIC
	/**< 0x03: Headset mic. */
#define AUDIO_TEST_IN_DEV_NONE		AUDIO_TEST_DRV_IN_DEV_NONE
	/**< 0xFFFFFFFF: Device none. */

/*!
  @brief	Output device type.
*/
#define AUDIO_TEST_OUT_SPEAKER		AUDIO_TEST_DRV_OUT_SPEAKER
	/**< 0x00: Speaker. */
#define AUDIO_TEST_OUT_HEADPHONE	AUDIO_TEST_DRV_OUT_HEADPHONE
	/**< 0x01: Headphone. */
#define AUDIO_TEST_OUT_EARPIECE		AUDIO_TEST_DRV_OUT_EARPIECE
	/**< 0x02: Earpiece. */
#define AUDIO_TEST_OUT_DEV_NONE		AUDIO_TEST_DRV_OUT_DEV_NONE
	/**< 0xFFFFFFFF: Device none. */

/*!
  @brief	LR type.
*/
#define AUDIO_TEST_LR_L			AUDIO_TEST_DRV_LR_L
	/**< 0x00: Use L. */
#define AUDIO_TEST_LR_R			AUDIO_TEST_DRV_LR_R
	/**< 0x01: Use R. */
#define AUDIO_TEST_LR_LR		AUDIO_TEST_DRV_LR_LR
	/**< 0x02: Use LR. */

/*!
  @brief	FSI Port.
*/
#define AUDIO_TEST_FSI_PORTA		AUDIO_TEST_DRV_FSI_PORTA
	/**< 0x00: Port A. */
#define AUDIO_TEST_FSI_PORTB		AUDIO_TEST_DRV_FSI_PORTB
	/**< 0x01: Port B. */

#define AUDIO_TEST_MAX_ELEMENT		2

/*---------------------------------------------------------------------------*/
/* define function macro declaration                                         */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* structure declaration                                                     */
/*---------------------------------------------------------------------------*/
/*!
  @brief	Input device info.
*/
struct audio_test_in_device_info {
	u_int device_type;	/**< Device type */
};

/*!
  @brief	Output device info.
*/
struct audio_test_out_device_info {
	u_int device_type;	/**< Device type */
	u_int LR_type;		/**< LR type */
	u_int volume;		/**< Volume */
};

/*!
  @brief	Codecs control info.
*/
struct audio_test_codec_ctrl {
	const char	*name;
	long		elem_value[AUDIO_TEST_MAX_ELEMENT];
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
/* prototype declaration                                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int audio_test_set_device(struct audio_test_in_device_info *in_device,
			struct audio_test_out_device_info *out_device);
int audio_test_start_loopback(u_int fsi_port);
int audio_test_stop_loopback(void);
int audio_test_detect_jack(u_int *state);
int audio_test_detect_key(u_int *state);
int audio_test_start_tone(void);
int audio_test_stop_tone(void);
int audio_test_start_pcm_loopback(u_int fsi_port, u_int vqa_val,
						u_int delay_val);
int audio_test_stop_pcm_loopback(void);
int audio_test_start_volte_loopback(u_int fsi_port,
					u_int vqa_val,
					u_int delay_val,
					u_int volte_type);
int audio_test_stop_volte_loopback(void);
int audio_test_change_playback_device(
	struct audio_test_out_device_info *out_device,
	u_int state);

/*---------------------------------------------------------------------------*/
/* inline function implementation                                            */
/*---------------------------------------------------------------------------*/
/* none */

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_TEST_LIB_H__ */
