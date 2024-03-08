#pragma once

#define DSP_CORE_STATUS_REG 0xF405
//#define DSP_READBACK_START_REG 0x0014

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 3
#define DSP_FADER_SPDIF_ST 0x007B
#define DSP_FADER_BLUETOOTH_ST 0x007C
#define DSP_FADER_MASTER_ST 0x0085
#define DSP_FADER_REVERB_ST 0x0084
#define DSP_SEND_SPDIF_MASTER 0x007D
#define DSP_SEND_BT_MASTER 0x007E
#define DSP_SEND_SPDIF_REVERB 0x0080
#define DSP_SEND_BT_REVERB 0x007F
const short dsp_fader_address[] = { DSP_FADER_SPDIF_ST,
                                    DSP_FADER_BLUETOOTH_ST,
                                    DSP_FADER_MASTER_ST };

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 6
//#define DSP_READBACK_LAST_STEREO 3
#define DSP_READB_SPDIF_L 0x0014
#define DSP_READB_SPDIF_R 0x0015
#define DSP_READB_BLUETOOTH_L 0x0016
#define DSP_READB_BLUETOOTH_R 0x0017
#define DSP_READB_MASTER_L 0x0079
#define DSP_READB_MASTER_R 0x007A
#define DSP_READB_REVERB_L 0x006A
#define DSP_READB_REVERB_R 0x006B
const short dsp_readback_addr[] = { DSP_READB_SPDIF_L,
                                    DSP_READB_SPDIF_R,
                                    DSP_READB_BLUETOOTH_L,
                                    DSP_READB_BLUETOOTH_R,
                                    DSP_READB_MASTER_L,
                                    DSP_READB_MASTER_R };

/* Внутренние идентификаторы каналов */
#define FADER_SPDIF_ST 0
#define FADER_BLUETOOTH_ST 1
#define FADER_MASTER_ST 2