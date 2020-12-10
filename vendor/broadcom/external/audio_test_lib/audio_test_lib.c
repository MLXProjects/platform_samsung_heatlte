/* audio_test_lib.c
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
  @file		audio_test_lib.c

  @brief	Audio test command library source file.
*/



/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include <fcntl.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <cutils/sockets.h>
#include <sys/socket.h>
#include <cutils/log.h>

#include <stdlib.h>
#include <signal.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <time.h>
#include <assert.h>
#include <endian.h>
#include <sys/poll.h>

#include <control_local.h>

#include <audio_test_extern.h>
#include <audio_test_lib.h>
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
enum {
	AMHAL_SPUV_START,
	AMHAL_SPUV_STOP,
	AMHAL_PCM_START,
	AMHAL_PCM_STOP,
	AMHAL_VOLUME,
	AMHAL_MUTE,
	AMHAL_SPEECH_MUTE,
	AMHAL_HW_PARMS,
	AMHAL_PARAMETERS,
	AMHAL_STOP,
	AMHAL_UDATA_IND,
	AMHAL_SYSTEM_ERROR_IND,
	AMHAL_DHA_VOICE_EQ_PARMS,
	AMHAL_VOLUME_EXT
} amhal_command_t;

/*---------------------------------------------------------------------------*/
/* structure declaration (private)                                           */
/*---------------------------------------------------------------------------*/
/* none */

/*---------------------------------------------------------------------------*/
/* prototype declaration                                                     */
/*---------------------------------------------------------------------------*/
int _audio_test_stop_pcm_loopback(void);

/*---------------------------------------------------------------------------*/
/* global variable declaration                                               */
/*---------------------------------------------------------------------------*/
/*!
  @brief	Output device type.
*/
static u_int audio_test_out_device_type = AUDIO_TEST_OUT_SPEAKER;
/*!
  @brief	Loopback state.
*/
static u_int audio_test_spuv_start_state = AUDIO_TEST_STATE_OFF;
/*!
  @brief	AMHAL handle.
*/
static int audio_test_amhal_fd;
/*!
  @brief	AMHAL volume.
*/
static int audio_test_amhal_volume[] = {
	0, 2, 4, 6, 8, 9
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
static void dump_elem_values(const snd_ctl_elem_value_t *ev,
				const snd_ctl_elem_info_t *einfo)
{
	uint i;

	ALOGD("DUMP_ELEM: %s", snd_ctl_elem_info_get_name(einfo));
	for (i = 0; i < snd_ctl_elem_info_get_count(einfo); i++) {
		switch (snd_ctl_elem_info_get_type(einfo)) {
		case SND_CTL_ELEM_TYPE_BOOLEAN:
		{
			int r = snd_ctl_elem_value_get_boolean(ev, i);
			ALOGD("DUMP_ELEM:   %d", r);
			break;
		}
		case SND_CTL_ELEM_TYPE_INTEGER:
		{
			long r = snd_ctl_elem_value_get_integer(ev, i);
			ALOGD("DUMP_ELEM:   %ld", r);
			break;
		}
		case SND_CTL_ELEM_TYPE_INTEGER64:
		{
			long long r = snd_ctl_elem_value_get_integer64(ev, i);
			ALOGD("DUMP_ELEM:   %lld", r);
			break;
		}
		case SND_CTL_ELEM_TYPE_ENUMERATED:
		{
			unsigned int r =
				snd_ctl_elem_value_get_enumerated(ev, i);
			ALOGD("DUMP_ELEM:   %d", r);
			break;
		}
		case SND_CTL_ELEM_TYPE_BYTES:
		{
			unsigned char r = snd_ctl_elem_value_get_byte(ev, i);
			ALOGD("DUMP_ELEM:   %d", (unsigned int)r);
			break;
		}
		case SND_CTL_ELEM_TYPE_IEC958:
		case SND_CTL_ELEM_TYPE_NONE:
		default:
			break;
		}
	}
}

/*!
  @brief	Get PCM name pointer.

  @param	in_device_type [i] Input device type.
  @param	out_device_type [i] Output device type.

  @return	PCM name.

  @note		.
*/
static char *audio_test_get_pcmname_ptr(u_int in_device_type,
					u_int out_device_type)
{
	char *pcmname = NULL;

	ALOGD("%s() in", __func__);

	switch (in_device_type) {
	case AUDIO_TEST_IN_MIC:
		switch (out_device_type) {
		case AUDIO_TEST_OUT_SPEAKER:
			pcmname = AUDIO_TEST_PCM_NAME_BM_SP;
			break;
		case AUDIO_TEST_OUT_HEADPHONE:
			pcmname = AUDIO_TEST_PCM_NAME_BM_HP;
			break;
		case AUDIO_TEST_OUT_EARPIECE:
			pcmname = AUDIO_TEST_PCM_NAME_BM_EP;
			break;
		default:
			goto dev_err;
		}
		break;
	case AUDIO_TEST_IN_MAINMIC:
		switch (out_device_type) {
		case AUDIO_TEST_OUT_SPEAKER:
			pcmname = AUDIO_TEST_PCM_NAME_MM_SP;
			break;
		case AUDIO_TEST_OUT_HEADPHONE:
			pcmname = AUDIO_TEST_PCM_NAME_MM_HP;
			break;
		case AUDIO_TEST_OUT_EARPIECE:
			pcmname = AUDIO_TEST_PCM_NAME_MM_EP;
			break;
		default:
			goto dev_err;
		}
		break;
	case AUDIO_TEST_IN_SUBMIC:
		switch (out_device_type) {
		case AUDIO_TEST_OUT_SPEAKER:
			pcmname = AUDIO_TEST_PCM_NAME_SM_SP;
			break;
		case AUDIO_TEST_OUT_HEADPHONE:
			pcmname = AUDIO_TEST_PCM_NAME_SM_HP;
			break;
		case AUDIO_TEST_OUT_EARPIECE:
			pcmname = AUDIO_TEST_PCM_NAME_SM_EP;
			break;
		default:
			goto dev_err;
		}
		break;
	case AUDIO_TEST_IN_HEADSETMIC:
		switch (out_device_type) {
		case AUDIO_TEST_OUT_SPEAKER:
			pcmname = AUDIO_TEST_PCM_NAME_HM_SP;
			break;
		case AUDIO_TEST_OUT_HEADPHONE:
			pcmname = AUDIO_TEST_PCM_NAME_HM_HP;
			break;
		case AUDIO_TEST_OUT_EARPIECE:
			pcmname = AUDIO_TEST_PCM_NAME_HM_EP;
			break;
		default:
			goto dev_err;
		}
		break;
	case AUDIO_TEST_IN_DEV_NONE:
		switch (out_device_type) {
		case AUDIO_TEST_OUT_SPEAKER:
			pcmname = AUDIO_TEST_PCM_NAME_SP;
			break;
		case AUDIO_TEST_OUT_HEADPHONE:
			pcmname = AUDIO_TEST_PCM_NAME_HP;
			break;
		case AUDIO_TEST_OUT_EARPIECE:
			pcmname = AUDIO_TEST_PCM_NAME_EP;
			break;
		default:
			goto dev_err;
		}
		break;
	default:
		goto dev_err;
	}

	ALOGD("%s() out", __func__);
	return pcmname;

dev_err:
	ALOGE("%s() ERROR!!! Unknown device type", __func__);
	return pcmname;
}

/*!
  @brief	Set asound.conf.

  @param	in_device_type [i] Input device type.
  @param	out_device_type [i] Output device type.

  @return	Function results.

  @note		.
*/
static int audio_test_set_asound_conf(u_int in_device_type,
					u_int out_device_type)
{
	int ret = -1;
	char *p = NULL;
	char *pcmname = NULL;

	ALOGD("%s() in", __func__);

	pcmname = audio_test_get_pcmname_ptr(in_device_type, out_device_type);
	if (NULL == pcmname) {
		ALOGE("%s() audio_test_get_pcmname_ptr error!!!", __func__);
		goto error;
	}

	ret = snd_config_check_values(pcmname, AUDIO_TEST_CLIENT_PT);
	if (0 != ret) {
		ALOGE("%s() snd_config check error!!!", __func__);
		goto error;
	}
	return ret;

error:
	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Set table of codec.

  @param	ctrl_tbl [i] Parameter table of control.
  @param	size [i] Table array size.

  @return	Function results.

  @note		.
*/
static int audio_test_set_codec_table(
			const struct audio_test_codec_ctrl *ctrl_tbl,
			u_int size)
{
	int ret = 0;
	u_int n = 0;
	struct audio_test_codec_ctrl ctrl;

	ALOGD("%s() in tbl_size[%d]", __func__, size);

	memset(&ctrl, 0, sizeof(ctrl));

	if (!ctrl_tbl) {
		ALOGE("%s() ERROR!!! Argument error", __func__);
		return -EINVAL;
	}

	/* Set codecs */
	for (n = 0; n < size; n++) {
		ALOGD("%s() name[%s]", __func__, ctrl_tbl[n].name);

		ctrl = ctrl_tbl[n];
		ret = snd_config_set_codec(&ctrl, NULL);
		if (ret < 0) {
			ALOGE("%s() ERROR!!! snd_config_set_codec ret[%d]",
				__func__, ret);
			goto error;
		}
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;

error:
	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Set control.

  @param	in_device_type [i] Input device type.
  @param	out_device_type [i] Onput device type.

  @return	Function results.

  @note		.
*/
static int audio_test_set_ctl(u_int in_device_type,
					u_int out_device_type)
{
	int ret = 0;
	snd_ctl_elem_id_t *eid_list = NULL;

	ALOGD("%s() in in_device[%d] out_device[%d]",
		__func__, in_device_type, out_device_type);

	ret = snd_config_create_ctl_and_list(&eid_list);
	if (ret < 0) {
		ALOGE("Cannot create CTL and List");
		goto error;
	}

	/* Set codec */
	ret = audio_test_set_asound_conf(in_device_type,
						out_device_type);
	if (0 != ret) {
		ALOGE("%s() ERROR!!! set_asound_conf ret[%d]",
			__func__, ret);
		goto error;
	}

error:
	snd_config_delete_ctl_and_list(eid_list);

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Clear value to register of codec.

  @param	.

  @return	Function results.

  @note		.
*/
static int audio_test_clear_codec(void)
{
	int ret = 0;
	snd_ctl_elem_id_t *eid_list = NULL;

	ALOGD("%s() in", __func__);

	ret = snd_config_create_ctl_and_list(&eid_list);
	if (ret < 0) {
		ALOGE("Cannot create CTL and List");
		goto error;
	}

	/* Clear */
	ret = audio_test_set_codec_table(audio_test_tbl_clear,
			audio_test_get_tbl_clear_array_size());
	if (0 != ret) {
		ALOGE("%s() ERROR!!! audio_test_set_codec_table ret[%d]",
			__func__, ret);
		goto error;
	}

error:
	snd_config_delete_ctl_and_list(eid_list);

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief Convert output device.

  @param param [o] Accessory ID.

  @return Function results.

  @note .
*/
static int audio_test_cnv_out_device_type(u_int *param)
{
	int ret = 0;

	switch (audio_test_out_device_type) {
	case AUDIO_TEST_OUT_EARPIECE:
		*param = 0x0;
		break;
	case AUDIO_TEST_OUT_SPEAKER:
		*param = 0x1;
		break;
	case AUDIO_TEST_OUT_HEADPHONE:
		*param = 0x2;
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

/*!
  @brief	Copy process.

  @param	to [i] Dest ptr.
  @param	from [i] Source ptr.
  @param	len [i] Size.

  @return	.

  @note		.
*/
static inline void audio_test_strncpy(char *to, const char *from, size_t len)
{
	ALOGD("%s() in", __func__);
	if (len > 0) {
		strncpy(to, from, len);
		*(to + len - 1)  = '\0';
	} else {
		ALOGE("Length error ! len[%d]", len);
	}
	ALOGD("%s() out", __func__);
}

/*!
  @brief	Start SPUV.

  @param	.

  @return	Function results.

  @note		.
*/
static int audio_test_start_spuv(u_int pcm_param)
{
	int ret = 0;
	int fd = -1;
	int32_t err = -1;
	ssize_t sent_length;
	u_int set = 1;
	u_int unset = 0;
	u_int param = 0x0;
	u_int spuv_pcm_param = pcm_param;
	u_int spuv_start_cmd = AMHAL_SPUV_START;
	u_int spuv_start_msg[4] = {spuv_start_cmd, 0U, 0U, 0U};
	u_int spuv_hw_parms_cmd = AMHAL_HW_PARMS;
	u_int spuv_hw_parms_msg[4] = {spuv_hw_parms_cmd, set, unset, set};
	u_int spuv_param_cmd = AMHAL_PARAMETERS;
	u_int spuv_param_msg[4] = {spuv_param_cmd, 0U, 0U, 0U};
	u_int spuv_pcm_start_cmd = AMHAL_PCM_START;
	u_int spuv_pcm_start_msg[4] = {spuv_pcm_start_cmd,
					spuv_pcm_param, 0U, 0U};

	ALOGD("audio_test_start_spuv() start");

	/***********************************/
	/* Create socket                   */
	/***********************************/
	fd = socket_local_client(AUDIO_TEST_AMHAL_SOCKET_FNAME,
				ANDROID_SOCKET_NAMESPACE_RESERVED,
				SOCK_DGRAM);
	if (fd < 0) {
		ALOGD("socket local client failed %d", fd);
		ret = -1;
		goto error;
	}
	audio_test_amhal_fd = fd;

	/************************************/
	/* Send SPUV_START command to AMHAL */
	/************************************/
	sent_length = send(fd, (const void *)&spuv_start_msg,
				sizeof spuv_start_msg, 0);
	if (sizeof(spuv_start_msg) == sent_length)
		ALOGD("send to AMHAL_SPUV_START ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to AMHAL_SPUV_START failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	/***********************************/
	/* Send HW_PARAMS command to AMHAL */
	/***********************************/
	sent_length = send(fd, (const void *)spuv_hw_parms_msg,
				sizeof spuv_hw_parms_msg, 0);
	if (sizeof(spuv_hw_parms_msg) == sent_length)
		ALOGD("send to AMHAL_HW_PARMS ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to AMHAL_HW_PARMS failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	/************************************/
	/* Send PARAMETERS command to AMHAL */
	/************************************/
	ret = audio_test_cnv_out_device_type(&param);
	if (0 != ret)
		goto error;

	spuv_param_msg[1] = param;
	sent_length = send(fd, (const void *)spuv_param_msg,
				sizeof spuv_param_msg, 0);
	if (sizeof(spuv_param_msg) == sent_length)
		ALOGD("send to AMHAL_PARAM ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to AMHAL_PARAM failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	/***********************************/
	/* Send PCM_START command to AMHAL */
	/***********************************/
	sent_length = send(fd, (const void *)spuv_pcm_start_msg,
				sizeof spuv_pcm_start_msg, 0);
	if (sizeof(spuv_pcm_start_msg) == sent_length)
		ALOGD("send to PCM_START ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to PCM_START failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	return ret;
error:
	if (fd >= 0) {
		close(fd);
		audio_test_amhal_fd = -1;
	}

	return ret;
}

/*!
  @brief	Stop SPUV.

  @param	.

  @return	Function results.

  @note		.
*/
static int audio_test_stop_spuv(void)
{
	int ret = 0;
	int fd = -1;
	ssize_t sent_length;
	u_int set = 1;
	u_int unset = 0;
	u_int spuv_pcm_stop_cmd = AMHAL_PCM_STOP;
	u_int spuv_pcm_stop_msg[4] = {spuv_pcm_stop_cmd, 0U, 0U, 0U};
	u_int spuv_stop_cmd = AMHAL_SPUV_STOP;
	u_int spuv_stop_msg[4] = {spuv_stop_cmd, 0U, 0U, 0U};

	/***********************************/
	/* Confirm AMHAL handle            */
	/***********************************/
	if (0 > audio_test_amhal_fd) {
		ret = -1;
		goto error;
	}

	fd = audio_test_amhal_fd;

	/**********************************/
	/* Send PCM_STOP command to AMHAL */
	/**********************************/
	sent_length = send(fd, (const void *)spuv_pcm_stop_msg,
				sizeof spuv_pcm_stop_msg, 0);
	if (sent_length == sizeof(spuv_pcm_stop_msg))
		ALOGD("send to AMHAL_PCM_STOP ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to AMHAL_PCM_STOP failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	/***********************************/
	/* Send SPUV_STOP command to AMHAL */
	/***********************************/
	sent_length = send(fd, (const void *)spuv_stop_msg,
				sizeof spuv_stop_msg, 0);
	if (sent_length == sizeof(spuv_stop_msg))
		ALOGD("send to AMHAL_SPUV_STOP ok: return %d sent",
				(int)sent_length);
	else {
		ALOGD("send to AMHAL_SPUV_STOP failed: Error send ret=%d",
				(int)sent_length);
		ret = -1;
		goto error;
	}

	/***********************************/
	/* Close socket                    */
	/***********************************/
	close(fd);
	audio_test_amhal_fd = -1;

	return ret;
error:
	return ret;
}

/*!
  @brief	Set SW gain.

  @param	volume [i] Value of SW gain.
				Range is 1-5.

  @return	Function results.

  @note		.
*/
static int audio_test_set_sw_gain(u_int volume)
{
	int ret = 0;
	int fd = 0;
	ssize_t sent_length;
	u_int set = 1;
	u_int unset = 0;
	u_int volume_cmd = AMHAL_VOLUME;
	u_int volume_msg[4] = {volume_cmd, 0U, 0U, 0U};

	ALOGD("%s() in volume[%d]", __func__, volume);

	/* Check an argument */
	if (AUDIO_TEST_SW_GAIN_LEVEL5 < volume) {
		ALOGE("%s() Argument Error!!!", __func__);
		ret = -1;
		goto error;
	}

	if (0 > audio_test_amhal_fd) {
		ALOGE("%s() Error!!! No file discription.", __func__);
		ret = -1;
		goto error;
	}

	fd = audio_test_amhal_fd;
	volume_msg[1] = audio_test_amhal_volume[volume];

	sent_length = send(fd, (const void *)volume_msg, sizeof(volume_msg), 0);
	if (sent_length == sizeof(volume_msg))
		ALOGD("%s() send to AMHAL_VOLUME ok: ret[%d]",
				__func__, (int)sent_length);
	else {
		ALOGE("%s() send to AMHAL_VOLUME failed: ret[%d] errno[%d]",
				__func__, (int)sent_length, errno);
		ret = -errno;
		goto error;
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	ALOGE("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Get hookSW file name.

  @param	name [o] HookSW file name.

  @return	Function results.

  @note		.
*/
static int audio_test_get_hooksw_file_name(char *name)
{
	int ret = 0;
	FILE *fp = NULL;
	char str[AUDIO_TEST_MAX_1LINE_LEN + 1];
	char *p = NULL;
	int detect = 0;
	long n = 0;

	ALOGD("%s() in", __func__);

	fp = fopen("/proc/bus/input/devices", "r");
	if (!fp) {
		ALOGE("%s() ERROR!!! fopen errno[%d]", __func__, errno);
		ret = -errno;
		goto error;
	}

	while (fgets(str, AUDIO_TEST_MAX_1LINE_LEN, fp) != NULL) {
		p = strstr(str, "N: Name=");
		if (!p)
			continue;

		p = p + strlen("N: Name=");
		if (strncmp(p, AUDIO_TEST_HOOKSW_NAME,
			strlen(AUDIO_TEST_HOOKSW_NAME)) == 0) {
			while (fgets(str, AUDIO_TEST_MAX_1LINE_LEN,
							fp) != NULL) {
				p = strstr(str, "N: Name=");
				if (p)
					break;

				p = strstr(str, "H: Handlers=");
				if (!p)
					continue;

				p = p + strlen("H: Handlers=");
				p = strstr(p, "event");
				if (!p) {
					ALOGE("%s() ERROR!!! NOT FOUND 'event'",
								__func__);
					ret = -EINVAL;
					goto error;
				} else {
					p = p + strlen("event");
					detect = 1;
					break;
				}
			}
			break;
		}
	}

	if (!detect) {
		ALOGE("%s() ERROR!!! NOT FOUND", __func__);
		ret = -EINVAL;
		goto error;
	}

	n = strtol(p, NULL, 10);
	if (errno == ERANGE) {
		ALOGE("%s() ERROR!!! Out of range(long)", __func__);
		ret = -errno;
		goto error;
	}

	snprintf(name, PATH_MAX - 1, "%s%ld", "/dev/input/event", n);

error:
	if (fp)
		fclose(fp);

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Get PCM name.

  @param	fd [i] .
  @param	pcmname [o] PCM name.

  @return	.

  @note		.
*/
static void audio_test_get_pcmname(int fd, char *pcmname)
{
	int res;
	struct audio_test_ioctl_pcmname_cmd pcmcmd;

	ALOGD("%s() in", __func__);

	memset(&pcmcmd, 0, sizeof(pcmcmd));

	pcmcmd.pcmtype = AUDIO_TEST_DRV_PCMTYPE_NORMAL;
	pcmcmd.pcmdirection = AUDIO_TEST_DRV_PCMDIR_PLAYBACK;

	res = ioctl(fd, AUDIO_TEST_IOCTL_GET_PCMNAME, &pcmcmd);
	if (0 != res)
		ALOGE("%s() ioctl GET_PCMNAME ERROR!!!", __func__);

	audio_test_strncpy(pcmname,
				pcmcmd.pcmname,
				AUDIO_TEST_PCMNAME_MAX_LEN);

	ALOGD("%s() out PCM name[%s]", __func__, pcmname);
}

/*!
  @brief	Set PCM name.

  @param	fd [i] .
  @param	in_device_type [i] Input device type.
  @param	out_device_type [i] Onput device type.
  @param	state [i] Status(Open/Close).

  @return	Function results.

  @note		.
*/
static int audio_test_set_pcmname(int fd, u_int in_device_type, u_int out_device_type, u_int state)
{
	int res = -1;
	struct audio_test_ioctl_pcmname_cmd pcmcmd;
	char *pcmname;

	ALOGD("%s() in", __func__);

	memset(&pcmcmd, 0, sizeof(pcmcmd));
	pcmcmd.pcmtype = AUDIO_TEST_DRV_PCMTYPE_PT;
	pcmcmd.pcmdirection = AUDIO_TEST_DRV_PCMDIR_PLAYBACK;
	pcmcmd.pcmstate = state;

	if (AUDIO_TEST_DRV_PCMSTATE_OPEN == state) {
		pcmname = audio_test_get_pcmname_ptr(in_device_type,
							out_device_type);
		if (NULL == pcmname) {
			ALOGE("%s() audio_test_get_pcmname_ptr error!!!", __func__);
			goto error;
		}
		audio_test_strncpy(pcmcmd.pcmname,
					pcmname,
					AUDIO_TEST_PCMNAME_MAX_LEN);
	}

	res = ioctl(fd, AUDIO_TEST_IOCTL_SET_PCMNAME, &pcmcmd);
	if (0 != res)
		ALOGE("%s() ioctl SET_PCMNAME ERROR!!!", __func__);

error:
	ALOGD("%s() out ret[%d]", __func__, res);
	return res;
}

/*!
  @brief	Set ALSA device.

  @param	fd [i] .

  @return	Function results.

  @note		.
*/
static int audio_test_set_alsa_device(int fd)
{
	int ret;
	char pcmname[AUDIO_TEST_PCMNAME_MAX_LEN];

	ALOGD("%s() in", __func__);

	ret = audio_test_set_pcmname(fd, 0, 0, AUDIO_TEST_DRV_PCMSTATE_CLOSE);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_pcmname ERROR!!!", __func__);
		goto error;
	}

	audio_test_get_pcmname(fd, pcmname);
	ret = snd_config_check_values(pcmname, AUDIO_TEST_CLIENT_PT);
	if (0 != ret) {
		ALOGE("%s() snd_config check error!!!", __func__);
		goto error;
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;

error:
	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*------------------------------------*/
/* for public function                */
/*------------------------------------*/
/*!
  @brief	Setup input device and output device.

  @param	in_device [i] Input device info.
  @param	out_device [i] Output device info.
  @param	state [i] Device setting state.
			AUDIO_TEST_STATE_ON.
			AUDIO_TEST_STATE_OFF.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int _audio_test_set_device(struct audio_test_in_device_info *in_device,
			struct audio_test_out_device_info *out_device,
			u_int state)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;

	if (!in_device || !out_device) {
		ALOGE("%s() ERROR!!! Argument error", __func__);
		return -EINVAL;
	}

	ALOGD("%s() in in_device[%d] out_device[%d] out_LR[%d] out_vol[%d] state[%d]",
		__func__, in_device->device_type, out_device->device_type,
		out_device->LR_type, out_device->volume, state);

	/***********************************/
	/* IOCTL                           */
	/***********************************/
	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		ALOGE("%s() open ERROR!!!", __func__);
		goto error;
	}

	/* Set control */
	ret = audio_test_set_ctl(in_device->device_type,
					out_device->device_type);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_ctl ERROR!!!", __func__);
		goto error;
	}

	cmd.out_device_type = out_device->device_type;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_SETDEVICE, &cmd);
	if (0 != ret) {
		ALOGE("%s() ioctl SETDEVICE ERROR!!!", __func__);
		goto error;
	}

	if (AUDIO_TEST_IN_DEV_NONE == in_device->device_type) {
		/* Change device on */
		if (AUDIO_TEST_STATE_ON == state) {
			ret = ioctl(fd, AUDIO_TEST_IOCTL_STARTSOUNDPLAY, &cmd);
			if (0 != ret) {
				ALOGE("%s() ioctl STARTSOUNDPLAY ERROR!!!", __func__);
				goto error;
			}
		/* Change device off */
		} else {
			ret = ioctl(fd, AUDIO_TEST_IOCTL_STOPSOUNDPLAY, &cmd);
			if (0 != ret) {
				ALOGE("%s() ioctl STOPSOUNDPLAY ERROR!!!", __func__);
				goto error;
			}
			ret = audio_test_set_pcmname(fd, 0, 0, AUDIO_TEST_DRV_PCMSTATE_CLOSE);
			if (0 != ret) {
				ALOGE("%s() audio_test_set_pcmname(CLOSE) ERROR!!!", __func__);
				goto error;
			}
			close(fd);
			return ret;
		}
	}

	ret = audio_test_set_pcmname(fd, in_device->device_type,
					out_device->device_type, AUDIO_TEST_DRV_PCMSTATE_OPEN);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_pcmname(OPEN) ERROR!!!", __func__);
		goto error;
	}

	close(fd);

	/***********************************/
	/* Regist output device            */
	/***********************************/
	audio_test_out_device_type = out_device->device_type;

	/* Set AMHAL volume */
	if (audio_test_spuv_start_state == AUDIO_TEST_STATE_ON) {
		ret = audio_test_set_sw_gain(out_device->volume);
		if (0 != ret) {
			ALOGE("%s() audio_test_set_sw_gain ERROR!!!", __func__);
			goto gain_error;
		}
	}

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);
gain_error:
	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
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
int _audio_test_start_loopback(u_int fsi_port)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;

	ALOGD("%s() in fsi_port[%d]", __func__, fsi_port);

	memset(&cmd, 0, sizeof(cmd));

	if (AUDIO_TEST_FSI_PORTA != fsi_port)
		return -1;

	/***********************************/
	/* IOCTL                           */
	/***********************************/
	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	cmd.fsi_port = fsi_port;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_STARTSCUWLOOP, &cmd);
	if (0 != ret)
		goto error;

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_ON;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop SCUW loopback [AudioIC->FSI->SCUW->FSI->AudioIC].

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int _audio_test_stop_loopback(void)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;

	ALOGD("%s() in", __func__);

	memset(&cmd, 0, sizeof(cmd));

	/***********************************/
	/* IOCTL                           */
	/***********************************/
	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	ret = ioctl(fd, AUDIO_TEST_IOCTL_STOPSCUWLOOP, &cmd);
	if (0 != ret)
		goto error;

	/***********************************/
	/* Clear codec                     */
	/***********************************/
	ret = audio_test_clear_codec();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! audio_test_clear_codec ret[%d]",
			__func__, ret);
		goto error;
	}

	ret = audio_test_set_alsa_device(fd);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_alsa_device ERROR!!!", __func__);
		goto error;
	}

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_OFF;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
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
int _audio_test_detect_jack(u_int *state)
{
	int ret = 0;
	FILE *fp = NULL;
	size_t size = 0;
	char state_char[AUDIO_TEST_LEN_JACK_STATE];
	int state_int = 0;

	ALOGD("%s() in", __func__);

	memset(state_char, 0, sizeof(state_char));

	/***********************************/
	/* Read file                       */
	/***********************************/
	fp = fopen("/sys/class/switch/h2w/state", "r");
	if (!fp) {
		ALOGE("%s() ERROR!!! fopen errno[%d]", __func__, errno);
		ret = -errno;
		goto error;
	}

	size = fread(state_char, 1, sizeof(state_char), fp);
	if (sizeof(state_char) != size) {
		ALOGE("%s() ERROR!!! fread size[%d] errno[%d]",
			__func__, size, errno);
		ret = -errno;
		goto error;
	}

	/***********************************/
	/* Get state                       */
	/***********************************/
	state_int = atoi(state_char);
	switch (state_int) {
	case AUDIO_TEST_VAL_JACK_STATE_IN:
	case AUDIO_TEST_LEN_JACK_STATE_INMIC:
		*state = AUDIO_TEST_STATE_ON;
		ALOGD("%s() AUDIO_TEST_STATE_ON", __func__);
		break;
	default:
		*state = AUDIO_TEST_STATE_OFF;
		ALOGD("%s() AUDIO_TEST_STATE_OFF", __func__);
		break;
	}

	fclose(fp);

	ALOGD("%s() out ret[%d] state[%d]", __func__, ret, *state);
	return ret;
error:
	if (fp)
		fclose(fp);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
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
int _audio_test_detect_key(u_int *state)
{
	int ret = 0;
	char name[PATH_MAX] = "";
	uint8_t *bits = NULL;
	ssize_t bits_size = 0;
	int j = 0;
	int k = 0;
	int fd = -1;
	int res = 0;
	int res2 = 0;

	ALOGD("%s() in", __func__);

	ret = audio_test_get_hooksw_file_name(name);
	if (ret != 0) {
		ALOGE("%s() ERROR!!! Get hooksw file name", __func__);
		goto error;
	}

	ALOGD("%s() open file name[%s]", __func__, name);

	fd = open(name, O_RDWR);
	if (fd < 0) {
		ALOGE("%s() ERROR!!! open errno[%d]", __func__, errno);
		ret = -errno;
		goto error;
	}

	while (1) {
		res = ioctl(fd, 
			AUDIO_TEST_EVIOCGBIT(AUDIO_TEST_EV_KEY, bits_size), 
			bits);

		if (res < bits_size)
			break;

		bits_size = res + 16;
		bits = realloc(bits, bits_size * 2);
		if (!bits) {
			ALOGE("%s() ERROR!!! realloc errno[%d]",
				__func__, errno);
			ret = -errno;
			goto error;
		}
	}

	res2 = ioctl(fd, AUDIO_TEST_EVIOCGKEY(res), bits + bits_size);
	ALOGD("%s() res:%d, bits_size:%d, res2:%d",
		__func__, res, (int)bits_size, res2);

	for (j = 0; j < res; j++) {
		for (k = 0; k < 8; k++) {
			if (bits[j] & 1 << k) {
				if ((j < res2) &&
					(bits[j + bits_size] & 1 << k)) {
					*state = AUDIO_TEST_STATE_ON;
					ALOGD("%s() AUDIO_TEST_STATE_ON",
						__func__);
				} else {
					*state = AUDIO_TEST_STATE_OFF;
					ALOGD("%s() AUDIO_TEST_STATE_OFF",
						__func__);
				}
			}
		}
	}
	free(bits);
	close(fd);

	ALOGD("%s() out ret[%d] state[%d]", __func__, ret, *state);
	return ret;
error:
	if (!bits)
		free(bits);

	if (fd >= 0)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Start tone.

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int _audio_test_start_tone(void)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;
	u_int param = AUDIO_TEST_1KHZTONE_PRM;

	ALOGD("%s() in", __func__);

	memset(&cmd, 0, sizeof(cmd));

	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	cmd.call_kind = AUDIO_TEST_DRV_KIND_1KHZ;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_SETCALLMODE, &cmd);
	if (0 != ret)
		goto error;

	/***********************************/
	/* Start SPUV                      */
	/***********************************/
	ret = audio_test_start_spuv(param);
	if (0 != ret)
		goto error;

	memset(&cmd, 0, sizeof(cmd));
	ret = ioctl(fd, AUDIO_TEST_IOCTL_STARTTONE, &cmd);
	if (0 != ret)
		goto error;

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_ON;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Stop tone.

  @param	.

  @return	Function results.

  @note		This function is a synchronization function.
		The plural collective calls are prohibited.
*/
int _audio_test_stop_tone(void)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;

	ALOGD("%s() in", __func__);

	memset(&cmd, 0, sizeof(cmd));

	/***********************************/
	/* Stop SPUV                       */
	/***********************************/
	ret = audio_test_stop_spuv();
	if (0 != ret)
		goto error;

	/***********************************/
	/* IOCTL                           */
	/***********************************/
	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	ret = ioctl(fd, AUDIO_TEST_IOCTL_STOPTONE, &cmd);
	if (0 != ret)
		goto error;

	memset(&cmd, 0, sizeof(cmd));
	cmd.call_kind = AUDIO_TEST_DRV_KIND_CALL;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_SETCALLMODE, &cmd);
	if (0 != ret)
		goto error;

	/***********************************/
	/* Clear codec                     */
	/***********************************/
	ret = audio_test_clear_codec();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! audio_test_clear_codec ret[%d]",
			__func__, ret);
		goto error;
	}

	ret = audio_test_set_alsa_device(fd);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_alsa_device ERROR!!!", __func__);
		goto error;
	}

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_OFF;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}

/*!
  @brief	Start SPUV loopback.
			PCM LOOPBACK   : [AudioIC->FSI->SCUW->SPUV->SCUW->FSI->AudioIC].
			VOLTE LOOPBACK : [AudioIC->FSI->SCUW->SPUV->Modem->SPUV->SCUW->FSI->AudioIC].

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
int _audio_test_start_pcm_loopback(u_int fsi_port,
						u_int vqa_val,
						u_int delay_val,
						u_int volte_val)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;
	u_int param = 0;

	ALOGD("%s() in fsi_port[%d] vqa[%d] delay[%d] volte_val[%d]",
		__func__, fsi_port, vqa_val, delay_val, volte_val);

	memset(&cmd, 0, sizeof(cmd));

	if (AUDIO_TEST_FSI_PORTA != fsi_port)
		return -1;

	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	cmd.call_kind = AUDIO_TEST_DRV_KIND_KIND_PCM_LB;
	cmd.vqa_val = vqa_val;
	cmd.delay_val = delay_val;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_SETCALLMODE, &cmd);
	if (0 != ret)
		goto error;

	/***********************************/
	/* Start SPUV                      */
	/***********************************/
	if (AUDIO_TEST_VOLTE_NONE == volte_val) {
		if (AUDIO_TEST_STATE_OFF == vqa_val ||
			AUDIO_TEST_DRV_STATE_OFF == delay_val) {
			param = AUDIO_TEST_IFLB_PRM;
		} else if (AUDIO_TEST_STATE_ON == vqa_val ||
			AUDIO_TEST_STATE_OFF == delay_val) {
			param = AUDIO_TEST_PCMLB_PRM;
		} else {
			param = AUDIO_TEST_PKTLB_PRM;
		}
	} else {
		if (AUDIO_TEST_VOLTE_NB == volte_val)
			param = AUDIO_TEST_VOLTENB_PRM;
		else
			param = AUDIO_TEST_VOLTEWB_PRM;
	}
	ret = audio_test_start_spuv(param);
	if (0 != ret)
		goto error;

	memset(&cmd, 0, sizeof(cmd));
	cmd.fsi_port = fsi_port;
	cmd.vqa_val = vqa_val;
	cmd.delay_val = delay_val;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_STARTSPUVLOOP, &cmd);
	if (0 != ret) {
		ret = -errno;
		goto spuv_error;
	}

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_ON;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
spuv_error:
	(void)_audio_test_stop_pcm_loopback();

error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
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
int _audio_test_stop_pcm_loopback(void)
{
	int ret = 0;
	int fd = 0;
	struct audio_test_ioctl_cmd cmd;

	ALOGD("%s() in", __func__);

	memset(&cmd, 0, sizeof(cmd));

	/***********************************/
	/* Stop SPUV                       */
	/***********************************/
	ret = audio_test_stop_spuv();
	if (0 != ret)
		goto error;

	/***********************************/
	/* IOCTL                           */
	/***********************************/
	fd = open(AUDIO_TEST_FILE, O_RDWR);
	if (-1 == fd) {
		ret = -1;
		goto error;
	}

	ret = ioctl(fd, AUDIO_TEST_IOCTL_STOPSPUVLOOP, &cmd);
	if (0 != ret)
		goto error;

	memset(&cmd, 0, sizeof(cmd));
	cmd.call_kind = AUDIO_TEST_DRV_KIND_CALL;
	ret = ioctl(fd, AUDIO_TEST_IOCTL_SETCALLMODE, &cmd);
	if (0 != ret)
		goto error;

	/***********************************/
	/* Clear codec                     */
	/***********************************/
	ret = audio_test_clear_codec();
	if (0 != ret) {
		ALOGE("%s() ERROR!!! audio_test_clear_codec ret[%d]",
			__func__, ret);
		goto error;
	}

	ret = audio_test_set_alsa_device(fd);
	if (0 != ret) {
		ALOGE("%s() audio_test_set_alsa_device ERROR!!!", __func__);
		goto error;
	}

	close(fd);

	audio_test_spuv_start_state = AUDIO_TEST_STATE_OFF;

	ALOGD("%s() out ret[%d]", __func__, ret);
	return ret;
error:
	if (-1 != fd)
		close(fd);

	ALOGE("%s() out ERROR!!! ret[%d]", __func__, ret);
	return ret;
}
