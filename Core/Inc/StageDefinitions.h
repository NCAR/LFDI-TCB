#ifndef INC_STAGEDEFINITIONS_H_
#define INC_STAGEDEFINITIONS_H_

#define STAGE1LOWER (656.092)
#define STAGE1UPPER (656.521)
#define STAGE1 (2.7)
#define STAGE2LOWER (656.127)
#define STAGE2UPPER (656.512)
#define STAGE2 (5.4)
#define STAGE3LOWER (656.277)
#define STAGE3UPPER (656.412)
#define STAGE3 (10.8)
#define STAGE4LOWER (656.004)
#define STAGE4UPPER (656.992)
#define STAGE4 (2.7)
#define STAGE5LOWER (656.006)
#define STAGE5UPPER (656.368)
#define STAGE5 (5.4)
#define STAGE6LOWER (656.1)
#define STAGE6UPPER (656.214)
#define STAGE6 (10.8)
#define STAGE1FSR (.42)
#define STAGE2FSR (.21)
#define STAGE3FSR (.105)
#define STAGE4FSR (.42)
#define STAGE5FSR (.21)
#define STAGE6FSR (.105)
#define DefaultSlope (0.04)
#define DefaultIntercept (-655.27)

struct sStage {
	float stageSize;
	float stageUpper;
	float stageLower;
	float freeSpectralRange;
	uint16_t *lookuptable;
	float slope;
	float intercept;
};


static uint16_t lookup27[1000] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1942,2282,2325,2218,2105,2104,2178,2247,2286,2303,2306,2301,2298,2301,2317,2341,2369,2397,2420,2438,2453,2464,2473,2480,2487,2494,2501,2510,2520,2531,2543,2555,2568,2581,2593,2606,2619,2631,2643,2654,2666,2677,2688,2698,2708,2718,2728,2737,2746,2755,2764,2773,2782,2791,2800,2808,2817,2827,2836,2845,2854,2863,2873,2882,2892,2901,2910,2920,2930,2939,2948,2958,2967,2976,2985,2994,3002,3010,3019,3027,3034,3042,3050,3057,3065,3072,3079,3087,3094,3102,3109,3117,3125,3132,3140,3148,3156,3163,3171,3179,3186,3194,3202,3209,3216,3224,3231,3239,3246,3254,3261,3269,3277,3285,3293,3302,3310,3319,3328,3337,3347,3356,3365,3374,3384,3393,3401,3410,3419,3427,3435,3443,3451,3459,3466,3473,3481,3488,3494,3501,3507,3514,3520,3526,3532,3538,3544,3550,3557,3563,3570,3577,3584,3592,3600,3608,3616,3625,3634,3644,3653,3663,3672,3682,3692,3701,3711,3720,3729,3739,3748,3757,3766,3775,3784,3793,3802,3812,3821,3830,3839,3848,3858,3867,3876,3885,3895,3904,3913,3923,3932,3941,3951,3960,3969,3978,3987,3996,4005,4014,4023,4031,4040,4049,4058,4068,4077,4087,4097,4108,4119,4130,4142,4153,4165,4177,4189,4201,4212,4224,4236,4247,4258,4270,4281,4293,4304,4316,4328,4340,4351,4363,4374,4386,4397,4408,4418,4429,4440,4451,4463,4475,4487,4501,4515,4530,4545,4561,4577,4593,4608,4624,4639,4655,4670,4685,4700,4716,4731,4746,4762,4777,4793,4808,4823,4838,4854,4869,4885,4900,4916,4932,4947,4963,4978,4993,5007,5021,5035,5049,5065,5081,5100,5121,5143,5167,5192,5217,5241,5265,5287,5306,5322,5337,5350,5364,5379,5396,5416,5439,5463,5487,5511,5535,5558,5580,5603,5626,5649,5672,5695,5718,5741,5764,5787,5811,5836,5860,5883,5902,5918,5936,5959,5991,6037,6085,6119,6140,6156,6177,6212,6260,6304,6333,6351,6368,6393,6431,6480,6530,6573,6607,6630,6648,6668,6697,6738,6787,6835,6881,6926,6972,7019,7065,7110,7155,7203,7255,7297,7315,7321,7334,7371,7449,7549,7622,7661,7716,7811,7904,7959,7995,8037,8085,8135,8181,8220,8262,8328,8421,8519,8610,8703,8804,8891,8942,8983,9053,9151,9252,9345,9429,9513,9624,9768,9902,9994,10083,10215,10360,10468,10552,10662,10805,10946,11077,11229,11417,11600,11744,11876,12033,12227,12455,12692,12894,13069,13273,13538,13852,14189,14496,14746,15034,15460,15945,16343,16648,16949,17336,17899,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t lookup54[1000] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15199,14709,14269,13873,13512,13181,12872,12577,12290,12003,11715,11430,11156,10899,10662,10446,10246,10059,9882,9714,9553,9399,9250,9105,8965,8829,8699,8573,8453,8340,8233,8134,8040,7950,7860,7768,7672,7576,7484,7399,7323,7255,7189,7122,7051,6977,6904,6838,6782,6737,6697,6653,6599,6530,6454,6379,6316,6272,6247,6230,6214,6190,6153,6106,6054,5999,5944,5893,5847,5810,5782,5762,5743,5720,5687,5642,5592,5542,5499,5466,5442,5423,5405,5386,5364,5340,5315,5291,5268,5245,5222,5199,5176,5153,5130,5107,5084,5061,5038,5015,4991,4968,4945,4922,4899,4877,4854,4831,4807,4783,4758,4734,4712,4693,4679,4667,4658,4649,4640,4631,4619,4605,4587,4565,4541,4516,4490,4465,4441,4419,4399,4382,4367,4355,4344,4334,4324,4315,4306,4296,4285,4273,4261,4249,4236,4224,4213,4203,4193,4184,4176,4167,4157,4146,4133,4118,4099,4077,4053,4029,4006,3986,3968,3954,3941,3930,3921,3913,3906,3899,3893,3886,3879,3872,3865,3858,3851,3843,3835,3827,3818,3809,3799,3789,3778,3767,3756,3744,3732,3720,3707,3695,3682,3670,3658,3646,3634,3623,3612,3602,3593,3585,3577,3571,3564,3558,3553,3548,3544,3540,3536,3532,3529,3525,3521,3518,3514,3510,3506,3502,3497,3491,3486,3480,3473,3466,3459,3451,3443,3434,3424,3414,3404,3393,3382,3370,3358,3346,3334,3323,3311,3300,3290,3281,3271,3263,3255,3247,3240,3232,3226,3219,3213,3206,3200,3194,3188,3181,3175,3168,3161,3154,3146,3138,3130,3121,3111,3100,3089,3078,3066,3054,3041,3030,3018,3007,2997,2988,2980,2972,2965,2959,2953,2948,2943,2938,2933,2929,2925,2920,2915,2911,2906,2900,2894,2888,2881,2874,2866,2858,2850,2842,2834,2826,2817,2809,2800,2792,2784,2775,2767,2759,2751,2744,2736,2728,2721,2714,2707,2700,2693,2687,2680,2674,2667,2660,2653,2645,2638,2629,2620,2611,2600,2589,2578,2566,2553,2541,2529,2518,2507,2497,2488,2480,2472,2465,2458,2452,2445,2439,2431,2424,2416,2407,2397,2386,2374,2362,2350,2337,2325,2314,2303,2294,2286,2277,2269,2259,2249,2236,2220,2201,2179,2154,2130,2107,2089,2063,2010,1912,1749,1500,1147,669,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t lookup108[1000] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15841,14374,13130,12084,11209,10478,9867,9348,8896,8484,8097,7736,7405,7107,6844,6615,6418,6250,6109,5988,5878,5772,5659,5538,5415,5299,5196,5112,5042,4979,4917,4850,4778,4705,4636,4576,4524,4479,4437,4395,4352,4306,4259,4210,4160,4111,4063,4020,3982,3951,3925,3904,3886,3870,3855,3841,3826,3809,3790,3768,3745,3720,3696,3673,3650,3627,3603,3579,3555,3531,3509,3490,3472,3452,3426,3391,3347,3297,3247,3203,3169,3144,3124,3107,3088,3068,3046,3023,2999,2975,2950,2925,2900,2876,2853,2830,2807,2784,2761,2738,2715,2692,2668,2645,2622,2599,2576,2552,2529,2506,2497,2483,2460,2437,2413,2390,2367,2343,2320,2297,2275,2252,2229,2205,2180,2155,2131,2109,2091,2068,2019,1926,1769,1529,1184,717,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// Outdated LUTs
////static uint16_t lookup27[1000] = {2589, 2594, 5800, 5811, 5822, 5833, 5844, 5856, 5867, 5878, 5889, 5900, 5911, 5922, 5933, 5944, 5956, 5967, 5978, 5989, 6000, 6025, 6050, 6075, 6100, 6111, 6122, 6133, 6144, 6156, 6167, 6178, 6189, 6200, 6225, 6250, 6275, 6300, 6311, 6322, 6333, 6344, 6356, 6367, 6378, 6389, 6400, 6425, 6450, 6475, 6500, 6520, 6540, 6560, 6580, 6600, 6611, 6622, 6633, 6644, 6656, 6667, 6678, 6689, 6700, 6725, 6750, 6775, 6800, 6825, 6850, 6875, 6900, 6920, 6940, 6960, 6980, 7000, 7025, 7050, 7075, 7100, 7125, 7150, 7175, 7200, 7220, 7240, 7260, 7280, 7300, 7325, 7350, 7375, 7400, 7425, 7450, 7475, 7500, 7520, 7540, 7560, 7580, 7600, 7650, 7700, 7750, 7800, 7825,
//7850, 7875, 7900, 7920, 7940, 7960, 7980, 8000, 8025, 8050, 8074, 8100, 8125, 8150, 8175, 8200, 8240, 8280, 8320, 8360, 8400, 8450, 8500, 8550, 8600, 8650, 8700, 8750, 8800, 8820, 8840, 8860, 8880, 8900, 8950, 9000, 9050, 9100, 9150, 9200, 9250, 9300, 9340, 9380, 9420, 9460, 9500, 9575, 9650, 9725, 9800, 9850, 9900, 9950, 10000, 10060, 10120, 10180, 10240, 10300, 10350, 10400, 10450, 10500, 10575, 10650, 10725, 10800, 10860, 10920, 10980, 11040, 11100, 11200, 11300, 11400, 11500, 11560, 11620, 11680, 11740, 11800, 11900, 12000, 12100, 12200, 12325, 12450, 12575, 12700, 12780, 12860, 12940, 13020, 13100, 13250, 13400, 13550, 13700, 13875, 14050, 14225, 14400, 14520, 14640, 14760, 14880, 15000, 15250, 15500, 15750, 16000, 16225, 16450, 16675, 16900, 16880, 16860, 16840, 16820,
//16800, 16678, 16557, 16435, 16312, 16192, 16070, 15949, 15827, 15705, 15584, 15462, 15340, 15219, 15097, 14975, 14854, 14732, 14610, 14489, 14367, 14246, 14124, 14002, 13881, 13759, 13637, 13516, 13394, 13272, 13151, 13029, 12907, 12786, 12664, 12543, 12421, 12299, 12178, 12056, 11934, 11813, 11691, 11569, 11448, 11326, 11204, 11083, 10961, 10840, 10718, 10596, 10475, 10353, 10231, 10110, 9988, 9866, 9745, 9623, 9501, 9380, 9258, 9137, 9015, 8893, 8772, 8650, 8528, 8407, 8285, 8163, 8042, 7920, 7799, 7677, 7555, 7434,
//7312, 7190, 7069, 6947, 6825, 6704, 6582, 6460, 6339, 6217, 6096, 5974, 5852, 5731, 5609, 5487, 5366, 5244, 5122, 5001, 4879, 4757, 4636, 4514, 4393, 4271, 4149, 4027, 3906, 3784, 3663, 3541, 3419, 3298, 3176, 3054, 2933, 2811, 2690, 2568, 2446, 2325, 2203, 2081, 1960, 1838, 1716, 1595, 1473, 1351, 1230, 1108, 987, 865, 743, 622, 500, 740, 980, 1220, 1460, 1700, 1775, 1850, 1925, 2000, 2011, 2021, 2033, 2044, 2056, 2067, 2078, 2089, 2100, 2125, 2150, 2175, 2200, 2208, 2215, 2223, 2231, 2238, 2246, 2254, 2262, 2269, 2277, 2285, 2292, 2300, 2311, 2322, 2333, 2344, 2356, 2367, 2378, 2389, 2400, 2406, 2412, 2418, 2424, 2429, 2435, 2441, 2447, 2453, 2459, 2465, 2471, 2476, 2482, 2488, 2494, 2500, 2506, 2511, 2517, 2522, 2528, 2533, 2539, 2544, 2550, 2556, 2561, 2567, 2572, 2578, 2583, 2589, 2594, 2600, 2606, 2612, 2618, 2624, 2629, 2635, 2641, 2647, 2653, 2659, 2665, 2671, 2676, 2682, 2688, 2694, 2700, 2705,
//2709, 2714, 2718, 2723, 2727, 2732, 2736, 2741, 2745, 2750, 2755, 2759, 2764, 2768, 2773, 2777, 2782, 2786, 2791, 2795, 2800, 2805, 2810, 2814, 2819, 2824, 2829, 2833, 2838, 2843, 2848, 2852, 2857, 2862, 2867, 2871, 2876, 2881, 2886, 2890, 2895, 2900, 2905, 2909, 2914, 2918, 2923, 2927, 2932, 2936, 2941, 2945, 2950, 2955, 2959, 2964, 2968, 2973, 2977, 2982, 2986, 2991, 2995, 3000, 3004, 3008, 3012, 3015, 3019, 3023, 3027, 3031, 3035, 3038, 3042, 3046, 3050, 3054, 3058, 3062, 3065, 3069, 3073, 3077, 3081, 3085, 3088, 3092, 3096, 3100, 3104, 3108, 3112, 3115, 3119, 3123, 3127, 3131, 3135, 3138, 3142, 3146, 3150, 3154, 3158, 3162, 3165, 3169, 3173, 3177, 3181, 3185, 3188, 3192, 3196, 3200, 3204, 3208, 3212, 3215, 3219, 3223, 3227, 3231, 3235, 3238, 3242, 3246, 3250, 3254, 3258,
//3262, 3265, 3269, 3273, 3277, 3281, 3285, 3288, 3292, 3296, 3300, 3305, 3309, 3314, 3318, 3323, 3327, 3332, 3336, 3341, 3345, 3350, 3355, 3359, 3364, 3368, 3373, 3377, 3382, 3386, 3391, 3395, 3400, 3404, 3408, 3412, 3415, 3419, 3423, 3427, 3431, 3435, 3438, 3442, 3446, 3450, 3454, 3458, 3462, 3465, 3469, 3473, 3477, 3481, 3485, 3488, 3492, 3496, 3500, 3503, 3506, 3510, 3513, 3516, 3519, 3523, 3526, 3529, 3532, 3535, 3539, 3542, 3545, 3548, 3552, 3555, 3558, 3561, 3565, 3568, 3571, 3574, 3577, 3581, 3584, 3587, 3590, 3594, 3597, 3600, 3605, 3609, 3614, 3618, 3623, 3627, 3632, 3636, 3641, 3645, 3650, 3655, 3659, 3664, 3668, 3673, 3677, 3682, 3686, 3691, 3695, 3700, 3705, 3710, 3714, 3719, 3724, 3729, 3733, 3738, 3743, 3748, 3752, 3757, 3762, 3767, 3771, 3776, 3781, 3786, 3790,
//3795, 3800, 3805, 3809, 3814, 3818, 3823, 3827, 3832, 3836, 3841, 3845, 3850, 3855, 3859, 3864, 3868, 3873, 3877, 3882, 3886, 3891, 3895, 3900, 3905, 3909, 3914, 3918, 3923, 3927, 3932, 3936, 3941, 3945, 3950, 3955, 3959, 3964, 3968, 3973, 3977, 3982, 3986, 3991, 3995, 4000, 4005, 4009, 4014, 4018, 4022, 4027, 4032, 4035, 4041, 4045, 4050, 4054, 4059, 4064, 4067, 4073, 4077, 4082, 4086, 4091, 4094, 4100, 4106, 4112, 4118, 4124, 4129, 4135, 4141, 4147, 4153, 4159, 4165, 4171, 4176, 4182, 4188, 4194, 4200, 4206, 4212, 4218, 4224, 4229, 4235, 4241, 4247, 4253, 4259, 4265, 4271, 4276, 4282, 4288, 4294, 4300, 4306, 4311, 4317, 4322, 4328, 4333, 4339, 4344, 4350, 4356, 4361, 4367, 4372, 4378, 4383, 4389, 4394, 4400, 4406, 4412, 4418, 4424, 4429, 4435, 4441, 4447, 4453, 4459, 4465,
//4471, 4476, 4482, 4488, 4494, 4500, 4508, 4515, 4523, 4531, 4538, 4546, 4554, 4562, 4569, 4577, 4585, 4592, 4600, 4608, 4615, 4623, 4631, 4638, 4646, 4654, 4662, 4669, 4677, 4685, 4692, 4700, 4708, 4715, 4723, 4731, 4738, 4746, 4754, 4762, 4769, 4777, 4785, 4792, 4800, 4808, 4815, 4823, 4831, 4838, 4846, 4854, 4862, 4869, 4877, 4885, 4892, 4900, 4908, 4915, 4923, 4931, 4938, 4946, 4954, 4962, 4969, 4977, 4985, 4992, 5000, 5008, 5015, 5023, 5031, 5038, 5046, 5054, 5062, 5069, 5077, 5085, 5092, 5100, 5111, 5122, 5133, 5144, 5156, 5167, 5178, 5189, 5200, 5211, 5222, 5233, 5244, 5256, 5267, 5278, 5289, 5300, 5308, 5315, 5323, 5331, 5338, 5346, 5354, 5362, 5369, 5377, 5385, 5392, 5400, 5412, 5425, 5438, 5450, 5463, 5475, 5488, 5500, 5511, 5522, 5533, 5544, 5556, 5567, 5578, 5589,
//5600, 5611, 5622, 5633, 5644, 5656, 5667, 5678, 5689, 5700, 3119, 3123, 3127, 3131, 3135};

//static uint16_t lookup27_Reversed[1000] = {7075, 7050, 7025, 7000, 6980, 6960, 6940, 6920,
//		6900, 6875, 6850, 6825, 6800, 6775, 6750, 6725, 6700, 6689, 6678, 6667,
//		6656, 6644, 6633, 6622, 6611, 6600, 6580, 6560, 6540, 6520, 6500, 6475,
//		6450, 6425, 6400, 6389, 6378, 6367, 6356, 6344, 6333, 6322, 6311, 6300,
//		6275, 6250, 6225, 6200, 6189, 6178, 6167, 6156, 6144, 6133, 6122, 6111,
//		6100, 6075, 6050, 6025, 6000, 5989, 5978, 5967, 5956, 5944, 5933, 5922,
//		5911, 5900, 5889, 5878, 5867, 5856, 5844, 5833, 5822, 5811, 5800, 5788,
//		5775, 5763, 5750, 5738, 5725, 5713, 5700, 5689, 5678, 5667, 5656, 5644,
//		5633, 5622, 5611, 5600, 5589, 5578, 5567, 5556, 5544, 5533, 5522, 5511,
//		5500, 5487, 5475, 5463, 5450, 5438, 5425, 5413, 5400, 5392, 5385, 5377,
//		5369, 5362, 5354, 5346, 5338, 5331, 5323, 5315, 5308, 5300, 5289, 5278,
//		5267, 5256, 5244, 5233, 5222, 5211, 5200, 5189, 5178, 5167, 5156, 5144,
//		5133, 5122, 5111, 5100, 5092, 5085, 5077, 5069, 5062, 5054, 5046, 5038,
//		5031, 5023, 5015, 5008, 5000, 4992, 4985, 4977, 4969, 4962, 4954, 4946,
//		4938, 4931, 4923, 4915, 4908, 4900, 4892, 4885, 4877, 4869, 4862, 4854,
//		4846, 4838, 4831, 4823, 4815, 4808, 4800, 4792, 4785, 4777, 4769, 4762,
//		4754, 4746, 4738, 4731, 4723, 4715, 4708, 4700, 4692, 4685, 4677, 4669,
//		4662, 4654, 4646, 4638, 4631, 4623, 4615, 4608, 4600, 4592, 4585, 4577,
//		4569, 4562, 4554, 4546, 4538, 4531, 4523, 4515, 4508, 4500, 4494, 4488,
//		4482, 4476, 4471, 4465, 4459, 4453, 4447, 4441, 4435, 4429, 4424, 4418,
//		4412, 4406, 4400, 4394, 4389, 4383, 4378, 4372, 4367, 4361, 4356, 4350,
//		4344, 4339, 4333, 4328, 4322, 4317, 4311, 4306, 4300, 4294, 4288, 4282,
//		4276, 4271, 4265, 4259, 4253, 4247, 4241, 4235, 4229, 4224, 4218, 4212,
//		4206, 4200, 4194, 4189, 4183, 4178, 4172, 4167, 4161, 4156, 4150, 4144,
//		4139, 4133, 4128, 4122, 4117, 4111, 4106, 16800, 16820, 16840, 16860,
//		16880, 16900, 16675, 16450, 16225, 16000, 15750, 15500, 15250, 15000,
//		14880, 14760, 14640, 14520, 14400, 14225, 14050, 13875, 13700, 13550,
//		13400, 13250, 13100, 13020, 12940, 12860, 12780, 12700, 12575, 12450,
//		12325, 12200, 12120, 12040, 11960, 11880, 11800, 11725, 11650, 11575,
//		11500, 11400, 11300, 11200, 11100, 11040, 10980, 10920, 10860, 10800,
//		10725, 10650, 10575, 10500, 10450, 10400, 10350, 10300, 10240, 10180,
//		10120, 10060, 10000, 9950, 9900, 9850, 9800, 9725, 9650, 9575, 9500,
//		9460, 9420, 9380, 9340, 9300, 9250, 9200, 9150, 9100, 9050, 9000, 8950,
//		8900, 8880, 8860, 8840, 8820, 8800, 8750, 8700, 8650, 8600, 8550, 8500,
//		8450, 8400, 8360, 8320, 8280, 8240, 8200, 8175, 8150, 8125, 8100, 8074,
//		8050, 8025, 8000, 7980, 7960, 7940, 7920, 7900, 7875, 7850, 7825, 7800,
//		7750, 7700, 7650, 7600, 7580, 7560, 7540, 7520, 7500, 7475, 7450, 7425,
//		7400, 7375, 7350, 7325, 7300, 7280, 7260, 7240, 7220, 7200, 7175, 7150,
//		7125, 7100, 7075, 7050, 7025, 7000, 6980, 6960, 6940, 6920, 6900, 6875,
//		6850, 6825, 6800, 6775, 6750, 6725, 6700, 6689, 6678, 6667, 6656, 6644,
//		6633, 6622, 6611, 6600, 6580, 6560, 6540, 6520, 6500, 6475, 6450, 6425,
//		6400, 6389, 6378, 6367, 6356, 6344, 6333, 6322, 6311, 6300, 6275, 6250,
//		6225, 6200, 6189, 6178, 6167, 6156, 6144, 6133, 6122, 6111, 6100, 6075,
//		6050, 6025, 6000, 5989, 5978, 5967, 5956, 5944, 5933, 5922, 5911, 5900,
//		5889, 5878, 5867, 5856, 5844, 5833, 5822, 5811, 5800, 5788, 5775, 5763,
//		5750, 5738, 5725, 5713, 5700, 5689, 5678, 5667, 5656, 5644, 5633, 5622,
//		5611, 5600, 5589, 5578, 5567, 5556, 5544, 5533, 5522, 5511, 5500, 5487,
//		5475, 5463, 5450, 5438, 5425, 5413, 5400, 5392, 5385, 5377, 5369, 5362,
//		5354, 5346, 5338, 5331, 5323, 5315, 5308, 5300, 5289, 5278, 5267, 5256,
//		5244, 5233, 5222, 5211, 5200, 5189, 5178, 5167, 5156, 5144, 5133, 5122,
//		5111, 5100, 5092, 5085, 5077, 5069, 5062, 5054, 5046, 5038, 5031, 5023,
//		5015, 5008, 5000, 4992, 4985, 4977, 4969, 4962, 4954, 4946, 4938, 4931,
//		4923, 4915, 4908, 4900, 4892, 4885, 4877, 4869, 4862, 4854, 4846, 4838,
//		4831, 4823, 4815, 4808, 4800, 4792, 4785, 4777, 4769, 4762, 4754, 4746,
//		4738, 4731, 4723, 4715, 4708, 4700, 4692, 4685, 4677, 4669, 4662, 4654,
//		4646, 4638, 4631, 4623, 4615, 4608, 4600, 4592, 4585, 4577, 4569, 4562,
//		4554, 4546, 4538, 4531, 4523, 4515, 4508, 4500, 4494, 4488, 4482, 4476,
//		4471, 4465, 4459, 4453, 4447, 4441, 4435, 4429, 4424, 4418, 4412, 4406,
//		4400, 4394, 4389, 4383, 4378, 4372, 4367, 4361, 4356, 4350, 4344, 4339,
//		4333, 4328, 4322, 4317, 4311, 4306, 4300, 4294, 4288, 4282, 4276, 4271,
//		4265, 4259, 4253, 4247, 4241, 4235, 4229, 4224, 4218, 4212, 4206, 4200,
//		4194, 4189, 4183, 4178, 4172, 4167, 4161, 4156, 4150, 4144, 4139, 4133,
//		4128, 4122, 4117, 4111, 4106, 4100, 4094, 4090, 4086, 4081, 4075, 4070,
//		4067, 4062, 4057, 4051, 4048, 4043, 4038, 4033, 4029, 4024, 4019, 4014,
//		4010, 4005, 4000, 3995, 3991, 3986, 3982, 3977, 3973, 3968, 3964, 3959,
//		3955, 3950, 3945, 3941, 3936, 3932, 3927, 3923, 3918, 3914, 3909, 3905,
//		3900, 3895, 3891, 3886, 3882, 3877, 3873, 3868, 3864, 3859, 3855, 3850,
//		3845, 3841, 3836, 3832, 3827, 3823, 3818, 3814, 3809, 3805, 3800, 3795,
//		3790, 3786, 3781, 3776, 3771, 3767, 3762, 3757, 3752, 3748, 3743, 3738,
//		3733, 3729, 3724, 3719, 3714, 3710, 3705, 3700, 3695, 3691, 3686, 3682,
//		3677, 3673, 3668, 3664, 3659, 3655, 3650, 3645, 3641, 3636, 3632, 3627,
//		3623, 3618, 3614, 3609, 3605, 3600, 3597, 3594, 3590, 3587, 3584, 3581,
//		3577, 3574, 3571, 3568, 3565, 3561, 3558, 3555, 3552, 3548, 3545, 3542,
//		3539, 3535, 3532, 3529, 3526, 3523, 3519, 3516, 3513, 3510, 3506, 3503,
//		3500, 3496, 3492, 3488, 3485, 3481, 3477, 3473, 3469, 3465, 3462, 3458,
//		3454, 3450, 3446, 3442, 3438, 3435, 3431, 3427, 3423, 3419, 3415, 3412,
//		3408, 3404, 3400, 3395, 3391, 3386, 3382, 3377, 3373, 3368, 3364, 3359,
//		3355, 3350, 3345, 3341, 3336, 3332, 3327, 3323, 3318, 3314, 3309, 3305,
//		3300, 3296, 3292, 3288, 3285, 3281, 3277, 3273, 3269, 3265, 3262, 3258,
//		3254, 3250, 3246, 3242, 3238, 3235, 3231, 3227, 3223, 3219, 3215, 3212,
//		3208, 3204, 3200, 3196, 3192, 3188, 3185, 3181, 3177, 3173, 3169, 3165,
//		3162, 3158, 3154, 3150, 3146, 3142, 3138, 3135, 3131, 3127, 3123, 3119,
//		3115, 3112, 3108, 3104, 3100, 3096, 3092, 3088, 3085, 3081, 3077, 3073,
//		3069, 3065, 3062, 3058, 3054, 3050, 3046, 3042, 3038, 3035, 3031, 3027,
//		3023, 3019, 3015, 3012, 3008, 3004, 3000, 5362, 5354, 5346};



//static uint16_t lookup54[1000] = {
//	0, 0, 0, 0, 0, 0, 17800, 17325, 16850, 16375, 15900, 15400, 14900, 14400, 13900, 13475,
//	13050, 12625, 12200, 11900, 11600, 11300, 11000, 10775, 10550, 10325, 10100, 9925, 9750,
//	9575, 9400, 9225, 9050, 8875, 8700, 8600, 8500, 8400, 8300, 8200, 8100, 8000, 7900, 7775,
//	7650, 7525, 7400, 7325, 7250, 7175, 7100, 7025, 6950, 6875, 6800, 6725, 6650, 6575, 6500,
//	6450, 6400, 6350, 6300, 6260, 6220, 6180, 6140, 6100, 6050, 6000, 5950, 5900, 5875, 5850,
//	5825, 5800, 5750, 5700, 5650, 5600, 5575, 5550, 5525, 5500, 5475, 5450, 5425, 5400, 5350,
//	5300, 5250, 5200, 5175, 5150, 5125, 5100, 5075, 5050, 5025, 5000, 4975, 4950, 4925, 4900,
//	4888, 4875, 4862, 4850, 4838, 4825, 4812, 4800, 4775, 4750, 4725, 4700, 4675, 4650, 4625,
//	4600, 4587, 4575, 4562, 4550, 4537, 4525, 4512, 4500, 4475, 4450, 4425, 4400, 4387, 4375,
//	4362, 4350, 4337, 4325, 4312, 4300, 4275, 4250, 4225, 4200, 4188, 4175, 4162, 4150, 4137,
//	4125, 4112, 4100, 4086, 4075, 4062, 4050, 4037, 4025, 4011, 4000, 3987, 3975, 3962, 3950,
//	3938, 3925, 3912, 3900, 3894, 3887, 3881, 3875, 3869, 3862, 3856, 3850, 3844, 3837, 3831,
//	3825, 3819, 3812, 3806, 3800, 3792, 3783, 3775, 3767, 3758, 3750, 3742, 3733, 3725, 3717,
//	3708, 3700, 3688, 3675, 3663, 3650, 3638, 3625, 3613, 3600, 3588, 3575, 3563, 3550, 3538,
//	3525, 3513, 3500, 3492, 3483, 3475, 3467, 3458, 3450, 3442, 3433, 3425, 3417, 3408, 3400,
//	3387, 3375, 3362, 3350, 3337, 3325, 3312, 3300, 3292, 3283, 3275, 3267, 3258, 3250, 3242,
//	3233, 3225, 3217, 3208, 3200, 3192, 3183, 3175, 3167, 3158, 3150, 3142, 3133, 3125, 3117,
//	3108, 3100, 3088, 3075, 3063, 3050, 3038, 3025, 3013, 3000, 2994, 2988, 2982, 2976, 2971,
//	2965, 2959, 2953, 2947, 2941, 2935, 2929, 2924, 2918, 2912, 2906, 2900, 2892, 2883, 2875,
//	2867, 2858, 2850, 2842, 2833, 2825, 2817, 2808, 2800, 2792, 2783, 2775, 2767, 2758, 2750,
//	2742, 2733, 2725, 2717, 2708, 2700, 2692, 2683, 2675, 2667, 2658, 2650, 2642, 2633, 2625,
//	2617, 2608, 2600, 2592, 2583, 2575, 2567, 2558, 2550, 2542, 2533, 2525, 2517, 2508, 2500,
//	2487, 2475, 2462, 2450, 2437, 2425, 2412, 2400, 2392, 2383, 2375, 2367, 2358, 2350, 2342,
//	2333, 2325, 2317, 2308, 2300, 2287, 2275, 2263, 2250, 2237, 2225, 2213, 2200, 2175, 2150,
//	2125, 2100, 2075, 2050, 2025, 2000, 1750, 1500, 1250, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//	};

//static uint16_t lookup108[1000] = {
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0,0, 0, 0, 0, 0, 0, 0, 1600, 1675, 1750, 1825, 1900, 1925, 1950, 1975, 2000, 2050, 2100,
//2150, 2200, 2225, 2250, 2275, 2300, 2325, 2350, 2375, 2400, 2425, 2450, 2475, 2500, 2525, 2550,
//2575, 2600, 2625, 2650, 2675, 2700, 2725, 2750, 2775, 2800, 2850, 2900, 2950, 3000, 3100, 3125,
//3150, 3175, 3200, 3250, 3300, 3350, 3400, 3425, 3450, 3475, 3500, 3525, 3550, 3575, 3600, 3625,
//3650, 3675, 3700, 3750, 3800, 3850, 3900, 3925, 3950, 3975, 4000, 4050, 4100, 4150, 4200, 4250,
//4300, 4350, 4400, 4460, 4520, 4580, 4640, 4700, 4775, 4850, 4925, 5000, 5100, 5200, 5300, 5400,
//5525, 5650, 5775, 5900, 6075, 6250, 6425, 6600, 6900, 7200, 7500, 7800, 8275, 8750, 9225, 9700,
//10650, 11600, 12550, 13500, 14575, 15650, 16725, 17800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//};
//

#endif /* INC_STAGEDEFINITIONS_H_ */
