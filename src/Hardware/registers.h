#pragma once

#define DSP_CORE_STATUS_REG 0xF405

#define DSP_BASSBOOST_REG 0x6016
#define DSP_BB_INTENSITY_REG 0x600B
#define DSP_BB_GAIN_REG 0x6014

#define DSP_REVERB_TIME_REG 0x6008
#define DSP_REVERB_HFDAMP_REG 0x00A1
#define DSP_REVERB_BGAIN_REG 0x00A3

/* 
 * TODO: переписать это говно на структуры
*/

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 7
#define DSP_STEREO_BEFORE 7
const short dsp_fader_address[DSP_FADER_COUNT * 2] = {
    0x0037,  // SPDIF левый
    0x003A,  // SPDIF правый
    0x003D,  // Bluetooth левый
    0x0040,  // Bluetooth правый
    0x0043,  // USB левый
    0x0046,  // USB правый
    0x00AE,  // Reverb левый
    0x00B1,  // Reverb правый
    0x0124,  // Master левый
    0x0127,  // Master правый
    0x011E,  // Submix левый
    0x0121,  // Submix правый
    0x00B4,  // Pitch левый
    0x00B7   // Pitch правый
};

/* Регистры стереорежимов */
#define DSP_FULL_STEREOMODE_TO 3
const short dsp_stereomode_address[] = {
    0x012D,  // SPDIF
    0x012E,  // Bluetooth
    0x012F,  // USB
    0x0128,  // тут стоит Reverb, но у него нет такого переключателя, поэтому заглушка
    0x013B,  // Master
    0x013A   // Submix
};

/* Регистры управления посылами */
#define DSP_BUS_COUNT 4     // количество шин
#define DSP_BUS_CHANNELS 5  // количество каналов в шине
#define DSP_IN_TO_BUS 3  // количество источников входного сигнала
#define DSP_OUT_BUS_BEFORE 2 // количество шин выхода сигнала (стоят в начале!)
const short dsp_bus_send_addr[DSP_BUS_COUNT][DSP_BUS_CHANNELS * 2] = {
    {
     0x00B9, // SPDIF -> Master левый
     0x00C4, // SPDIF -> Master правый
     0x00BB, // Bluetooth -> Master левый
     0x00C6, // Bluetooth -> Master правый
     0x00BD, // USB -> Master левый
     0x00C8, // USB -> Master правый
     0x00BF, // Reverb -> Master левый
     0x00CA, // Reverb -> Master правый
     0x00C1, // Pitch -> Master левый
     0x00CC   // Pitch -> Master правый
    },
    {
     0x00CD, // SPDIF -> Submix левый
     0x00D8, // SPDIF -> Submix правый
     0x00CF, // Bluetooth -> Submix левый
     0x00DA, // Bluetooth -> Submix правый
     0x00D1, // USB -> Submix левый
     0x00DC, // USB -> Submix правый
     0x00D3, // Reverb -> Submix левый
     0x00DE, // Reverb -> Submix правый
     // костыль, но чутка надежнее и удобнее
     0x00D5, // Pitch -> Submix левый
     0x00E0   // Pitch -> Submix правый
    },
    {
     0x0048, // SPDIF -> Reverb левый
     0x004F, // SPDIF -> Reverb правый
     0x004A, // Bluetooth -> Reverb левый
     0x0051, // Bluetooth -> Reverb правый
     0x004C, // USB -> Reverb левый
     0x0053, // USB -> Reverb правый

     // * включается трек pi14cyber - Костыль *
     // лучше лишний раз поменять не тот уровень, чем положить SigmaDSP
     // при попытке записи чего-то в неизвестно какой адрес
     0x00D3, // Reverb -> Reverb нельзя
     0x00DE, // Reverb -> Reverb всё ещё нельзя
     0x00D3, // Pitch -> Reverb нельзя
     0x00DE   // аналогично
    },
    {
     0x0054, // SPDIF -> Pitch левый
     0x0055, // SPDIF -> Pitch правый
     0x0056, // Bluetooth -> Pitch левый
     0x0057, // Bluetooth -> Pitch правый
     0x0058, // USB -> Pitch левый
     0x0059, // USB -> Pitch правый
     0x00D5, // искл. Pitch -> Reverb
     0x00E0, // искл. Pitch -> Reverb
     0x00D5, // искл. Pitch -> Pitch
     0x00E0   // искл. Pitch -> Pitch
    }
};

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 13
const short dsp_readback_addr[DSP_READBACK_COUNT] = {
    0x0030,  // SPDIF левый
    0x0031,  // SPDIF правый
    0x0032,  // Bluetooth левый
    0x0033,  // Bluetooth правый
    0x0034,  // USB левый
    0x0035,  // USB правый
    0x00AA,  // Reverb левый
    0x00AB,  // Reverb правый
    0x012B,  // Master левый
    0x012C,  // Master правый
    0x0129,  // Submix левый
    0x012A,  // Submix правый
    0x00AC   // Pitch моно
};

/* Внутренние идентификаторы каналов */
#define FADER_SPDIF_ST 0
#define FADER_BLUETOOTH_ST 1
#define FADER_USB_ST 2
#define FADER_REVERB_ST 3
#define FADER_MASTER_ST 4
#define FADER_SUBMIX_ST 5
#define FADER_PITCH 6

/* Внутренние идентификаторы посылов */
#define SEND_TO_MASTER 0
#define SEND_TO_SUBMIX 1
#define SEND_TO_REVERB 2
#define SEND_TO_PITCH 3