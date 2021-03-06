
#include "sdk_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <poll.h>

#ifndef SDK_ENC_H_
#define SDK_ENC_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	EPTZ_CMD_NULL = -1,
	EPTZ_CMD_UP,
	EPTZ_CMD_DOWN,
	EPTZ_CMD_LEFT,
	EPTZ_CMD_RIGHT,
	EPTZ_CMD_LEFT_UP,
	EPTZ_CMD_RIGHT_UP,
	EPTZ_CMD_LEFT_DOWN,
	EPTZ_CMD_RIGHT_DOWN,
	EPTZ_CMD_AUTOPAN,
	EPTZ_CMD_IRIS_OPEN,
	EPTZ_CMD_IRIS_CLOSE,
	EPTZ_CMD_ZOOM_IN,
	EPTZ_CMD_ZOOM_OUT,
	EPTZ_CMD_FOCUS_FAR,
	EPTZ_CMD_FOCUS_NEAR,
	EPTZ_CMD_STOP,
	EPTZ_CMD_WIPPER_ON,
	EPTZ_CMD_WIPPER_OFF,
	EPTZ_CMD_LIGHT_ON,
	EPTZ_CMD_LIGHT_OFF,
	EPTZ_CMD_POWER_ON,
	EPTZ_CMD_POWER_OFF,
	EPTZ_CMD_GOTO_PRESET,
	EPTZ_CMD_SET_PRESET,
	EPTZ_CMD_CLEAR_PRESET,
	EPTZ_CMD_TOUR,
	EPTZ_COMMAND_CNT,
}EPTZ_COMMAND;

typedef enum{
	eSDK_ENC_FIX_MODE_WALL=0,
	eSDK_ENC_FIX_MODE_CELL,
	eSDK_ENC_FIX_MODE_TABLE,
}emSDK_ENC_FIX_MODE;

typedef enum{
	eSDK_ENC_SHOW_MODE_WALL_ORIGIN = 0x0,
	eSDK_ENC_SHOW_MODE_WALL_180,
	eSDK_ENC_SHOW_MODE_WALL_SPLIT,
	eSDK_ENC_SHOW_MODE_WALL_WALL_SPLIT,
	eSDK_ENC_SHOW_MODE_WALL_4R,
	eSDK_ENC_SHOW_MODE_WALL_KITR,
	eSDK_ENC_SHOW_MODE_WALL_KITO,

	eSDK_ENC_SHOW_MODE_CELL_ORIGIN = 0x10,
	eSDK_ENC_SHOW_MODE_CELL_360,
	eSDK_ENC_SHOW_MODE_CELL_SPLIT,
	eSDK_ENC_SHOW_MODE_CELL_4R,
	eSDK_ENC_SHOW_MODE_CELL_WALL_SPLIT,
	eSDK_ENC_SHOW_MODE_CELL_180,
	eSDK_ENC_SHOW_MODE_CELL_KITR,
	eSDK_ENC_SHOW_MODE_CELL_KITO,

	eSDK_ENC_SHOW_MODE_TABLE_ORIGIN= 0x20,
	eSDK_ENC_SHOW_MODE_TABLE_360,
	eSDK_ENC_SHOW_MODE_TABLE_SPLIT,
	eSDK_ENC_SHOW_MODE_TABLE_4R,
	eSDK_ENC_SHOW_MODE_TABLE_VR,
	eSDK_ENC_SHOW_MODE_TABLE_KITR,
	eSDK_ENC_SHOW_MODE_TABLE_KITO,
}emSDK_ENC_SHOW_MODE;

typedef enum SDK_ENC_BUF_FRAME_TYPE
{
    kSDK_ENC_BUF_FRAME_UNUSE = 0,
    kSDK_ENC_BUF_FRAME_BASE_IDRSLICE = 1,                              //the Idr frame at Base layer
    kSDK_ENC_BUF_FRAME_BASE_PSLICE_REFTOIDR,                           //the P frame at Base layer, referenced by other frames at Base layer and reference to Idr frame
    kSDK_ENC_BUF_FRAME_BASE_PSLICE_REFBYBASE,                          //the P frame at Base layer, referenced by other frames at Base layer
    kSDK_ENC_BUF_FRAME_BASE_PSLICE_REFBYENHANCE,                       //the P frame at Base layer, referenced by other frames at Enhance layer
    kSDK_ENC_BUF_FRAME_ENHANCE_PSLICE_REFBYENHANCE,                    //the P frame at Enhance layer, referenced by other frames at Enhance layer
    kSDK_ENC_BUF_FRAME_ENHANCE_PSLICE_NOTFORREF,                       //the P frame at Enhance layer ,not referenced
    kSDK_ENC_BUF_FRAME_ENHANCE_PSLICE_BUTT
}enSDK_ENC_BUF_FRAME_TYPE;

#define kSDK_ENC_BUF_DATA_MAGIC (0xff00a0a0UL)
typedef enum SDK_ENC_BUF_DATA_TYPE {
	// video
	kSDK_ENC_BUF_DATA_H264 = (0x00000000UL),
	kSDK_ENC_BUF_DATA_H265,
	kSDK_ENC_BUF_DATA_JPEG,
	// audio
	kSDK_ENC_BUF_DATA_PCM = (0x80000000UL),
	kSDK_ENC_BUF_DATA_G711A,
	kSDK_ENC_BUF_DATA_G711U,
	kSDK_ENC_BUF_DATA_AAC,
}enSDK_ENC_BUF_DATA_TYPE, *lpSDK_ENC_BUF_DATA_TYPE;

#pragma pack(4)
typedef struct _SoupFrameHead{
    unsigned int magic;         /* magic number 固定为 0x534f55ff , "SOU·" */
    unsigned int version;       /* 版本信息，当前版本为1.0.0.0，固定为0x01000000*/
    unsigned int frametype;     /* 码流帧类型，当前版本支持三种类型：0x00--音频 0x01--视频I帧 0x02--视频P帧 */
    unsigned int framesize;     /* 码流帧的裸数据长度 */
    /*
     *  帧时间戳，64位数据，精度到微秒
     *  历史原因，这个pts直接使用的struct timeval,Client 解析时sec/usec又反过来了
     *  所以有以下形式的联合体
     *  union{
     *      JaInt64 pts;
     *      struct {
     *          unsigned int ts_usec;
     *          unsigned int ts_sec;
     *      }ts;
     *  }
     */
    unsigned int ts_usec;
    unsigned int ts_sec;
    unsigned int externsize; /* 扩展数据大小，当前版本为0 */
    union{
        /* 视频宽、高 编码， ascii "H264" */
        struct {
            unsigned int width;
            unsigned int height;
            unsigned int enc;
        }v;
        /* 音频采样率，位宽， G711<orz 只有四位放不下g711a> */
        struct {
            unsigned int samplerate;
            unsigned int samplewidth;
            unsigned int enc;
        }a;
    };
}ST_SoupFrameHead, *LP_SoupFrameHead;

typedef struct SDK_ENC_BUF_ATTR {
	// public attr
	uint32_t magic; // must be "kSdkEncBufDataMagic"

	enSDK_ENC_BUF_DATA_TYPE type;
	uint64_t timestamp_us; // the timestamp of soc engine, unit: us
	uint64_t time_us; // the timestamp of system clock, unit: us
	size_t data_sz;
	union {
		// kSdkEncBufDataH264
		struct {
			bool keyframe; // TRUE / FALSE
			uint32_t ref_counter; // ref frame counter
			uint32_t fps;
			uint32_t width;
			uint32_t height;
			uint32_t frametype;
		}h264;
		struct {
			bool keyframe; // TRUE / FALSE
			uint32_t ref_counter; // ref frame counter
			uint32_t fps;
			uint32_t width;
			uint32_t height;
			uint32_t frametype;
		}h265;
	
		// kSdkEncBufDataPcm, kSdkEncBufDataG711a, kSdkEncBufDataG711u
		struct {
			uint32_t sample_rate;
			uint32_t sample_width;
			uint32_t packet;
			float compression_ratio; // if g711a.u == 2.0
		}pcm, g711a, g711u;
	};
	ST_SoupFrameHead soupFrameHead;
}stSDK_ENC_BUF_ATTR, *lpSDK_ENC_BUF_ATTR;
#pragma pack()

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define kSDK_ENC_QUALITY_AUTO (0)
#define kSDK_ENC_QUALITY_FLUENCY (1)
#define kSDK_ENC_QUALITY_BD (2)
#define kSDK_ENC_QUALITY_HD (3)

typedef struct SDK_ENC_STREAM_H264_ATTR {
	char name[32];
	int  width, height;
	int fps, gop;

#define kSDK_ENC_H264_PROFILE_AUTO (0)
#define kSDK_ENC_H264_PROFILE_BASELINE (1)
#define kSDK_ENC_H264_PROFILE_MAIN (2)
#define kSDK_ENC_H264_PROFILE_HIGH (3)
	int profile;
#define kSDK_ENC_H264_RC_MODE_AUTO (0)
#define kSDK_ENC_H264_RC_MODE_VBR (1)
#define kSDK_ENC_H264_RC_MODE_CBR (2)
#define kSDK_ENC_H264_RC_MODE_ABR (3)
#define kSDK_ENC_H264_RC_MODE_FIXQP (4)
	int rc_mode;
	int bps;
	int quality;
	int buf_id;
    int refEnable;//璺冲抚鍙傝�� 1寮�鍚�0鍏抽棴
	bool _enable_smartP;
}ST_SDK_ENC_STREAM_H264_ATTR, *LP_SDK_ENC_STREAM_H264_ATTR;

typedef struct SDK_ENC_STREAM_H265_ATTR {
	char name[32];
	int  width, height;
	int fps, gop;
	
#define kSDK_ENC_H265_PROFILE_MAIN (0)

	int profile;
#define kSDK_ENC_H265_RC_MODE_AUTO (0)
#define kSDK_ENC_H265_RC_MODE_VBR (1)
#define kSDK_ENC_H265_RC_MODE_CBR (2)
#define kSDK_ENC_H265_RC_MODE_FIXQP (3)
	int rc_mode;
	int bps;
	int quality;
	int buf_id;
    int refEnable;//璺冲抚鍙傝�� 1寮�鍚�0鍏抽棴
	bool _enable_smartP;
}ST_SDK_ENC_STREAM_H265_ATTR, *LP_SDK_ENC_STREAM_H265_ATTR;


typedef struct SDK_ENC_STREAM_ATTR{
	enSDK_ENC_BUF_DATA_TYPE enType;
	union{
		ST_SDK_ENC_STREAM_H264_ATTR  H264_attr;
		ST_SDK_ENC_STREAM_H265_ATTR  H265_attr;
	};
}ST_SDK_ENC_STREAM_ATTR,*LP_SDK_ENC_STREAM_ATTR;



typedef struct SDK_ENC_STREAM_G711A_ATTR {
	int ain; // ignore when init
	int vin_ref; // ignore when init
	int sample_rate, sample_width;
	size_t packet_size;
}ST_SDK_ENC_STREAM_G711A_ATTR, *LP_SDK_ENC_STREAM_G711A_ATTR;

typedef enum SDK_ENC_SNAPSHOT_QUALITY {
	kSDK_ENC_SNAPSHOT_QUALITY_HIGHEST,
	kSDK_ENC_SNAPSHOT_QUALITY_HIGH,
	kSDK_ENC_SNAPSHOT_QUALITY_MEDIUM,
	kSDK_ENC_SNAPSHOT_QUALITY_LOW,
	kSDK_ENC_SNAPSHOT_QUALITY_LOWEST,
}enSDK_ENC_SNAPSHOT_QUALITY, *lpSDK_ENC_SNAPSHOT_QUALITY;

#define kSDK_ENC_SNAPSHOT_SIZE_AUTO (0)
#define kSDK_ENC_SNAPSHOT_SIZE_MAX (-1)
#define kSDK_ENC_SNAPSHOT_SIZE_MIN (-2)

typedef union SDK_ENC_VIDEO_OVERLAY_PIXEL {
	uint32_t argb8888;
	struct {
		uint8_t blue, green, red, alpha;
	};
}stSDK_ENC_VIDEO_OVERLAY_PIXEL, *lpSDK_ENC_VIDEO_OVERLAY_PIXEL;

#define SDK_ENC_VIDEO_OVERLAY_PIXEL_RGB(_pixel, _a, _r, _g, _b) \
	do{\
		_pixel.alpha = (_a), _pixel.red = (_r), _pixel.green = (_g), _pixel.blue = (_b);\
	}while(0)

typedef struct SDK_ENC_VIDEO_OVERLAY_PIXEL_FORMAT {
	uint32_t rmask, gmask, bmask, amask;
}stSDK_ENC_VIDEO_OVERLAY_PIXEL_FORMAT, *lpSDK_ENC_VIDEO_OVERLAY_PIXEL_FORMAT;

typedef struct SDK_ENC_VIDEO_OVERLAY_CANVAS {

	size_t width, height;
	stSDK_ENC_VIDEO_OVERLAY_PIXEL_FORMAT pixel_format;
	void* pixels;
	int phy_addr;

	int (*put_pixel)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, int x, int y,
		stSDK_ENC_VIDEO_OVERLAY_PIXEL pixel);
	int (*get_pixel)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, int x, int y,
		lpSDK_ENC_VIDEO_OVERLAY_PIXEL ret_pixel);
	bool (*match_pixel)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, stSDK_ENC_VIDEO_OVERLAY_PIXEL pixel1, stSDK_ENC_VIDEO_OVERLAY_PIXEL pixel2);
	int (*put_rect)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, int x, int y,
		size_t width, size_t height,stSDK_ENC_VIDEO_OVERLAY_PIXEL pixel);
	int (*fill_rect)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, int x, int y,
		size_t width, size_t height, stSDK_ENC_VIDEO_OVERLAY_PIXEL pixel);
	int (*erase_rect)(struct SDK_ENC_VIDEO_OVERLAY_CANVAS* canvas, int x, int y, size_t width, size_t height);
	
}ST_SDK_ENC_VIDEO_OVERLAY_CANVAS, *LP_SDK_ENC_VIDEO_OVERLAY_CANVAS;

typedef int (*fSDK_ENC_DO_BUFFER_REQUEST)(int buf_id, size_t data_size, bool key_flag);
typedef int (*fSDK_ENC_DO_BUFFER_APPEND)(int buf_id, const void *data_piece, ssize_t data_piece_size);
typedef int (*fSDK_ENC_DO_BUFFER_COMMIT)(int buf_id);

typedef struct SDK_ENC_API {
	
	// enc buffering callback
	fSDK_ENC_DO_BUFFER_REQUEST do_buffer_request;
	fSDK_ENC_DO_BUFFER_APPEND do_buffer_append;
	fSDK_ENC_DO_BUFFER_COMMIT do_buffer_commit;

	int (*create_stream_h264)(int vin, int stream, LP_SDK_ENC_STREAM_H264_ATTR h264_attr);
	int (*release_stream_h264)(int vin, int stream);
	int (*enable_stream_h264)(int vin, int stream, bool flag);
	int (*set_stream_h264)(int vin, int stream, LP_SDK_ENC_STREAM_H264_ATTR h264_attr);
	int (*get_stream_h264)(int vin, int stream, LP_SDK_ENC_STREAM_H264_ATTR h264_attr);
	int (*request_stream_h264_keyframe)(int vin, int stream);
	
	int (*create_stream_h265)(int vin, int stream, LP_SDK_ENC_STREAM_H265_ATTR h265_attr);
	int (*release_stream_h265)(int vin, int stream);
	int (*enable_stream_h265)(int vin, int stream, bool flag);
	int (*set_stream_h265)(int vin, int stream, LP_SDK_ENC_STREAM_H265_ATTR h265_attr);
	int (*get_stream_h265)(int vin, int stream, LP_SDK_ENC_STREAM_H265_ATTR h265_attr);
	int (*request_stream_h265_keyframe)(int vin, int stream);

	int (*create_stream_g711a)(int ain, int vin_ref);
	int (*create_audio_stream)(int ain, int vin_ref, int type);
	int (*release_stream_g711a)(int ain);
	
	int (*start)(void);
	int (*stop)(void);
	
	int (*creat_snapshot_chn)(int vin, int vencChannelJPEG, ssize_t width, ssize_t height);
	int (*release_snapshot_chn)(int vin, int vencChannelJPEG);
	int (*snapshot)(int vin, enSDK_ENC_SNAPSHOT_QUALITY quality, ssize_t width, ssize_t height, FILE* stream);

	// overlay
	LP_SDK_ENC_VIDEO_OVERLAY_CANVAS (*create_overlay_canvas)(size_t width, size_t height);
	LP_SDK_ENC_VIDEO_OVERLAY_CANVAS (*load_overlay_canvas)(const char *bmp24_path);
	void (*release_overlay_canvas)(LP_SDK_ENC_VIDEO_OVERLAY_CANVAS canvas);

	int (*create_overlay)(int vin, int stream, const char* overlay_name,
		float x, float y, LP_SDK_ENC_VIDEO_OVERLAY_CANVAS const canvas);
	int (*release_overlay)(int vin, int stream, const char* overlay_name);

	LP_SDK_ENC_VIDEO_OVERLAY_CANVAS (*get_overlay_canvas)(int vin, int stream, const char* overlay_name);

	int (*show_overlay)(int vin, int stream, const char* overlayName, bool showFlag);
	int (*update_overlay)(int vin, int stream, const char* overlay_name);
	int (*eptz_ctrl)(int vin, int stream, int cmd, int param);
	int (*enc_mode)(int vin, int stream, int fix_mode, int show_mode);
	void *(*upgrade_env_prepare)(void *param);
	int (*update_overlay_bytext)(int vin, int stream, const char* text);	
	int (*enc_resolution)(int width, int height);

}stSDK_ENC_API, *lpSDK_ENC_API;

// could be used after 'SDK_init_enc' success to call
extern lpSDK_ENC_API sdk_enc;

extern int SDK_ENC_init();
extern int SDK_ENC_destroy();
extern int SDK_ENC_wdr_destroy();
extern int SDK_ENC_create_stream(int vin, int stream, LP_SDK_ENC_STREAM_ATTR stream_attr);
extern int SDK_ENC_release_stream(int vin, int stream);
extern int SDK_ENC_set_stream(int vin, int stream,LP_SDK_ENC_STREAM_ATTR stream_attr);
extern int SDK_ENC_get_stream(int vin, int stream, LP_SDK_ENC_STREAM_ATTR stream_attr);
extern int SDK_ENC_enable_stream(int vin, int stream, bool flag);
extern int SDK_ENC_request_stream_keyframe(int vin, int stream);
extern int SDK_ENC_eptz_ctrl(int nChn, int enCmd, unsigned char u8Param);
extern int SDK_ENC_get_enc_pts(int vin, int stream, unsigned long long *encPts);

#if defined(HI3518E_V2)
/*
* @brief 涓昏鐢ㄦ潵缁欏埌sdk灞傚綋鍓嶇殑璁惧鍨嬪彿锛屽尯鍒哖x鍜孋x鐨凲P鍊硷紝
         鍦⊿DK_ENC_init鍚庤皟鐢紝涓嶅缓璁敤浜庡叾瀹冨姛鑳�
* @param model emSDK_ENC_PRODUCT_MODEL_PX = 0,
               emSDK_ENC_PRODUCT_MODEL_CX,
*/


extern void SDK_ENC_set_model(int model);
#endif

#ifdef __cplusplus
};
#endif
#endif //SDK_ENC_H_

