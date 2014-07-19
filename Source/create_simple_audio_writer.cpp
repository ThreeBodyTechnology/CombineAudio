
#include "create_simple_audio_writer.h"
#include <iostream>

using namespace std;

AudioFormatWriter* create_simple_audio_writer(OutputStream* stream,double sampleRate,int channelNum,int64 bigLength,int bitDepth)
{
	WavAudioFormat manager;

	StringPairArray mata = WavAudioFormat::createBWAVMetadata(String::empty,
															  String::empty,
															  String::empty,
															  Time::getCurrentTime(),
															  bigLength,
															  String::empty);

	ScopedPointer<AudioFormatWriter> writer = manager.createWriterFor(stream,sampleRate,channelNum,bitDepth,mata,0);

	if(writer == nullptr) { wcout << "ÒôÆµÐ´ÈëÆ÷½¨Á¢Ê§°Ü" << endl; throw 3; }
	return writer.release();
}

