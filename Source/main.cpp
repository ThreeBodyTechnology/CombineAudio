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
#include "AudioDirectory2MergeFiles.h"
#include "MergeFiles2AudioDirectory.h"
using namespace std;

int main()
{
	wcout << "哥您好!,我是AudioFileLineUp,请问您是想分离还是想合并音频文件，点1合并，点2分离" << endl;
	int input = 0;
	for(;;) {
		cin >> input;
		if(input == 1) {
			break;
		} else if(input == 2){
			break;
		}
	}
	for(;;) {
		wcout << "请输入输入目录:" << endl;
		wchar_t a[6000];
		wcin >> a;
		wcout << "请输入输出目录:" << endl;
		wchar_t b[6000];
		wcin >> b;
		if(wcscmp(a,b)==0) {
			wcout << "哥,俩目录别一样啊,重来吧您呐!" << endl;
		} else {
			try {
				if(input == 1){
					AudioDir2MergeFiles(a,20,b);
				} else {
					MerFiles2AudioDirectory(a,b);
				}
			} catch(...) {
				wcout << "!!!!!处理失败!!!!!!" << endl;
				break;
			}
			wcout << "!!!!处理成功!!!!" << endl;
			break;
		}
	}
	system("PAUSE");
	return 0;
}
