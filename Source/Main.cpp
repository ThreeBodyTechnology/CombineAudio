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
    
    StringPairArray mata = WavAudioFormat::createBWAVMetadata ( String::empty          ,
                                                                String::empty          ,
                                                                String::empty          ,
                                                                Time::getCurrentTime() ,
                                                                bigLength              ,
                                                                String::empty          );
    
    ScopedPointer<AudioFormatWriter> writer= manager.createWriterFor( stream,sampleRate,channelNum,32,mata,0 );
    
    if (writer==nullptr) { cout<<"音频写入器建立失败"<<endl; throw 3; }
    return writer.release();
}

tuple<int,int64> find_all_audio_file_in_rot(const String& rot,AudioFileArray& filesToRead)
{
    File file(rot);
    
    if( (! file.exists()) || file.existsAsFile() ) {
        wcout<<"文件路径不存在"<<endl;
        throw 3;
    }
    
    if (!file.isDirectory()) {
        wcout<<"文件路径不存在"<<endl;
        throw 3;
    }
    
    Array<File> files;
    int number = file.findChildFiles(files, File::findFiles|File::ignoreHiddenFiles ,true,"*.wav");
    
    if (number<=0) {
        wcout<<"找到的音频文件数为0,我退出了啊"<<endl;
        throw 3;
    }
    
    wcout<<"共找到音频文件:"<<files.size()<<"个,名称为:"<<endl;
    for (int i=0; i<files.size(); ++i) {
        wcout<<files[i].getFileName().toUTF16()<<endl;
    }
    
    ScopedPointer<WavAudioFormat> reader=new WavAudioFormat();
    OwnedArray<AudioFormatReader> readers;
    
    for (int i=0; i<files.size(); ++i) {
        auto* k=reader->createReaderFor(files[i].createInputStream(), true);
        
        if(k==nullptr) {
            wcout<<"文件"<<files[i].getFileNameWithoutExtension().toUTF16()<<"读取错误"<<endl;
            throw 3;
        }
        
        readers.add(k);
    }
    
    jassert(readers.size()==files.size());
    
    double sampleRate = readers[0]->sampleRate;
    int all_file_ch_num=1;
    int64 all_file_length =0;
    for (int i=0; i<readers.size(); ++i) {
        if( sampleRate!=readers[i]->sampleRate ) {
            wcout<<"文件"<<files[i].getFileNameWithoutExtension().toUTF16()
            <<"......的采样率与其他不同,我暂时不能处理采样率不相同的一堆文件,退出了啊"<<endl;
            throw 3;
        }
        if( readers[i]->numChannels <= 0 ) {
            wcout<<"文件读取错误,读取到了通道数小于等于0的wav文件,退出了啊"<<endl;
            throw 3;
        } else if( readers[i]->numChannels > 2 ) {
            wcout<<"检测到通道数大于2的音频文件,我们暂时不能处理超过两通道的的文件,退出了啊"<<endl;
            throw 3;
        } else if(readers[i]->numChannels > 1 ) {
            all_file_ch_num=2;
            jassert(readers[i]->numChannels==2);
        }
        
        if(readers[i]->lengthInSamples<=0) {
            wcout<<"检测到尺寸小于等于0的文件,我不知道该怎么办了,推出了啊"<<endl;
            throw 3;
        } else {
            all_file_length+=readers[i]->lengthInSamples;
        }
    }
    
    for (int i=0; i<readers.size(); ++i) {
        filesToRead.add( new AudioFile( files[i].getFileNameWithoutExtension(),readers.removeAndReturn(i) ));
    }
    
    return all_file_ch_num;
};

void save_some_file_to_big_file(int channelNum,double sampleRate,AudioFileArray& array,const String& rotPath)
{
    AudioSampleBuffer empty_buffer(1,(int)sampleRate*0.25);
    empty_buffer.clear();
    String bigFileRot(rotPath);
    
    if ( !File(bigFileRot).exists() ) { cout<<"音频文件的上层目录不存在,这不科学"<<endl; throw 3; }
    if ( !File(bigFileRot).isDirectory() ) { cout<<"音频文件的上层目录不是文件夹,这不科学"<<endl; throw 3; }
    
    File bigFile;
    for(int i=0; (bigFile=File(bigFileRot+"lineup_sub_file"+String(i)+".wav")).exists() ;++i);
    
    File bigFileXml=File(bigFile.getFullPathName()+".xml");
    
    if (bigFileXml.exists()) {
        wcout<<"建立了新的bigFile可是xml却已经存在了,这是严重错误,我懵了,退出了啊"<<endl;
        throw 3;
    }
    
    if(bigFile.create()!=Result::ok()) { cout<<"音频文件建立失败"<<endl; throw 3; }
    if(bigFileXml.create()!=Result::ok()) { cout<<"xml文件建立失败"<<endl; throw 3; }
    
    ScopedPointer<OutputStream> ptr = bigFile.createOutputStream();
    if(ptr==nullptr) { cout<<"音频文件写入器建立失败"<<endl; throw 3; }
    
    ScopedPointer<AudioFormatWriter> writer=create_simple_audio_writer(ptr.release(),channelNum,sampleRate,0);
    if(writer==nullptr) { cout<<"音频文件写入器建立失败"<<endl; throw 3; }

    ScopedPointer<XmlElement> xml=new XmlElement(bigFileXml.getFileName());
    
    int64 big_file_size_counter=0;
    int processing_counter=0;
    
    for (int i = 0; i < array.size(); ++i) {
        ScopedPointer<AudioFile> file=array.removeAndReturn(i);
        const String& small_file_name = get<0>(*file);
        AudioFormatReader& small_file_reader = *get<1>(*file);
        
        XmlElement* a = new XmlElement("sub_file");
        
        a->setAttribute ("name", small_file_name);
        a->setAttribute ("startInBigFile", String(big_file_size_counter));
        a->setAttribute ("length",String(small_file_reader.lengthInSamples) );
        
        xml->addChildElement(a);
        
        if( !writer->writeFromAudioReader(small_file_reader,0,-1) ) {
            wcout<<"音频文件写入错误,退出了啊"<<endl;
        }
        big_file_size_counter+=small_file_reader.lengthInSamples;
        
        if( !writer->writeFromAudioSampleBuffer(empty_buffer,0,-1)) {
            wcout<<"音频文件写入错误,退出了啊"<<endl;
        }
        big_file_size_counter+=empty_buffer.getNumSamples();
        
        if( big_file_size_counter*4*channelNum/1024/1024 >= 1000 ) {
            break;
        }
        
        if(processing_counter++ <= 5){
            wcout<<".";
        } else {
            wcout<<"\b";
            processing_counter=0;
        }
        
    }
    
    if( ! xml->writeToFile( bigFileXml , "" ) ) {
        cout<<"xml文件写入失败"<<endl;
        delete writer.release();
        while(!bigFile.deleteFile());
        throw 3;
    }
};



void create_xml_from_AudioFileArray(const String& rotPath,const String& targetPath)
{
    AudioFileArray allFileArray;
    tuple<int,int64> result = find_all_audio_file_in_rot(rotPath,allFileArray);
    if(allFileArray.size()==0) {
        wcout<<"目录里没货啊,退出了啊"<<endl;
        throw 3;
    }
    wcout<<"processing";
    while(allFileArray.size()>0) {
        save_some_file_to_big_file(get<0>(result), get<1>(*allFileArray[0])->sampleRate , allFileArray, targetPath);
    }
};

//==============================================================================
int main (int argc, char* argv[])
{
    wcout<<"哥您好!,我是AudioFileLineUp,我会根据您选择的目录自动识别您是想分离还是想合并音频文件"<<endl;
    for(;;) {
        wcout<<"请输入输入目录:"<<endl;
        wchar_t a[6000];
        wcin>>a;
        wcout<<"请输入输出目录:"<<endl;
        wchar_t b[6000];
        wcin>>b;
        if(wcscmp(a,b)) {
            wcout<<"哥,俩目录别一样啊,重来吧您呐!"<<endl;
        } else {
            try {
                create_xml_from_AudioFileArray(a,b);
            } catch(...) {
                wcout<<"!!!!!处理失败!!!!!!想退出直接右上角,不想退出请接着来~"<<endl;
            }
            wcout<<"处理成功!想退出直接右上角,不想退出请接着来~"<<endl;
        }
    }
    return 0;
}
