#pragma once
#define CH_GROUP_COUNT 4

// карты каналов
const unsigned char chan_inputs[] = { 0, 1, 2 };
const unsigned char chan_fx[] = { 3 };
const unsigned char chan_outs[] = { 4 };
const unsigned char chan_all[] = { 0, 1, 2, 3, 4 };

// группы каналов
const unsigned char* const ch_groups[] = {
    chan_inputs, chan_fx, chan_outs, chan_all
};
const unsigned char ch_count[] = { 3, 1, 1, 5 };

extern const char chan_labels[][7];
extern const char* grp_labels[];
extern const char* sendto_labels[];