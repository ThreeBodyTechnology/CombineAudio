#ifndef create_simple_audio_writer_h__
#define create_simple_audio_writer_h__

#include "JuceHeader.h"

AudioFormatWriter* create_simple_audio_writer(OutputStream* stream,double sampleRate,int channelNum,int64 bigLength,int bitDepth = 32);

#endif // create_simple_audio_writer_h__
