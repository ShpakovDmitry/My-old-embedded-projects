//
//
//



#ifndef _FFT_DATA_MANIPULATION_
#define _FFT_DATA_MANIPULATION_


unsigned int SignalInit(signal *sig, unsigned int sample_num, double sample_rate);
void SignalFree(signal *sig);
void SignalCopy(signal *B, signal *A);	// A ->copy-> B



















#endif
