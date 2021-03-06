
/*

	holds an array of all draggable/droppable objects
	Objects mostly
*/

#pragma once

#include "CoreEngine.h"
#include "InspectorUpdater.h"

class ObjectBrowser : public Component, public Button::Listener, public InspectorUpdater {
public:
	ObjectBrowser() {
		addAndMakeVisible(enemy);
		addAndMakeVisible(block);
		addAndMakeVisible(collectable);
		addAndMakeVisible(checkpoint);
		enemy.setButtonText("Enemy Character");
		block.setButtonText("Block");
		collectable.setButtonText("Collectable");
		checkpoint.setButtonText("Transition Point");
		block.addListener(this);
		enemy.addListener(this);
		collectable.addListener(this);
		checkpoint.addListener(this);
		//addAndMakeVisible(scrollBar);
		//scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

	}
	~ObjectBrowser() {

	}
	void setCoreEngine(CoreEngine* engine){
		coreEngine = engine;
	}
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {		
			//g.fillAll(Colours::aquamarine);
		 //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	}

	void resized() override
	{
		juce::Rectangle<int> r = getLocalBounds();
		int BUTTON_HEIGHT = r.getHeight() / NUM_VISIBLE_BUTTONS;
		
		enemy.setBounds(r.getX(), 0, getWidth(), BUTTON_HEIGHT);
		block.setBounds(r.getX(), BUTTON_HEIGHT, getWidth(), BUTTON_HEIGHT);
		collectable.setBounds(r.getX(), BUTTON_HEIGHT*2, getWidth(), BUTTON_HEIGHT);
		checkpoint.setBounds(r.getX(), BUTTON_HEIGHT * 3, getWidth(), BUTTON_HEIGHT);
		//scrollBar.setBounds(getLocalBounds());
	}

	void buttonClicked(Button* button) override
	{
		if (button == &block)
		{
			coreEngine->addBlock();
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
        else if (button == &enemy)
		{
			coreEngine->addEnemy();
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
		else if (button == &collectable)
		{
			coreEngine->addCollectable();
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
		else if (button == &checkpoint)
		{
			coreEngine->addCheckpoint();
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
	}

private:
	const int NUM_VISIBLE_BUTTONS = 5;
	CoreEngine* coreEngine;
	TextButton enemy;
	TextButton collectable;
	TextButton block;
	TextButton checkpoint;
	//Slider scrollBar;
};

