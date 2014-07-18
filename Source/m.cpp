/*
==============================================================================

This file was auto-generated!

It contains the basic startup code for a Juce application.

==============================================================================
*/
#include <iostream>
#include <tuple>
#include <cmath>
#include <cwchar>
#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;
using namespace std;

typedef tuple<String,ScopedPointer<AudioFormatReader>> AudioFile;
typedef OwnedArray<AudioFile> AudioFileArray;


AudioFormatWriter* create_simple_audio_writer(OutputStream* stream,double sampleRate,int channelNum,int64 bigLength)
{
	WavAudioFormat manager;

	StringPairArray mata = WavAudioFormat::createBWAVMetadata(String::empty,
															  String::empty,
															  String::empty,
															  Time::getCurrentTime(),
															  bigLength,
															  String::empty);

	ScopedPointer<AudioFormatWriter> writer = manager.createWriterFor(stream,sampleRate,channelNum,32,mata,0);

	if(writer == nullptr) { wcout << "��Ƶд��������ʧ��" << endl; throw 3; }
	return writer.release();
};

tuple<int,int64> find_all_audio_file_in_rot(const String& rot,AudioFileArray& filesToRead)
{
	File file(rot);

	if(( !file.exists() ) || file.existsAsFile()) {
		wcout << "�ļ�·��������" << endl;
		throw 3;
	}

	if(!file.isDirectory()) {
		wcout << "�ļ�·��������" << endl;
		throw 3;
	}

	Array<File> files;
	int number = file.findChildFiles(files,File::findFiles | File::ignoreHiddenFiles,true,"*.wav");

	if(number <= 0) {
		wcout << "�ҵ�����Ƶ�ļ���Ϊ0,���˳��˰�" << endl;
		throw 3;
	}

	wcout << "���ҵ���Ƶ�ļ�:" << files.size() << "��,����Ϊ:" << endl;
	for(int i = 0; i < files.size(); ++i) {
		wcout << files[i].getFileName() << endl;
	}

	ScopedPointer<WavAudioFormat> reader = new WavAudioFormat();
	OwnedArray<AudioFormatReader> readers;

	for(int i = 0; i < files.size(); ++i) {
		auto* k = reader->createReaderFor(files[i].createInputStream(),true);

		if(k == nullptr) {
			wcout << "�ļ�" << files[i].getFileNameWithoutExtension().toUTF16() << "��ȡ����" << endl;
			throw 3;
		}

		readers.add(k);
	}

	jassert(readers.size() == files.size());

	double sampleRate = readers[0]->sampleRate;
	int all_file_ch_num = 1;
	int64 all_file_length = 0;
	for(int i = 0; i < readers.size(); ++i) {
		if(sampleRate != readers[i]->sampleRate) {
			wcout << "�ļ�" << files[i].getFileNameWithoutExtension().toUTF16()
				<< "......�Ĳ�������������ͬ,����ʱ���ܴ�������ʲ���ͬ��һ���ļ�,�˳��˰�" << endl;
			throw 3;
		}
		if(readers[i]->numChannels <= 0) {
			wcout << "�ļ���ȡ����,��ȡ����ͨ����С�ڵ���0��wav�ļ�,�˳��˰�" << endl;
			throw 3;
		} else if(readers[i]->numChannels > 2) {
			wcout << "��⵽ͨ��������2����Ƶ�ļ�,������ʱ���ܴ�������ͨ���ĵ��ļ�,�˳��˰�" << endl;
			throw 3;
		} else if(readers[i]->numChannels > 1) {
			all_file_ch_num = 2;
			jassert(readers[i]->numChannels == 2);
		}

		if(readers[i]->lengthInSamples <= 0) {
			wcout << "��⵽�ߴ�С�ڵ���0���ļ�,�Ҳ�֪������ô����,�Ƴ��˰�" << endl;
			throw 3;
		} else {
			all_file_length += readers[i]->lengthInSamples;
		}
	}

	for(int i = 0; readers.size()>0; ++i) {
		filesToRead.add(new AudioFile(files[i].getFileNameWithoutExtension(),readers.removeAndReturn(0)));
	}

	return tuple<int,int64>(all_file_ch_num,all_file_length);
};

void save_some_file_to_big_file(int channelNum,double sampleRate,AudioFileArray& array,const String& rotPath,XmlElement* master_xml)
{
	AudioSampleBuffer empty_buffer(1,(int)(sampleRate*0.25));
	empty_buffer.clear();
	String bigFileRot(rotPath);

	jassert(File(bigFileRot).exists());
	jassert(File(bigFileRot).isDirectory());

	File bigFile;
	for(int i = 0; ( bigFile = File(bigFileRot + "lineup_sub_file" + String(i) + ".wav") ).exists() ; ++i);

	File bigFileXml = File(bigFile.getFullPathName() + ".xml");

	if(bigFileXml.exists()) {
		wcout << "�������µ�bigFile����xmlȴ�Ѿ�������,�������ش���,������,�˳��˰�" << endl;
		throw 3;
	}

	if(bigFile.create() != Result::ok()) { cout << "��Ƶ�ļ�����ʧ��" << endl; throw 3; }
	if(bigFileXml.create() != Result::ok()) { cout << "xml�ļ�����ʧ��" << endl; throw 3; }

	ScopedPointer<OutputStream> ptr = bigFile.createOutputStream();
	if(ptr == nullptr) { cout << "��Ƶ�ļ�д��������ʧ��" << endl; throw 3; }

	ScopedPointer<AudioFormatWriter> writer = create_simple_audio_writer(ptr.release(),sampleRate,channelNum,0);
	if(writer == nullptr) { cout << "��Ƶ�ļ�д��������ʧ��" << endl; throw 3; }

	ScopedPointer<XmlElement> xml = new XmlElement(bigFileXml.getFileName());

	int64 big_file_size_counter = 0;
	int processing_counter = 0;

	while( array.size()>0 ) {
		if(!writer->writeFromAudioSampleBuffer(empty_buffer,0,-1)) {
			wcout << "��Ƶ�ļ�д�����,�˳��˰�" << endl;
		}
		big_file_size_counter += empty_buffer.getNumSamples();

		ScopedPointer<AudioFile> file = array.removeAndReturn(0);
		const String& small_file_name = get<0>(*file);
		AudioFormatReader& small_file_reader = *get<1>(*file);

		XmlElement* a = new XmlElement("sub_file");

		a->setAttribute("name",small_file_name);
		a->setAttribute("startInBigFile",String(big_file_size_counter));
		a->setAttribute("length",String(small_file_reader.lengthInSamples));

		xml->addChildElement(a);

		if(!writer->writeFromAudioReader(small_file_reader,0,-1)) {
			wcout << "��Ƶ�ļ�д�����,�˳��˰�" << endl;
		}
		big_file_size_counter += small_file_reader.lengthInSamples;

		//cout << big_file_size_counter * 4 * channelNum / 1024 / 1024<<endl;
		if(big_file_size_counter * 4 * channelNum / 1024 / 1024 >= 1000) {
		//	break;
		}

		if(processing_counter++ <= 5){
			wcout << ".";
		} else {
			wcout << "\b";
			processing_counter = 0;
		}

	}


};



void create_xml_from_AudioFileArray(const String& rotPath,const String& targetPath)
{
	AudioFileArray allFileArray;
	tuple<int,int64> result = find_all_audio_file_in_rot(rotPath,allFileArray);
	if(allFileArray.size() == 0) {
		wcout << "Ŀ¼��û����,�˳��˰�" << endl;
		throw 3;
	}
	File targetFile(targetPath);
	if(targetFile.exists() && targetFile.isDirectory()) {
		wcout << "�ļ����Ѿ����ڣ��˳��˰�" << endl;
		throw 3;
	}
	if(targetFile.createDirectory()!=Result::ok()) {
		wcout << "�ļ��д���ʧ�ܣ��˳��˰�" << endl;
		throw 3;
	}
	File targetXml(targetFile.getFullPathName() + "//merge.xml");
	if(targetXml.create() != Result::ok()) {
		wcout << "xml�ļ�����ʧ�ܣ��˳��˰�" << endl;
		throw 3;
	}

	ScopedPointer<XmlElement> master_xml = new XmlElement(File(rotPath).getFileNameWithoutExtension());

	wcout << "processing";
	while(allFileArray.size() > 0) {
		save_some_file_to_big_file(get<0>(result),get<1>(*allFileArray[0])->sampleRate,allFileArray,targetPath,master_xml);
	}
	if(!master_xml->writeToFile(bigFileXml,"")) {
		cout << "xml�ļ�д��ʧ��" << endl;
		delete writer.release();
		while(!bigFile.deleteFile());
		throw 3;
	}
};

//==============================================================================
int main()
{
	wcout << "������!,����AudioFileLineUp,�һ������ѡ���Ŀ¼�Զ�ʶ����������뻹����ϲ���Ƶ�ļ�" << endl;
	for(;;) {
		wcout << "����������Ŀ¼:" << endl;
		wchar_t a[6000];
		wcin >> a;
		wcout << "���������Ŀ¼:" << endl;
		wchar_t b[6000];
		wcin >> b;
		if(wcscmp(a,b)==0) {
			wcout << "��,��Ŀ¼��һ����,����������!" << endl;
		} else {
			try {
				create_xml_from_AudioFileArray(a,b);
			} catch(...) {
				wcout << "!!!!!����ʧ��!!!!!!" << endl;
				break;
			}
			wcout << "!!!!����ɹ�!!!!" << endl;
			break;
		}
	}
	system("PAUSE");
	return 0;
}
