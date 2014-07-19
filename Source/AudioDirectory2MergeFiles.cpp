/*
  ==============================================================================

    AudioXml2MergeFiles.cpp
    Created: 19 Jul 2014 3:44:02am
    Author:  mengke

  ==============================================================================
*/

#include "create_simple_audio_writer.h"
#include "AudioDirectory2MergeFiles.h"
#include <iostream>
#include <tuple>
using namespace std;

class AudioFileContainer
{
public:
	AudioFileContainer(){}
	~AudioFileContainer(){}
public:
	File m_src_file_path;
	String m_file_name;
	String m_target_big_file_name;
	int64 m_start_in_target_file;
	int64 m_length;
	ScopedPointer<AudioFormatReader> m_reader;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileContainer);
};



tuple<File,File> CheckSourcePathAndCreateTargetPath(const String& sourcePath,const String& targetPath)
{
	File sourcePathFile(sourcePath);

	if(!sourcePathFile.exists()) {
		cout << " can not find source path~" << endl;
		throw 3;
	}

	if(!sourcePathFile.isDirectory()) {
		cout << "source path is not a directory!" << endl;
		throw 3;
	}

	File targetPathFile(targetPath);
	File bigFilePath = File(targetPath).getChildFile(sourcePathFile.getFileName() + ".merge");

	if(bigFilePath.exists()) {
		if(!bigFilePath.existsAsFile()) {
			cout << "target path exists!" << endl;
			throw 3;
		}
	}

	if(!bigFilePath.createDirectory()) {
		cout << "can't create target file path" << endl;
		throw 3;
	}

	return tuple<File,File>(sourcePathFile,bigFilePath);
}

void SreachSourcePathAudioFileFetchArray(const File& sourcePath,OwnedArray<AudioFileContainer>& array)
{
	jassert(array.size() == 0);
	Array<File> childPaths;
	int allFileNum = sourcePath.findChildFiles(childPaths,File::findFiles,true,"*");

	if(allFileNum <= 0) {
		cout << "source path is empty!" << endl;
		throw 3;
	}
	bool notAudioFile = false;
	for(int i = 0; i < childPaths.size(); ++i) {
		if(!( childPaths[i].hasFileExtension("wav") || childPaths[i].hasFileExtension("WAV") )) {
			cout << childPaths[i].getRelativePathFrom(sourcePath) << " is not a audio file" << endl;
			notAudioFile = true;
		}
		if(childPaths[i].isHidden()) {
			cout << childPaths[i].getRelativePathFrom(sourcePath) << " is a hidden file" << endl;
			notAudioFile = true;
		}
	}
	if(notAudioFile == true) {
		cout << "source path can only contain wav audio files only..." << endl;
		throw 3;
	}

	int i = 0;
	for(; i < childPaths.size(); ++i) {
		AudioFileContainer* a = new AudioFileContainer();
		a->m_file_name = childPaths[i].getRelativePathFrom(sourcePath);
		a->m_src_file_path = childPaths[i].getFullPathName();
		a->m_length = 0;
		a->m_start_in_target_file = 0;
		a->m_target_big_file_name = String::empty;
		//cout << "linking:" << childPaths[i].getRelativePathFrom(sourcePath) << endl;
		array.add(a);
	}
	cout << "all wav file number:" << i << endl;
}

tuple<int,double> AudioContaner2ReaderGroup(OwnedArray<AudioFileContainer>& readersToProcess)
{
	ScopedPointer<WavAudioFormat> wavFormat = new WavAudioFormat();

	for(int i = 0; i < readersToProcess.size();++i) {
		ScopedPointer<AudioFormatReader> inputStream = wavFormat->createReaderFor(readersToProcess[i]->m_src_file_path.createInputStream(),true);
		if(inputStream == nullptr) {
			cout << "Audio file " << readersToProcess[i]->m_file_name << "read error" << endl;
			throw 3;
		}
		readersToProcess[i]->m_reader = inputStream.release();
	}

	double sampleRate = readersToProcess[0]->m_reader->sampleRate;
	int all_file_ch_num = 1;
	for(int i = 0; i < readersToProcess.size();++i) {
		if(sampleRate != readersToProcess[i]->m_reader->sampleRate ) {
			wcout << "file " << readersToProcess[0]->m_file_name << "has diff sample rate" << endl;
			throw 3;
		}
		if(readersToProcess[0]->m_reader->numChannels <= 0) {
			wcout << "File read error, read wav files to the channel number less than or equal to 0" << endl;
			throw 3;
		} else if(readersToProcess[0]->m_reader->numChannels > 2) {
			wcout << "Detect the channel number greater than 2 , can't handle more than two channels." << endl;
			throw 3;
		} else if(readersToProcess[0]->m_reader->numChannels > 1) {
			all_file_ch_num = 2;
			jassert(readersToProcess[0]->m_reader->numChannels == 2);
		}

		if(readersToProcess[0]->m_reader->lengthInSamples <= 0) {
			wcout << "Detect file size less than or equal to zero. don't know what to do" << endl;
			throw 3;
		}
	}

	return tuple<int,double>(all_file_ch_num,sampleRate);
}

void SaveSomeFileToMergeFile( AudioSampleBuffer& muttingBuffer,
							  int channelNum,
							  double sampleRate,
							  OwnedArray<AudioFileContainer>& readers,
							  XmlElement* masterXml,
							  const File& sourcePath,
							  const File& targetPath
							)
{
	File bigFile;
	for(int i = 0; ( bigFile = File(targetPath.getChildFile(sourcePath.getFileName() + "_merge" + String(i) + ".wav")) ).exists() ; ++i);

	if(bigFile.create() != Result::ok()) { cout << "音频文件建立失败" << endl; throw 3; }

	ScopedPointer<OutputStream> ptr = bigFile.createOutputStream();
	if(ptr == nullptr) { cout << "音频文件写入器建立失败" << endl; throw 3; }

	ScopedPointer<AudioFormatWriter> writer = create_simple_audio_writer(ptr.release(),sampleRate,channelNum,0);
	if(writer == nullptr) { cout << "音频文件写入器建立失败" << endl; throw 3; }

	ScopedPointer<XmlElement> thisBigFileXmlElement = new XmlElement("merge_file");
	thisBigFileXmlElement->setAttribute("name",bigFile.getFileName());

	int64 big_file_size_counter = 0;

	while(readers.size() > 0) {
		if(!writer->writeFromAudioSampleBuffer(muttingBuffer,0,muttingBuffer.getNumSamples())) {
			wcout << "音频文件写入错误,退出了啊" << endl;
		}
		big_file_size_counter += muttingBuffer.getNumSamples();

		ScopedPointer<AudioFileContainer> file = readers.removeAndReturn(0);
		ScopedPointer<XmlElement> small_file_xml = new XmlElement("src_file");

		AudioFormatReader*const small_file_reader = file->m_reader;

		small_file_xml->setAttribute("name",file->m_file_name);
		small_file_xml->setAttribute("startInBigFile",String(big_file_size_counter));
		small_file_xml->setAttribute("length",String(small_file_reader->lengthInSamples));
		small_file_xml->setAttribute("channelNum",(int)small_file_reader->numChannels);
		small_file_xml->setAttribute("bitDepth",(int)small_file_reader->bitsPerSample);

		if(!writer->writeFromAudioReader(*small_file_reader,0,-1)) {
			wcout << "音频文件写入错误,退出了啊" << endl;
		}
		big_file_size_counter += small_file_reader->lengthInSamples;

		if(big_file_size_counter * 4 * channelNum >= 1000 * 1024 * 1024) {
			break;
		}

		thisBigFileXmlElement->addChildElement(small_file_xml.release());

		cout << ".";

	}
	if(!writer->writeFromAudioSampleBuffer(muttingBuffer,0,muttingBuffer.getNumSamples())) {
		wcout << "音频文件写入错误,退出了啊" << endl;
	}
	masterXml->addChildElement(thisBigFileXmlElement.release());
};


void AudioDir2MergeFiles(const String& sourcePath,int64 muttingTimeEachFile,const String& targetPath)
{
	tuple<File,File> paths = CheckSourcePathAndCreateTargetPath(sourcePath,targetPath);

	OwnedArray<AudioFileContainer> readersToProcess;
	ScopedPointer<XmlElement> masterXml = new XmlElement("AudioFileLineUp");
	
	SreachSourcePathAudioFileFetchArray(get<0>(paths),readersToProcess);
	
	tuple<int,double> audioMessage = AudioContaner2ReaderGroup(readersToProcess);

	AudioSampleBuffer empty_buffer(1,(int)( get<1>(audioMessage)*muttingTimeEachFile / 1000.0 ));
	empty_buffer.clear();

	while(readersToProcess.size() > 0) {
		SaveSomeFileToMergeFile(empty_buffer,get<0>(audioMessage),get<1>(audioMessage),readersToProcess,masterXml,get<0>(paths),get<1>(paths));
	}

	File bigFileXml = File(get<1>(paths)).getChildFile(get<1>(paths).getFileName() + ".xml");
	 
 	if(bigFileXml.exists()) {
 		wcout << "target xml exists!" << endl;
	 	throw 3;
 	}

	if(!bigFileXml.create()) {
		cout << "target xml create wrong!" << endl;
		throw 3;
	}
	if(!masterXml->writeToFile(bigFileXml,String::empty)) {
		cout << "target xml create wrong!" << endl;
		throw 3;
	}
}



