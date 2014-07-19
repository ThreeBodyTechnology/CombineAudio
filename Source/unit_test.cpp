/*
  ==============================================================================

  This file is part of the CombineAudio tool.

  Permission is granted to use this software under the terms of either:
  a) the GPL v2 (or any later version)

  Details of these licenses can be found at: www.gnu.org/licenses

  ==============================================================================
*/

#include "JuceHeader.h"
#include "create_simple_audio_writer.h"

void test(const File& input,const File& output)
{
	output.create();
	ScopedPointer<WavAudioFormat> wavFormat = new WavAudioFormat();
	ScopedPointer<AudioFormatReader> inputStream = wavFormat->createReaderFor(input.createInputStream(),true);
	OutputStream* writer_stream = output.createOutputStream();
	ScopedPointer<AudioFormatWriter> writer = create_simple_audio_writer(writer_stream,inputStream->sampleRate,inputStream->numChannels,0,inputStream->bitsPerSample);
	writer->writeFromAudioReader(*inputStream,0,-1);
}

int main2()
{
	test(File("c://x.wav"),File("c://y.wav"));
	return 1;
}