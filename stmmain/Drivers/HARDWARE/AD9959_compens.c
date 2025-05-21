
#include "AD9959_compens.H"




/*
对于AD9959的定点数增益数组
步进1MHz/数据，从0开始
浮点数增益 ampF=AD9959_COMP_Q2F( 定点数增益q )
ampF在0-MAX_F之间
*/
const uint16_t AD9959_AMP_MAP[AD9959_COMP_MAP_SIZE]={
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384,
	16384,16384,16384,16384,16384
};

/*
三次插值函数
arr_begin：真实值数组起始地址
arr_len：    数组长度
intp_idx：   插值目标点
在接近数组起始或结束的位置不准确
*/
float Interpolate_Q15(uint16_t* arr_begin, uint16_t arr_len, float intp_idx)
{
	if(intp_idx<0) return AD9959_COMP_Q2F(arr_begin[0]);
	if(intp_idx>=arr_len) return AD9959_COMP_Q2F(arr_begin[arr_len-1]);
	
	uint16_t i1 = (uint16_t)(intp_idx);
	if (i1 < 1) {
		i1 = 1;
	} else if (i1 > arr_len - 3) {
		i1 = arr_len - 3;
	}
	
	float v0 = AD9959_COMP_Q2F(arr_begin[i1 - 1]);
	float v1 = AD9959_COMP_Q2F(arr_begin[i1]);
	float v2 = AD9959_COMP_Q2F(arr_begin[i1 + 1]);
	float v3 = AD9959_COMP_Q2F(arr_begin[i1+2]);
	float d = intp_idx - i1;
	// 计算三次插值结果
	return v1 + 0.5f * d * (v2 - v0 + d * (2.0f * v0 - 5.0f * v1 + 4.0f * v2 - v3 + d * (3.0f * (v1 - v2) + v3 - v0)));
}

float AD9959_GetMinAmp(uint16_t rng1, uint16_t rng2)
{
	float result=AD9959_COMP_MAX_F;
	for(uint16_t i=rng1; i<AD9959_COMP_MAP_SIZE && i<rng2; i++)
	{
		if(AD9959_COMP_Q2F(AD9959_AMP_MAP[i])<result) result=AD9959_COMP_Q2F(AD9959_AMP_MAP[i]);
	}
	return result;
}

float AD9959_AmpComps(float freq)
{
	float idx=freq/1000000;
	float amp_act=Interpolate_Q15((uint16_t*)AD9959_AMP_MAP,AD9959_COMP_MAP_SIZE,idx);
	return 1/amp_act;
}


