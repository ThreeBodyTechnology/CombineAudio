#CombineAudio
==========================

combine a number of wav audio files into a big audio file, and then make the entire file back into multiple audio files, 
if you have 100 drum tracks but only one neve 1073 , you will need this.

This project used JUCE library:
https://github.com/julianstorer/JUCE

Now I'm making the console version, I will add a GUI interface in the future.

for pro audio user, sometimes you may need the entire audio samples folder to combine to one sample file,
including sub-folders to be processed (for example, different styles of drum samples). so this tool will search
all your sub-folders . and combine them to one file , if you want this big file to go back, it will upzip this file
to a folder, including all sub-folders . All the original file directories will not be changed.

My English is not good,I'm guess you don't understand what I said above ... so .....

Lets say you have a piano sample library, its contains a "~/samples" dir,and in this dir you have 
"/hi_vel/C1/","/lo_vel/E5","/lo_vel/B7"...... sub-dir . and you have good
EQ and compressor, you want to make all sample files in "~/samples" to be process, CombineAudio will
take care of all sub-dirs.

Or ,lets say you have a song to mix , and the song has 100 tracks. you'd like to make all 100 tracks 
through NEVE preamp,but you don't have a NEVE mixer.you only have one NEVE 1073 pair. so you 
may want all tracks through 1073 and record back one by one. you never done this easy!!!
and now CombineAudio can help . what you need to do is:

###step1:
	open the CombineAudio , you will see a console open(Now I'm making the console version
	, and I will add a GUI interface in the future). type "1" if you want to combine the files.
	then type in which dir you want combine.
	this dir must only contain wav audio files(mono or stereo) in the same sample rate.
	if this dir has a sub-dir , CombineAudio will handle those .
	if each of the audio files is mono channel,the combine file will be mono,and if even one file is stereo,
	the combine file will be stereo.(but they will be back to the original number of the channel). if 
	the combine file is too big(>1024m), it will combine the files to several combine files.
	(will NOT cut original files). there will be a 0.25s mute time between original files.
	and a click sound in the beginning of every combine files. so that you can "see" the delay 
	you process the combine files.

###step2:
	you will be requested a new dir to save the combine files,type in a new dir. and wait, please.
	the console will be closed when the work is done. a dir "xxxx.merge" will be created. all combine files 
	will be there.

###step3:
	when the work is done,in this new dir you can see several combine files and a xml file. never change
	this xml file! and never change the names of all combine files!

###step4:
	now you can process what you want to all combine files. like adding a compressor, EQ , hardware preamp ...
	do what you want to and save the files in the same sample rate. the only thing you have to care about is ,
	you can NEVER CHANGE THE TIME ! like adding something in the beginning of the file ,a delay
	,or a Linear Phase EQ without delay compensation.......
	lots of effects will change the time without warning you(even the hardware). that's why I added a 
	click sound in the beginning of the files. generally speaking, when you are done the process you want and record back,
	the click sound will be change to some thing like a "sinc". you can zoom in to see how click sound change.
	if there was delay , kill it before step5. then save all combine files(don't change the file name).

###step5:
	now you want cut combine files back, reopen CombineAudio ,type in "2" .type in the dir all combine files lives.
	like "abc.merge". type in the dir where you want to save the files and wait...

###step6:
	files will go back to small files in the dir you type in. including all sub-dir. and then ,have fun!

中文版:

CombineAudio
==========================

将一大堆音频文件收尾相接成数个大文件,然后可再分离成小文件.如果你有100轨需要处理但只有一个话放,你会需要它的.

本项目使用了JUCE库:
https://github.com/julianstorer/JUCE

当前我只做了个控制台界面,有空的时候我将会添加一个GUI界面.
