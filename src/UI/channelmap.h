#pragma once
#define CH_GROUP_COUNT 4

// карты каналов
const unsigned char chan_inputs[] = { 0, 1 };
const unsigned char chan_test[] = { 0, 1, 1, 0, 0 };
const unsigned char chan_outs[] = { 2 };
const unsigned char chan_all[] = { 0, 1, 2 };

// группы каналов
const unsigned char* const ch_groups[] = {
    chan_inputs, chan_test, chan_outs, chan_all
};
const unsigned char ch_count[] = { 2, 5, 1, 3 };

extern const char chan_labels[][7];
extern const char* grp_labels[];