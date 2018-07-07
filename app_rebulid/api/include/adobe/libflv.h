/*
 * libflv.h
 *
 *  Created on: 2012-8-29
 *      Author: root
 */

#ifndef LIBFLV_H_
#define LIBFLV_H_

#include "bufio.h"

#define AMF_FALSE	(0)
#define AMF_TRUE	(1)

/* offsets for packed values */
#define FLV_AUDIO_SAMPLESSIZE_OFFSET 1
#define FLV_AUDIO_SAMPLERATE_OFFSET  2
#define FLV_AUDIO_CODECID_OFFSET     4

#define FLV_VIDEO_FRAMETYPE_OFFSET   4

/* bitmasks to isolate specific values */
#define FLV_AUDIO_CHANNEL_MASK    0x01
#define FLV_AUDIO_SAMPLESIZE_MASK 0x02
#define FLV_AUDIO_SAMPLERATE_MASK 0x0c
#define FLV_AUDIO_CODECID_MASK    0xf0

#define FLV_VIDEO_CODECID_MASK    0x0f
#define FLV_VIDEO_FRAMETYPE_MASK  0xf0

#define AMF_END_OF_OBJECT         0x09

enum {
	FLV_HEADER_FLAG_HASVIDEO = 1,
	FLV_HEADER_FLAG_HASAUDIO = 4,
};

enum {
	FLV_TAG_TYPE_AUDIO = 0x08,
	FLV_TAG_TYPE_VIDEO = 0x09,
	FLV_TAG_TYPE_META  = 0x12,
};

enum {
	FLV_MONO   = 0,
	FLV_STEREO = 1,
};

enum {
	FLV_SAMPLESSIZE_8BIT  = 0,
	FLV_SAMPLESSIZE_16BIT = 1 << FLV_AUDIO_SAMPLESSIZE_OFFSET,
};

enum {
	FLV_SAMPLERATE_SPECIAL = 0, /**< signifies 5512Hz and 8000Hz in the case of NELLYMOSER */
	FLV_SAMPLERATE_11025HZ = 1 << FLV_AUDIO_SAMPLERATE_OFFSET,
	FLV_SAMPLERATE_22050HZ = 2 << FLV_AUDIO_SAMPLERATE_OFFSET,
	FLV_SAMPLERATE_44100HZ = 3 << FLV_AUDIO_SAMPLERATE_OFFSET,
};

enum {
	FLV_CODECID_PCM                  = 0,
	FLV_CODECID_ADPCM                = 1 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_MP3                  = 2 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_PCM_LE               = 3 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER_16KHZ_MONO= 4 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER_8KHZ_MONO = 5 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER           = 6 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_G711_A_LAW			 = 7 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_G711_MU_LAW			 = 8 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_AAC                  = 10<< FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_SPEEX                = 11<< FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_MP3_8KHZ			 = 14<< FLV_AUDIO_CODECID_OFFSET,	
};

enum{
	FLV_AAC_SEQUENCE_HEADER	= 0,
	FLV_AAC_RAW,
};

enum {
	FLV_CODECID_H263    = 2,
	FLV_CODECID_SCREEN  = 3,
	FLV_CODECID_VP6     = 4,
	FLV_CODECID_VP6A    = 5,
	FLV_CODECID_SCREEN2 = 6,
	FLV_CODECID_H264    = 7,
};

enum {
	FLV_FRAME_KEY        = 1 << FLV_VIDEO_FRAMETYPE_OFFSET,
	FLV_FRAME_INTER      = 2 << FLV_VIDEO_FRAMETYPE_OFFSET,
	FLV_FRAME_DISP_INTER = 3 << FLV_VIDEO_FRAMETYPE_OFFSET,
};

enum {
	AMF_DATA_TYPE_NUMBER      = 0x00,
	AMF_DATA_TYPE_BOOL        = 0x01,
	AMF_DATA_TYPE_STRING      = 0x02,
	AMF_DATA_TYPE_OBJECT      = 0x03,
	AMF_DATA_TYPE_NULL        = 0x05,
	AMF_DATA_TYPE_UNDEFINED   = 0x06,
	AMF_DATA_TYPE_REFERENCE   = 0x07,
	AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
	AMF_DATA_TYPE_OBJECT_END  = 0x09,
	AMF_DATA_TYPE_ARRAY       = 0x0a,
	AMF_DATA_TYPE_DATE        = 0x0b,
	AMF_DATA_TYPE_LONG_STRING = 0x0c,
	AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
};


int avlib_flv_mkheader(BufIO *pb,int width,int height,int bit_rate,double framerate, double duration, unsigned char* _data, int _data_len);
int avlib_flv_write_packet(BufIO *pb, unsigned char *buf,int size,int timestamp,int keyframe);
int jalib_rtmp_mk_avcframe(BufIO* _pb, unsigned char* _data, int _data_len);
int jalib_rtmp_mkmeta(BufIO* _pb, int _width, int _height, int _bit_rate, double _framerate, double _duration);
int avlib_rtmp_mkvideo(BufIO* _pb, unsigned char* buf, int size, int keyframe);

#endif /* LIBFLV_H_ */
