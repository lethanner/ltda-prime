#pragma once

#define DSP_CORE_STATUS_REG 0xF405
//#define DSP_READBACK_START_REG 0x0014

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 3
#define DSP_FADER_SPDIF_ST 0x001C
#define DSP_FADER_BLUETOOTH_ST 0x0019
#define DSP_FADER_MASTER_ST 0x0085
#define DSP_FADER_REVERB_ST 0x0075
const short dsp_fader_address[] = { DSP_FADER_SPDIF_ST,
                                    DSP_FADER_BLUETOOTH_ST,
                                    DSP_FADER_MASTER_ST };

/* Регистры управления посылами */
#define DSP_SEND_SPDIF_REVERB_L 0x001E
#define DSP_SEND_SPDIF_REVERB_R 0x0020
#define DSP_SEND_BT_REVERB_L 0x001F
#define DSP_SEND_BT_REVERB_R 0x0021

#define DSP_SEND_SPDIF_MASTER_L 0x007E
#define DSP_SEND_SPDIF_MASTER_R 0x0081
#define DSP_SEND_BT_MASTER_L 0x007F
#define DSP_SEND_BT_MASTER_R 0x0082
#define DSP_SEND_RVRB_MASTER_L 0x0080
#define DSP_SEND_RVRB_MASTER_R 0x0083

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 6
//#define DSP_READBACK_LAST_STEREO 3
#define DSP_READB_SPDIF_L 0x0014
#define DSP_READB_SPDIF_R 0x0015
#define DSP_READB_BLUETOOTH_L 0x0016
#define DSP_READB_BLUETOOTH_R 0x0017
#define DSP_READB_MASTER_L 0x0087
#define DSP_READB_MASTER_R 0x0088
#define DSP_READB_REVERB_L 0x0072
#define DSP_READB_REVERB_R 0x0073
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