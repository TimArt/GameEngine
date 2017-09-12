/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameView.h"
#include "GameLoop.h"
#include "GameModel.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public Button::Listener, public Thread
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button *) override;

	/**TEST**/
	void childThreadFinished();

private:
    //==============================================================================
    TextButton button;
    Colour backgroundColour;
	GameLoop gameLoop;
    GameView gameView;
	
	GameModel* gameModelCurrentFrame;
	GameModel* gameModelSwapFrameA;
	GameModel* gameModelSwapFrameB;
	GameModel ** gameModelLogicContainer;
	GameModel ** gameModelRenderContainer;


	CriticalSection renderMutex;
	CriticalSection logicMutex;
	WaitableEvent logicWaitable;
	WaitableEvent renderWaitable;
	WaitableEvent coreWaitable;
	 
	void run() override;



    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
