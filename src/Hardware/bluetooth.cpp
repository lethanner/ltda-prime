#include "bluetooth.h"
#include "../config.h"

void LTDABT::init()
{
	i2s_pin_config_t my_pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 23,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    set_pin_config(my_pin_config);
    start(BLUETOOTH_NAME);
}

void LTDABT::sendAVRCPVolume(int8_t volume)
{
	volume_set_by_local_host(map(volume, -97, 0, 0, 127));
}

LTDABT bluetooth;