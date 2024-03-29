#pragma once

#define DSP_CORE_STATUS_REG 0xF405
//#define DSP_READBACK_START_REG 0x0014

/* Регистры управления громкостью */
#define DSP_FADER_COUNT 5
const short dsp_fader_address[] = {
    0x001E,  // SPDIF стерео
    0x001B,  // Bluetooth стерео
    0x0021,  // USB стерео
    0x0081,  // Reverb стерео
    0x008C   // Master стерео
};

/* Регистры управления посылами */
#define DSP_BUS_COUNT 2
#define DSP_BUS_CHANNELS 8
const short dsp_bus_send_addr[][DSP_BUS_CHANNELS] = {
    {
      0x0083,  // SPDIF -> Master левый
      0x0087,  // SPDIF -> Master правый
      0x0084,  // Bluetooth -> Master левый
      0x0088,  // Bluetooth -> Master правый
      0x0085,  // USB -> Master левый
      0x0089,  // USB -> Master правый
      0x0086,  // Reverb -> Master левый
      0x008A   // Reverb -> Master правый
    },
    {
      0x002A,  // SPDIF -> Reverb левый
      0x002D,  // SPDIF -> Reverb правый
      0x002B,  // Bluetooth -> Reverb левый
      0x002E,  // Bluetooth -> Reverb правый
      0x002C,  // USB -> Reverb левый
      0x002F,  // USB -> Reverb правый

      // * включается трек pi14cyber - Костыль *
      // лучше лишний раз поменять не тот уровень, чем положить SigmaDSP
      // при попытке записи чего-то в неизвестно какой адрес
      0x002A,  // Reverb -> Reverb нельзя
      0x002D   // Reverb -> Reverb всё ещё нельзя
    }
};

/* Регистры считывания данных об уровне сигнала */
#define DSP_READBACK_COUNT 10
const short dsp_readback_addr[] = {
    0x0014,  // SPDIF левый
    0x0015,  // SPDIF правый
    0x0016,  // Bluetooth левый
    0x0017,  // Bluetooth правый
    0x0018,  // USB левый
    0x0019,  // USB правый
    0x007E,  // Reverb левый
    0x007F,  // Reverb правый
    0x008E,  // Master левый
    0x008F   // Master правый
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