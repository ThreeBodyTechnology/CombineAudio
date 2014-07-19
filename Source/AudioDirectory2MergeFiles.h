/*
  ==============================================================================

    AudioXml2MergeFiles.h
    Created: 19 Jul 2014 3:44:02am
    Author:  mengke

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
