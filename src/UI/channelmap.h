#pragma once
#define CH_GROUP_COUNT 3

// карты каналов
const unsigned char chan_stereo[] = { 0, 1 };
const unsigned char chan_test[] = { 0, 1, 1, 0, 0 };
const unsigned char chan_single[] = { 0 };

// группы каналов
const unsigned char* const ch_groups[] = {
    chan_stereo, chan_test, chan_single
};
const unsigned char ch_count[] = { 2, 5, 1 };

extern const char chan_labels[][6];
extern const char* grp_labels[];