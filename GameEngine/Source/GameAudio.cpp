//
//  GameAudio.cpp
//  GameEngine
//
//  Created by Tim Arterbury on 9/19/17.
//
//

#include "GameAudio.h"


GameAudio::GameAudio()
{
    formatManager.registerBasicFormats();
    
    // Run garbage collection at constant interval
    //this->startTimer(GARBAGE_COLLECTION_INTERVAL_MILISECONDS);
}

GameAudio::~GameAudio()
{
    // MixerAudioSource shutdown
    this->releaseResources();
    this->removeAllInputs();
}


void GameAudio::playAudioFile (File & audioFile, bool looping)
{
    AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
    
    if (reader != nullptr)
    {
        AudioFormatReaderSource * newSource = new AudioFormatReaderSource (reader, true);
                                                            // 2nd parameter, "true", in this constructor
                                                            // specifies to delete the AudioFormatReader
                                                            // when the AudioFormatReaderSouce is deleted
        newSource->setLooping(looping);
    
            
        this->addInputSource (newSource, true);
                                        // 2nd parameter, "false", specifies to not
                                        // delete the AudioFormatReaderSource when
                                        // removed from the MixerAudioSource, since
                                        // we are storing them in an OwnedArray that
                                        // will later deallocate them.

       //audioSources.add(newSource.release());
        //audioSources.add(newSource); // If not using a ScopedPointer for newSource
    }
}



//void GameAudio::timerCallback()
//{
//    /**
//        DEV-NOTE: This could be cleaned up in the future by maybe using an array
//        of AudioTransportSources and checking for isPlaying() or
//        hasStreamFinished(), but for now, this essentially does the same thing.
//     */
//    
//    Array<int> audioSourceIndeciesToDelete;
//    
//    lock.enter();
//    
//        // Look through audio sources and deallocate resources if the audio is done
//        // being used
//        for (int i = 0; i < audioSources.size(); ++i)
//        {
//            // Get pointer to audio source
//            AudioFormatReaderSource* audioSource = audioSources[i];
//            
//            if (!audioSource->isLooping())
//            {
//                if (audioSource->getNextReadPosition() > audioSource->getTotalLength())
//                {
//                    // Remove from MixerAudioSource & releases its resources
//                    this->removeInputSource(audioSource);
//                    
//                    // Prepare to delete the audioSource pointer
//                    audioSourceIndeciesToDelete.add(i);
//                }
//            }
//        }
//        
//        // Remove unused audio source pointers
//        for (int i : audioSourceIndeciesToDelete)
//        {
//            audioSources.remove(i, true);
//            
//            // REMOVE LATER
//            DBG("Audio was deleted.");
//        }
//    
//    lock.exit();
//}
