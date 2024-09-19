/* Ещё таблицы перевода уровней сигнала в децибелы */
#pragma once
#include <Arduino.h>

/* для отображения уровня на индикаторе */
// -inf, -57, -51, -45, -39, -33, -27, -21, -15, -10, -6, -2, 0 дБ
const DRAM_ATTR uint32_t db_calibr_ledmonitor[] = {
    0,       23698,   47285,   94345,   188243,   375595,  749411,
    1495271, 2983458, 5305422, 8408526, 13326616, 16777215
};

/* для отображения столбиков на дисплее */
// -inf, -82, -80, -78, -76, -74, -72, -70, -68, -66, -64, -62, -60, -58,
// -56, -54, -52, -50, -48, -46, -44, -42, -40, -38, -36, -34, -32, -30,
// -28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0 дБ
const DRAM_ATTR uint32_t db_calibr_onscreen[] = {
    0,       1333,    1678,    2112,    2659,     3347,     4214,    5305,    6679,
    8409,    10586,   13327,   16777,   21121,    26590,    33475,   42142,   53054,
    66791,   84085,   105857,  133266,  167772,   211213,   265901,  334749,  421425,
    530542,  667913,  840853,  1058571, 1332662,  1677722,  2112126, 2659010, 3347495,
    4214246, 5305422, 6679130, 8408526, 10585708, 13326616, 16777215
};