/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include <stdio.h>
#include <stdlib.h>

#include <xdc/std.h>

/***************************************************************************/
/* LTE Interelaver table with the coefficients P0,P1,P2 and G0 needed for  */
/* internal LTE coefficient generation.                                    */
/* K     f1    f2   P0    P1    P2    G0                                   */
/***************************************************************************/
Int16   TCP3_LteInterleaverTable [220][7] = {
      40,    3,   10,   30,   20,   10,   13,  //    0
      48,    7,   12,   36,   24,   12,   19,  //    1
      56,   19,   42,   42,   28,   14,    5,  //    2
      64,    7,   16,   48,   32,   16,   23,  //    3
      72,    7,   18,   54,   36,   18,   25,  //    4
      80,   11,   20,   60,   40,   20,   31,  //    5
      88,    5,   22,   22,   44,   66,   27,  //    6
      96,   11,   24,   72,   48,   24,   35,  //    7
     104,    7,   26,   78,   52,   26,   33,  //    8
     112,   41,   84,   28,   56,   84,   13,  //    9
     120,  103,   90,   90,   60,   30,   73,  //   10
     128,   15,   32,   96,   64,   32,   47,  //   11
     136,    9,   34,   34,   68,  102,   43,  //   12
     144,   17,  108,   36,   72,  108,  125,  //   13
     152,    9,   38,   38,   76,  114,   47,  //   14
     160,   21,  120,   40,   80,  120,  141,  //   15
     168,  101,   84,   42,   84,  126,   17,  //   16
     176,   21,   44,   44,   88,  132,   65,  //   17
     184,   57,   46,   46,   92,  138,  103,  //   18
     192,   23,   48,  144,   96,   48,   71,  //   19
     200,   13,   50,   50,  100,  150,   63,  //   20
     208,   27,   52,  156,  104,   52,   79,  //   21
     216,   11,   36,  162,  108,   54,   47,  //   22
     224,   27,   56,  168,  112,   56,   83,  //   23
     232,   85,   58,   58,  116,  174,  143,  //   24
     240,   29,   60,   60,  120,  180,   89,  //   25
     248,   33,   62,   62,  124,  186,   95,  //   26
     256,   15,   32,  192,  128,   64,   47,  //   27
     264,   17,  198,   66,  132,  198,  215,  //   28
     272,   33,   68,   68,  136,  204,  101,  //   29
     280,  103,  210,  210,  140,   70,   33,  //   30
     288,   19,   36,  216,  144,   72,   55,  //   31
     296,   19,   74,  222,  148,   74,   93,  //   32
     304,   37,   76,   76,  152,  228,  113,  //   33
     312,   19,   78,  234,  156,   78,   97,  //   34
     320,   21,  120,   80,  160,  240,  141,  //   35
     328,   21,   82,   82,  164,  246,  103,  //   36
     336,  115,   84,  252,  168,   84,  199,  //   37
     344,  193,   86,   86,  172,  258,  279,  //   38
     352,   21,   44,   88,  176,  264,   65,  //   39
     360,  133,   90,   90,  180,  270,  223,  //   40
     368,   81,   46,   92,  184,  276,  127,  //   41
     376,   45,   94,   94,  188,  282,  139,  //   42
     384,   23,   48,  288,  192,   96,   71,  //   43
     392,  243,   98,  294,  196,   98,  341,  //   44
     400,  151,   40,  300,  200,  100,  191,  //   45
     408,  155,  102,  306,  204,  102,  257,  //   46
     416,   25,   52,  104,  208,  312,   77,  //   47
     424,   51,  106,  318,  212,  106,  157,  //   48
     432,   47,   72,  324,  216,  108,  119,  //   49
     440,   91,  110,  330,  220,  110,  201,  //   50
     448,   29,  168,  112,  224,  336,  197,  //   51
     456,   29,  114,  114,  228,  342,  143,  //   52
     464,  247,   58,  348,  232,  116,  305,  //   53
     472,   29,  118,  118,  236,  354,  147,  //   54
     480,   89,  180,  120,  240,  360,  269,  //   55
     488,   91,  122,  366,  244,  122,  213,  //   56
     496,  157,   62,  124,  248,  372,  219,  //   57
     504,   55,   84,  378,  252,  126,  139,  //   58
     512,   31,   64,  384,  256,  128,   95,  //   59
     528,   17,   66,  132,  264,  396,   83,  //   60
     544,   35,   68,  408,  272,  136,  103,  //   61
     560,  227,  420,  420,  280,  140,   87,  //   62
     576,   65,   96,  144,  288,  432,  161,  //   63
     592,   19,   74,  444,  296,  148,   93,  //   64
     608,   37,   76,  152,  304,  456,  113,  //   65
     624,   41,  234,  156,  312,  468,  275,  //   66
     640,   39,   80,  480,  320,  160,  119,  //   67
     656,  185,   82,  164,  328,  492,  267,  //   68
     672,   43,  252,  504,  336,  168,  295,  //   69
     688,   21,   86,  172,  344,  516,  107,  //   70
     704,  155,   44,  528,  352,  176,  199,  //   71
     720,   79,  120,  540,  360,  180,  199,  //   72
     736,  139,   92,  552,  368,  184,  231,  //   73
     752,   23,   94,  564,  376,  188,  117,  //   74
     768,  217,   48,  192,  384,  576,  265,  //   75
     784,   25,   98,  196,  392,  588,  123,  //   76
     800,   17,   80,  200,  400,  600,   97,  //   77
     816,  127,  102,  612,  408,  204,  229,  //   78
     832,   25,   52,  208,  416,  624,   77,  //   79
     848,  239,  106,  636,  424,  212,  345,  //   80
     864,   17,   48,  216,  432,  648,   65,  //   81
     880,  137,  110,  220,  440,  660,  247,  //   82
     896,  215,  112,  672,  448,  224,  327,  //   83
     912,   29,  114,  228,  456,  684,  143,  //   84
     928,   15,   58,  696,  464,  232,   73,  //   85
     944,  147,  118,  708,  472,  236,  265,  //   86
     960,   29,   60,  240,  480,  720,   89,  //   87
     976,   59,  122,  732,  488,  244,  181,  //   88
     992,   65,  124,  248,  496,  744,  189,  //   89
    1008,   55,   84,  756,  504,  252,  139,  //   90
    1024,   31,   64,  768,  512,  256,   95,  //   91
    1056,   17,   66,  264,  528,  792,   83,  //   92
    1088,  171,  204,  816,  544,  272,  375,  //   93
    1120,   67,  140,  840,  560,  280,  207,  //   94
    1152,   35,   72,  864,  576,  288,  107,  //   95
    1184,   19,   74,  888,  592,  296,   93,  //   96
    1216,   39,   76,  912,  608,  304,  115,  //   97
    1248,   19,   78,  936,  624,  312,   97,  //   98
    1280,  199,  240,  960,  640,  320,  439,  //   99
    1312,   21,   82,  328,  656,  984,  103,  //  100
    1344,  211,  252, 1008,  672,  336,  463,  //  101
    1376,   21,   86,  344,  688, 1032,  107,  //  102
    1408,   43,   88, 1056,  704,  352,  131,  //  103
    1440,  149,   60,  360,  720, 1080,  209,  //  104
    1472,   45,   92,  368,  736, 1104,  137,  //  105
    1504,   49,  846,  376,  752, 1128,  895,  //  106
    1536,   71,   48, 1152,  768,  384,  119,  //  107
    1568,   13,   28,  392,  784, 1176,   41,  //  108
    1600,   17,   80,  400,  800, 1200,   97,  //  109
    1632,   25,  102,  408,  816, 1224,  127,  //  110
    1664,  183,  104, 1248,  832,  416,  287,  //  111
    1696,   55,  954, 1272,  848,  424, 1009,  //  112
    1728,  127,   96, 1296,  864,  432,  223,  //  113
    1760,   27,  110, 1320,  880,  440,  137,  //  114
    1792,   29,  112,  448,  896, 1344,  141,  //  115
    1824,   29,  114,  456,  912, 1368,  143,  //  116
    1856,   57,  116,  464,  928, 1392,  173,  //  117
    1888,   45,  354,  472,  944, 1416,  399,  //  118
    1920,   31,  120, 1440,  960,  480,  151,  //  119
    1952,   59,  610, 1464,  976,  488,  669,  //  120
    1984,  185,  124,  496,  992, 1488,  309,  //  121
    2016,  113,  420,  504, 1008, 1512,  533,  //  122
    2048,   31,   64, 1536, 1024,  512,   95,  //  123
    2112,   17,   66,  528, 1056, 1584,   83,  //  124
    2176,  171,  136, 1632, 1088,  544,  307,  //  125
    2240,  209,  420,  560, 1120, 1680,  629,  //  126
    2304,  253,  216,  576, 1152, 1728,  469,  //  127
    2368,  367,  444, 1776, 1184,  592,  811,  //  128
    2432,  265,  456,  608, 1216, 1824,  721,  //  129
    2496,  181,  468,  624, 1248, 1872,  649,  //  130
    2560,   39,   80, 1920, 1280,  640,  119,  //  131
    2624,   27,  164, 1968, 1312,  656,  191,  //  132
    2688,  127,  504, 2016, 1344,  672,  631,  //  133
    2752,  143,  172, 2064, 1376,  688,  315,  //  134
    2816,   43,   88, 2112, 1408,  704,  131,  //  135
    2880,   29,  300,  720, 1440, 2160,  329,  //  136
    2944,   45,   92,  736, 1472, 2208,  137,  //  137
    3008,  157,  188,  752, 1504, 2256,  345,  //  138
    3072,   47,   96, 2304, 1536,  768,  143,  //  139
    3136,   13,   28,  784, 1568, 2352,   41,  //  140
    3200,  111,  240, 2400, 1600,  800,  351,  //  141
    3264,  443,  204, 2448, 1632,  816,  647,  //  142
    3328,   51,  104, 2496, 1664,  832,  155,  //  143
    3392,   51,  212, 2544, 1696,  848,  263,  //  144
    3456,  451,  192, 2592, 1728,  864,  643,  //  145
    3520,  257,  220,  880, 1760, 2640,  477,  //  146
    3584,   57,  336,  896, 1792, 2688,  393,  //  147
    3648,  313,  228,  912, 1824, 2736,  541,  //  148
    3712,  271,  232, 2784, 1856,  928,  503,  //  149
    3776,  179,  236, 2832, 1888,  944,  415,  //  150
    3840,  331,  120, 2880, 1920,  960,  451,  //  151
    3904,  363,  244, 2928, 1952,  976,  607,  //  152
    3968,  375,  248, 2976, 1984,  992,  623,  //  153
    4032,  127,  168, 3024, 2016, 1008,  295,  //  154
    4096,   31,   64, 3072, 2048, 1024,   95,  //  155
    4160,   33,  130, 1040, 2080, 3120,  163,  //  156
    4224,   43,  264, 3168, 2112, 1056,  307,  //  157
    4288,   33,  134, 1072, 2144, 3216,  167,  //  158
    4352,  477,  408, 1088, 2176, 3264,  885,  //  159
    4416,   35,  138, 3312, 2208, 1104,  173,  //  160
    4480,  233,  280, 1120, 2240, 3360,  513,  //  161
    4544,  357,  142, 1136, 2272, 3408,  499,  //  162
    4608,  337,  480, 1152, 2304, 3456,  817,  //  163
    4672,   37,  146, 1168, 2336, 3504,  183,  //  164
    4736,   71,  444, 3552, 2368, 1184,  515,  //  165
    4800,   71,  120, 3600, 2400, 1200,  191,  //  166
    4864,   37,  152, 1216, 2432, 3648,  189,  //  167
    4928,   39,  462, 3696, 2464, 1232,  501,  //  168
    4992,  127,  234, 3744, 2496, 1248,  361,  //  169
    5056,   39,  158, 3792, 2528, 1264,  197,  //  170
    5120,   39,   80, 3840, 2560, 1280,  119,  //  171
    5184,   31,   96, 3888, 2592, 1296,  127,  //  172
    5248,  113,  902, 1312, 2624, 3936, 1015,  //  173
    5312,   41,  166, 1328, 2656, 3984,  207,  //  174
    5376,  251,  336, 4032, 2688, 1344,  587,  //  175
    5440,   43,  170, 4080, 2720, 1360,  213,  //  176
    5504,   21,   86, 1376, 2752, 4128,  107,  //  177
    5568,   43,  174, 4176, 2784, 1392,  217,  //  178
    5632,   45,  176, 1408, 2816, 4224,  221,  //  179
    5696,   45,  178, 1424, 2848, 4272,  223,  //  180
    5760,  161,  120, 1440, 2880, 4320,  281,  //  181
    5824,   89,  182, 1456, 2912, 4368,  271,  //  182
    5888,  323,  184, 4416, 2944, 1472,  507,  //  183
    5952,   47,  186, 4464, 2976, 1488,  233,  //  184
    6016,   23,   94, 4512, 3008, 1504,  117,  //  185
    6080,   47,  190, 4560, 3040, 1520,  237,  //  186
    6144,  263,  480, 4608, 3072, 1536,  743,   //  187

/*********FOLLOWING PART IS EXTENTION TO 8192 with step 64 TO TEST TCP3 *************/
     6208,     3,   194,  4656,  3104,  1552,   197,     //     188
     6272,     3,    14,  4704,  3136,  1568,    17,     //     189
     6336,     5,    66,  1584,  3168,  4752,    71,     //     190
     6400,     3,    10,  4800,  3200,  1600,    13,     //     191
     6464,     3,   202,  4848,  3232,  1616,   205,     //     192
     6528,     5,   102,  1632,  3264,  4896,   107,     //     193
     6592,     3,   206,  4944,  3296,  1648,   209,     //     194
     6656,     3,    26,  4992,  3328,  1664,    29,     //     195
     6720,    11,   210,  5040,  3360,  1680,   221,     //     196
     6784,     3,   106,  5088,  3392,  1696,   109,     //     197
     6848,     3,   214,  5136,  3424,  1712,   217,     //     198
     6912,     5,     6,  1728,  3456,  5184,    11,     //     199
     6976,     3,   218,  5232,  3488,  1744,   221,     //     200
     7040,     3,   110,  5280,  3520,  1760,   113,     //     201
     7104,     5,   222,  1776,  3552,  5328,   227,     //     202
     7168,     3,    14,  5376,  3584,  1792,    17,     //     203
     7232,     3,   226,  5424,  3616,  1808,   229,     //     204
     7296,     5,   114,  1824,  3648,  5472,   119,     //     205
     7360,     3,   230,  5520,  3680,  1840,   233,     //     206
     7424,     3,    58,  5568,  3712,  1856,    61,     //     207
     7488,     5,    78,  1872,  3744,  5616,    83,     //     208
     7552,     3,   118,  5664,  3776,  1888,   121,     //     209
     7616,     3,   238,  5712,  3808,  1904,   241,     //     210
     7680,     7,    30,  5760,  3840,  1920,    37,     //     211
     7744,     3,    22,  5808,  3872,  1936,    25,     //     212
     7808,     3,   122,  5856,  3904,  1952,   125,     //     213
     7872,     5,   246,  1968,  3936,  5904,   251,     //     214
     7936,     3,    62,  5952,  3968,  1984,    65,     //     215
     8000,     3,    10,  6000,  4000,  2000,    13,     //     216
     8064,     5,    42,  2016,  4032,  6048,    47,     //     217
     8128,     3,   254,  6096,  4064,  2032,   257,     //     218
     8192,     3,     2,  6144,  4096,  2048,     5,     //     219
};

Int16 TCP3_WimaxInterleaverTable[17][4] ={
      20,     18,    11,       4,  //  0
       8,     12,    23,      34,  //  1
       4,     14,    27,      40,  //  2
      44,     54,    23,       4,  //  3
      28,      8,    39,      46,  //  4
      44,     12,    79,      90,  //  5
      52,     14,    27,      40,  //  6
      68,     20,   107,     126,  //  7
      44,     12,    23,      34,  //  8
      44,     12,    71,      82,  //  9
      52,     14,    27,      40,  // 10
      52,     14,    87,     100,  // 11
     212,    356,   119,     402,  // 12
     172,    588,   387,     474,  // 13
     172,     44,   447,    1390,  // 14
     124,   1000,    87,    1070,  // 15
     212,   1320,   131,    1362   // 16
};

/*WiMAX frame lengths*/
Int32 WIMAX_FRAME_LENGTHS[17] = {
          48, /*  0:   6 Bytes */
          72, /*  1:   9 Bytes */
          96, /*  2:  12 Bytes */
         144, /*  3:  18 Bytes */
         192, /*  4:  24 Bytes */
         216, /*  5:  27 Bytes */
         240, /*  6:  30 Bytes */
         288, /*  7:  36 Bytes */
         360, /*  8:  45 Bytes */
         384, /*  9:  48 Bytes */
         432, /* 10:  54 Bytes */
         480, /* 11:  60 Bytes */
         960, /* 12: 120 Bytes */
        1920, /* 13: 240 Bytes */
        2880, /* 14: 360 Bytes */
        3840, /* 15: 480 Bytes */
        4800  /* 16: 600 Bytes */
};

/*
 * Calculates the LTE interleaver table index for given block length K
 */
Int32 LTE_interleaver_index(Int32 K)
{
    Int32 ind = K;

    if ( K <= 512u )
    {
        ind = ( K >> 3 ) - 5u;
    }
    else if ( K <= 1024u )
    {
        ind = ( K >> 4 ) + 27u;
    }
    else if ( K <= 2048u )
    {
        ind = ( K >> 5 ) + 59u;
    }
    else if ( K <= 8192u )
    {
        ind = ( K >> 6 ) + 91u;
    }

    if ( ind == K )
    {
        printf("%s\n"," Error: Unsupported block length ");
        exit(0);
    }

    return (ind);
}

/*
 * Calculates the WIMAX interleaver table index for given block length K
 */
Int32 WIMAX_interleaver_index(Int32 K)
{
    Int32 ind = K, i;

    for (i = 0; i < 17; ++i)
    {
        if ( K == WIMAX_FRAME_LENGTHS[i] )
        {
            ind = i;
        }
    }

    if ( ind == K )
    {
        printf("%s\n"," Error: Unsupported block length ");
        exit(0);
    }

    return (ind);
}
