/*
  ==============================================================================

  This file is part of the CombineAudio tool.

  Permission is granted to use this software under the terms of either:
  a) the GPL v2 (or any later version)

  Details of these licenses can be found at: www.gnu.org/licenses

  ==============================================================================
*/

#include "create_simple_audio_writer.h"
#include "MergeFiles2AudioDirectory.h"
#include <iostream>
#include <tuple>
using namespace std;

tuple<File,File,XmlElement*> CheckSourcePathAndCreateTargetPath2(const String& sourcePath,const String& targetPath)
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

	if(!sourcePath.contains(".merge")) {
		cout << "source path is not created from AudioFileLineUp!" << endl;
		throw 3;
	}

	File xmlFile=sourcePathFile.getChildFile(sourcePathFile.getFileName() + ".xml");
	if(!xmlFile.existsAsFile()) {
		cout << "xml file missing!" << endl;
		throw 3;
	}
	ScopedPointer<XmlElement> masterXml = XmlDocument::parse(xmlFile);
	if(masterXml==nullptr) {
		cout << "bad xml file!" << endl;
		throw 3;
	}
	if(!masterXml->hasTagName("AudioFileLineUp")) {
		cout << "bad xml file!" << endl;
		throw 3;
	}

	File targetPathFile(targetPath);

	File bigFilePath = File(targetPath).getChildFile(sourcePathFile.getFileName() + ".separate");

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

	return tuple<File,File,XmlElement*>(sourcePathFile,bigFilePath,masterXml.release());
}


void MerFiles2AudioDirectory(const String& srcPath,const String& targetPath)
{
	ScopedPointer<WavAudioFormat> wavFormat = new WavAudioFormat();
	auto value = CheckSourcePathAndCreateTargetPath2(srcPath,targetPath);

	ScopedPointer<XmlElement> masterXml = get<2>(value);
	File srcPathFile = get<0>(value);
	File targetPathFile = get<1>(value);

	if(masterXml->hasTagName("AudioFileLineUp")) {
		forEachXmlChildElement(*masterXml,e) {
			if(e->hasTagName("merge_file")) {
				String name = e->getStringAttribute("name");

				File srcFile = srcPathFile.getChildFile(name);
				if(!srcFile.exists()){}
				ScopedPointer<AudioFormatReader> inputStream = wavFormat->createReaderFor(srcFile.createInputStream(),true);
				if(inputStream == nullptr) {
						cout << "Audio file read error" << endl;
						throw 3;
				}

				double sampleRate = inputStream->sampleRate;
				
				forEachXmlChildElement(*e,e2) {
					if(e2->hasTagName("src_file")) {
						if( !( e2->hasAttribute("name") && e2->hasAttribute("startInBigFile") && e2->hasAttribute("length") && e2->hasAttribute("channelNum") && e2->hasAttribute("bitDepth") ) ) {
							cout << "bad xml file!" << endl;
							throw 3;
						}

						String name = e2->getStringAttribute("name");
						int64 startInBigFile = e2->getStringAttribute("startInBigFile").getLargeIntValue();
						int64 length = e2->getStringAttribute("length").getLargeIntValue();
						int channelNum = e2->getIntAttribute("channelNum",-1);
						int bitDepth = e2->getIntAttribute("bitDepth",-1);

						if(channelNum > 0 && bitDepth > 0) {

						}

						File smallFile = targetPathFile.getChildFile(name);
						if(smallFile.exists()){
							cout << "small audio file exists!" << endl;
							throw 3;
						}
						if(!smallFile.create()) {
							cout << "small audio file create wrong!" << endl;
							throw 3;
						}
						OutputStream* writer_stream = smallFile.createOutputStream();
						if(writer_stream == nullptr){
							cout << "audio writer wrong!" << endl;
							throw 3;
						}

						ScopedPointer<AudioFormatWriter> writer = create_simple_audio_writer(writer_stream,sampleRate,channelNum,0,bitDepth);

						if(!writer->writeFromAudioReader(*inputStream,startInBigFile,length)) {
							wcout << "音频文件写入错误,退出了啊" << endl;
							throw 3;
						}

						cout << ".";
					} else {
						cout << "some thing wrong!" << endl;
						throw 3;
					}
				}
			} else {
				cout << "some thing wrong!" << endl;
				throw 3;
			}
		}
	} else {
		cout << "some thing wrong!" << endl;
		throw 3;
	}
}