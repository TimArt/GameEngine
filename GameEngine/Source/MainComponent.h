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
class MainContentComponent   : public Component, public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button *) override;

private:
    //==============================================================================
    TextButton button;
    Colour backgroundColour;
    GameView gameView;
	GameLoop gameLoop;
	GameModel gameModelCurrentFrame;
	GameModel gameModelSwapFrameA;
	GameModel gameModelSwapFrameB;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
