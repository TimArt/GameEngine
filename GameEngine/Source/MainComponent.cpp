/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent() : Thread("CoreEngine")
{
    //button.setButtonText ("Lolz wutt");
    //addAndMakeVisible (button);
    
    //backgroundColour = Colours::black;
    //button.addListener (this);

	gameModelCurrentFrame = new GameModel();
	gameModelSwapFrameA = new GameModel();
	gameModelSwapFrameB = new GameModel();

	gameLoop.setCoreWaitable(&coreWaitable);
	gameLoop.setLoopWaitable(&logicWaitable);

	gameView.setCoreWaitable(&coreWaitable);
	gameView.setLoopWaitable(&renderWaitable);

	gameModelLogicContainer = &gameModelSwapFrameA;

	gameModelRenderContainer = &gameModelSwapFrameB;


	gameLoop.setGameModels(gameModelLogicContainer,gameModelCurrentFrame);

	gameView.setGameModel(gameModelRenderContainer);

	gameLoop.startThread();

    addAndMakeVisible (gameView);

    gameView.setOpenGLEnabled (true);
    
    setSize (600, 400);

	this->startThread();
}

MainContentComponent::~MainContentComponent()
{
	gameView.setOpenGLEnabled(false);
	gameLoop.stopThread(200);

}

void MainContentComponent::paint (Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    //g.fillAll (backgroundColour);
    g.fillAll (Colours::white);
	
    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    button.setBounds (30, 30, 200, 200);
    
    gameView.setBounds(getLocalBounds());
}

void MainContentComponent::buttonClicked (Button * buttonPtr)
{
    if (buttonPtr == &button)
    {
        backgroundColour = Colours::white;
        repaint();
    }
}


void  MainContentComponent::run() {

	while (!threadShouldExit()) {

		renderWaitable.signal();

		logicWaitable.signal();


		coreWaitable.wait();


		coreWaitable.wait();

		if (*gameModelLogicContainer = gameModelSwapFrameA) {
			*gameModelLogicContainer = gameModelSwapFrameB;
			*gameModelRenderContainer = gameModelSwapFrameA;
		} else {
			*gameModelLogicContainer = gameModelSwapFrameA;
			*gameModelRenderContainer = gameModelSwapFrameB;
		}
		

	}
	
	
}

