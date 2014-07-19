/*
  ==============================================================================

  This file is part of the CombineAudio tool.

  Permission is granted to use this software under the terms of either:
  a) the GPL v2 (or any later version)

  Details of these licenses can be found at: www.gnu.org/licenses

  ==============================================================================
*/
  
#ifndef create_simple_audio_writer_h__
#define create_simple_audio_writer_h__

#include "JuceHeader.h"

AudioFormatWriter* create_simple_audio_writer(OutputStream* stream,double sampleRate,int channelNum,int64 bigLength,int bitDepth = 32);

#endif // create_simple_audio_writer_h__
