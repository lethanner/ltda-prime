#pragma once

#define DSP_CORE_STATUS_REG 0xF405
#define DSP_READBACK_START_REG 0x0014

#define DSP_READBACK_COUNT 4
#define DSP_FADER_COUNT 2

#define DSP_FADER_SPDIF_ST 0x0019
#define DSP_FADER_BLUETOOTH_ST 0x001C
const short dsp_fader_address[] = { DSP_FADER_SPDIF_ST, DSP_FADER_BLUETOOTH_ST };
#define FADER_SPDIF_ST 0
#define FADER_BLUETOOTH_ST 1