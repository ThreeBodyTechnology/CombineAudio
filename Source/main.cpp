/*
  ==============================================================================

  This file is part of the CombineAudio tool.

  Permission is granted to use this software under the terms of either:
  a) the GPL v2 (or any later version)

  Details of these licenses can be found at: www.gnu.org/licenses

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
	wcout << "������!,����AudioFileLineUp,������������뻹����ϲ���Ƶ�ļ�����1�ϲ�����2����" << endl;
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
				if(input == 1){
					AudioDir2MergeFiles(a,20,b);
				} else {
					MerFiles2AudioDirectory(a,b);
				}
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
