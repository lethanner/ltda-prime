#pragma once

#define DSP_CORE_STATUS_REG 0xF405
#define DSP_BASSBOOST_REG 0x6016
#define DSP_BB_INTENSITY_REG 0x600B
#define DSP_BB_GAIN_REG 0x6014
#define DSP_REVERB_TIME_REG 0x6008
#define DSP_REVERB_HFDAMP_REG 0x0079
#define DSP_REVERB_BGAIN_REG 0x007B
//#define DSP_READBACK_START_REG 0x0014

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 5
const short dsp_fader_address[] = {
    0x0022,  // SPDIF стерео
    0x001F,  // Bluetooth стерео
    0x0025,  // USB стерео
    0x0085,  // Reverb стерео
    0x0090   // Master стерео
};

/* Регистры управления посылами */
#define DSP_BUS_COUNT 2
#define DSP_BUS_CHANNELS 4
#define DSP_IN_TO_BUS 3
const short dsp_bus_send_addr[][DSP_BUS_CHANNELS * 2] = {
    {
     0x0087, // SPDIF -> Master левый
     0x008B, // SPDIF -> Master правый
     0x0088, // Bluetooth -> Master левый
     0x008C, // Bluetooth -> Master правый
     0x0089, // USB -> Master левый
     0x008D, // USB -> Master правый
     0x008A, // Reverb -> Master левый
     0x008E   // Reverb -> Master правый
    },
    {
     0x002E, // SPDIF -> Reverb левый
     0x0031, // SPDIF -> Reverb правый
     0x002F, // Bluetooth -> Reverb левый
     0x0032, // Bluetooth -> Reverb правый
     0x0030, // USB -> Reverb левый
     0x0033, // USB -> Reverb правый

     // * включается трек pi14cyber - Костыль *
     // лучше лишний раз поменять не тот уровень, чем положить SigmaDSP
     // при попытке записи чего-то в неизвестно какой адрес
     0x008A, // Reverb -> Reverb нельзя
     0x008E   // Reverb -> Reverb всё ещё нельзя
    }
};

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 10
const short dsp_readback_addr[] = {
    0x0018,  // SPDIF левый
    0x0019,  // SPDIF правый
    0x001A,  // Bluetooth левый
    0x001B,  // Bluetooth правый
    0x001C,  // USB левый
    0x001D,  // USB правый
    0x0082,  // Reverb левый
    0x0083,  // Reverb правый
    0x00CA,  // Master левый
    0x00CB   // Master правый
};

/* Внутренние идентификаторы каналов */
#define FADER_SPDIF_ST 0
#define FADER_BLUETOOTH_ST 1
#define FADER_USB_ST 2
#define FADER_REVERB_ST 3
#define FADER_MASTER_ST 4

/* Внутренние идентификаторы посылов */
#define SEND_TO_MASTER 0
#define SEND_TO_REVERB 1