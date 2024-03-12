#pragma once

#define DSP_CORE_STATUS_REG 0xF405
//#define DSP_READBACK_START_REG 0x0014

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 4
const short dsp_fader_address[] = {
    0x001C,  // SPDIF стерео
    0x0019,  // Bluetooth стерео
    0x0075,  // Reverb стерео
    0x0085   // Master стерео
};

/* Регистры управления посылами */
#define DSP_BUS_COUNT 2
#define DSP_BUS_CHANNELS 6
const short dsp_bus_send_addr[][DSP_BUS_CHANNELS] = {
    {
      0x007E,  // SPDIF -> Master левый
      0x0081,  // SPDIF -> Master правый
      0x007F,  // Bluetooth -> Master левый
      0x0082,  // Bluetooth -> Master правый
      0x0080,  // Reverb -> Master левый
      0x0083   // Reverb -> Master правый
    },
    {
      0x001E,  // SPDIF -> Reverb левый
      0x0020,  // SPDIF -> Reverb правый
      0x001F,  // Bluetooth -> Reverb левый
      0x0021,  // Bluetooth -> Reverb правый

      // * включается трек pi14cyber - Костыль *
      // лучше лишний раз поменять не тот уровень, чем положить SigmaDSP
      // при попытке записи чего-то в неизвестно какой адрес
      0x0080,  // Reverb -> Reverb нельзя
      0x0083   // Reverb -> Reverb всё ещё нельзя
    }
};

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 8
const short dsp_readback_addr[] = {
    0x0014,  // SPDIF левый
    0x0015,  // SPDIF правый
    0x0016,  // Bluetooth левый
    0x0017,  // Bluetooth правый
    0x0072,  // Reverb левый
    0x0073,  // Reverb правый
    0x0087,  // Master левый
    0x0088   // Master правый
};

/* Внутренние идентификаторы каналов */
#define FADER_SPDIF_ST 0
#define FADER_BLUETOOTH_ST 1
#define FADER_REVERB_ST 2
#define FADER_MASTER_ST 3

/* Внутренние идентификаторы посылов */
#define SEND_TO_MASTER 0
#define SEND_TO_REVERB 1