/*
  ==============================================================================

  This file is part of the CombineAudio tool.

  Permission is granted to use this software under the terms of either:
  a) the GPL v2 (or any later version)

  Details of these licenses can be found at: www.gnu.org/licenses

  ==============================================================================
*/

#ifndef AUDIOXML2MERGEFILES_H_INCLUDED
#define AUDIOXML2MERGEFILES_H_INCLUDED

#include "JuceHeader.h"

/**
 set mergeFiles to pathToSaveMergeFiles,xml will be delete auto.
*/
void AudioDir2MergeFiles(const String& sourcePath,int64 muttingTimeEachFile,const String& targetPath);


#endif  // AUDIOXML2MERGEFILES_H_INCLUDED
