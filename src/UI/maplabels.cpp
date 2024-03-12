#include "channelmap.h"

// подписи на каналах
const char chan_labels[][7] = {
    "SPDIF",
    "BT",
    "Reverb",
    "Master"
};

// подписи групп
const char* grp_labels[] = {
    "Inputs",
    "FX",
    "Outputs",
    "All"
};

// подписи шин для посылов
const char* sendto_labels[] = {
    // со входов на все шины
    "Master",
    "Reverb"
};