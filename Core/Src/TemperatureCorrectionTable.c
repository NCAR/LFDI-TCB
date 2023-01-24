/*
 * This Document Consists of Two Tables
 * 	One To convert the Users request for nm Tuning to a voltage at 25C
 * 		ie if I wanted 656.28 nm that would be in tune at 3V at 25C
 * 	the Second is to get the Offset of the needed Voltage Depending on the current temperature
 *
 */
#include "TemperatureCorrectionTable.h"

//This Will translate a nm Tuning request to a position at 25C
//The Range Should Cover 600nm to 700nm in 0.01 nm intervals or 10000 entries
uint16_t NM_to_Position(float * nm)
{
	uint16_t index = (uint16_t)*nm*100;
//Index starts at 600 and moves to 700 in .01 intervals
//Convert the User input by subtracting 600 and muliplying by 10000
	uint16_t adjusted_input = (*nm - 600)*10000;
	static const uint16_t lookup[415];
	uint16_t pos = lookup[index];
	return pos;
}


//Lookup Table to convert a pixel position to a voltage at the base Temperature. 25C
float BaseT_Position_to_BaseT_Voltage(uint16_t position){
	if(position > 982){
		return 0.0;
	}
	//This look up will convert a pixel position to a voltage at 25C
	//The Index is the pixel position and the value is the voltage in mV
	static const uint16_t lookup[983] =
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12562,12469,12816,12037,11863,11140,11019,10897,10763,10607,10496,10406,10344,
	10298,10234,10186,10116,10062,9986,9922,9864,9795,9700,9600,9514,9453,9400,9349,9302,9259,9208,9172,9116,9079,9021,
	8979,8913,8857,8800,8746,8700,8670,8627,8600,8577,8541,8515,8500,8478,8446,8424,8402,8391,8370,8339,8318,8300,8284,
	8253,8228,8204,8188,8159,8118,8092,8041,8000,7936,7891,7830,7796,7754,7711,7687,7648,7612,7591,7552,7522,7500,7475,
	7437,7409,7391,7356,7325,7300,7277,7237,7206,7183,7139,7104,7078,7033,7000,6975,6934,6904,6886,6850,6825,6800,6786,
	6762,6728,6705,6691,6667,6634,6610,6596,6571,6537,6513,6498,6474,6441,6418,6400,6383,6351,6329,6307,6296,6274,6243,
	6222,6201,6190,6170,6139,6118,6100,6087,6063,6036,6017,6000,5989,5971,5947,5926,5910,5900,5888,5872,5847,5831,5816,
	5801,5796,5781,5766,5742,5726,5711,5700,5691,5677,5662,5637,5623,5609,5600,5592,5579,5562,5544,5532,5521,5510,5500,
	5499,5489,5479,5470,5460,5442,5432,5423,5415,5406,5400,5399,5391,5382,5374,5362,5348,5340,5332,5324,5316,5308,5300,
	5300,5294,5285,5277,5268,5252,5243,5234,5226,5217,5209,5200,5200,5192,5183,5174,5165,5151,5136,5127,5117,5108,5100,
	5099,5089,5080,5071,5052,5042,5033,5023,5014,5005,5000,4996,4987,4978,4968,4957,4939,4929,4920,4910,4900,4900,4889,
	4879,4866,4847,4836,4825,4813,4801,4798,4785,4772,4757,4734,4720,4706,4700,4688,4675,4662,4640,4628,4617,4606,4600,
	4596,4587,4578,4569,4553,4543,4536,4528,4521,4513,4506,4500,4500,4496,4490,4483,4477,4471,4465,4457,4446,4437,4431,
	4426,4420,4414,4409,4403,4400,4400,4397,4392,4387,4381,4376,4371,4362,4352,4346,4341,4336,4331,4326,4321,4316,4311,
	4306,4301,4300,4300,4296,4291,4287,4282,4277,4272,4265,4255,4248,4243,4238,4234,4229,4224,4220,4215,4210,4206,4201,
	4200,4200,4197,4193,4188,4184,4179,4175,4170,4166,4161,4154,4145,4138,4133,4129,4125,4120,4116,4111,4107,4103,4100,
	4100,4100,4095,4091,4087,4082,4078,4073,4068,4059,4051,4046,4042,4037,4033,4029,4024,4020,4015,4011,4006,4002,4000,
	4000,3999,3994,3990,3985,3981,3976,3972,3967,3962,3956,3946,3939,3934,3929,3924,3920,3915,3910,3905,3901,3900,3900,
	3896,3891,3886,3881,3876,3871,3866,3861,3851,3841,3835,3830,3825,3820,3814,3809,3803,3800,3800,3797,3792,3786,3781,
	3776,3770,3765,3758,3747,3738,3733,3727,3722,3716,3711,3705,3700,3700,3699,3694,3688,3683,3678,3672,3664,3653,3646,
	3641,3636,3631,3625,3620,3615,3610,3605,3600,3600,3600,3595,3590,3585,3580,3575,3570,3561,3551,3546,3541,3536,3531,
	3527,3522,3517,3513,3508,3504,3500,3500,3500,3495,3491,3486,3482,3477,3473,3468,3464,3459,3450,3442,3436,3432,3428,
	3423,3419,3415,3411,3406,3402,3400,3400,3399,3395,3391,3387,3383,3379,3374,3370,3366,3362,3356,3348,3340,3336,3332,
	3328,3324,3320,3316,3312,3308,3304,3300,3300,3300,3298,3294,3290,3286,3282,3278,3274,3271,3263,3255,3249,3245,3241,
	3237,3233,3230,3226,3222,3218,3214,3210,3207,3203,3200,3200,3200,3198,3194,3190,3186,3182,3179,3175,3171,3165,3157,
	3150,3146,3142,3139,3135,3131,3128,3124,3120,3116,3113,3109,3105,3102,3100,3100,3100,3097,3093,3090,3086,3082,3079,
	3075,3072,3066,3059,3051,3047,3043,3040,3036,3033,3029,3025,3022,3018,3015,3011,3007,3004,3000,3000,3000,3000,2996,
	2992,2989,2985,2982,2978,2975,2971,2968,2964,2960,2954,2947,2940,2936,2933,2929,2926,2922,2919,2915,2912,2908,2904,
	2901,2900,2900,2900,2897,2893,2890,2886,2883,2879,2876,2872,2869,2865,2861,2856,2849,2841,2837,2834,2830,2826,2823,
	2819,2816,2812,2808,2805,2801,2800,2800,2800,2796,2793,2789,2785,2782,2778,2774,2771,2767,2763,2759,2751,2744,2738,
	2734,2731,2727,2723,2719,2716,2712,2708,2704,2700,2700,2700,2699,2695,2691,2687,2684,2680,2676,2672,2667,2659,2651,
	2647,2643,2639,2635,2631,2627,2624,2620,2616,2612,2608,2604,2600,2600,2600,2599,2595,2591,2587,2583,2579,2575,2572,
	2565,2557,2550,2546,2542,2538,2534,2530,2527,2523,2519,2515,2511,2507,2503,2500,2500,2500,2498,2494,2490,2486,2482,
	2478,2474,2470,2466,2462,2457,2449,2441,2437,2433,2429,2425,2421,2417,2413,2409};
	float volts = lookup[position]/1000.0;
	return volts;
}

//This Function Will Convert the temperature to a positional offset
//This is based on the Found relation ship between Position and Temperature
uint16_t temperature_position_offset(double* temp){

	uint16_t offset =(uint16_t)(46.09*(*temp)-978.1);
	return offset;
}


//This Function Will Convert a Wavelength and a temperature to a Voltage
float Wavelength_to_Voltage(float* wavelength, double* temp){
	//Skip this For now we need a translation table
	//uint16_t base_position = NM_to_Position(wavelength);
	double base_temp = 25;
	uint16_t base_position = 100;
	base_position= base_position + 415;
	uint16_t absolute_BaseT_Offset = temperature_position_offset(&base_temp);
	uint16_t absolute_Offset = temperature_position_offset(temp);
	uint16_t position = base_position + (absolute_BaseT_Offset - absolute_Offset);
	float Voltage = BaseT_Position_to_BaseT_Voltage(position);
	return Voltage;
}
