#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableObject.h"
#include "MainComponent.h"

class GameLoop : public Thread {
public:
	GameLoop() : Thread("GameLogic") {
		
	}

	~GameLoop() {
		delete gameModelCurrentFrame;
		delete gameModelContainer;
		gameModelCurrentFrame = nullptr;

	}

	void setGameModels(GameModel** container,GameModel* currentFrame) {
		gameModelCurrentFrame = currentFrame;
		gameModelContainer = container;
	}

	void setCoreWaitable(WaitableEvent* waitable) {
		coreWaitable = waitable;
	}

	void setLoopWaitable(WaitableEvent* waitable) {
		logicWaitable = waitable;

	}

private:


	void run() {

		
		currentTime = Time::currentTimeMillis();
		int64 checkTime = 0;

		//Enter main game loop
		while (!threadShouldExit()) {
			//wait for logic to say go
			logicWaitable->wait();

			newTime = Time::currentTimeMillis();
			deltaTime = newTime - currentTime;
			currentTime = newTime;
			checkTime += deltaTime;

			//output last framerate every second
			if (checkTime > 1000) {
				checkTime = 0;
				DBG(1000.0/deltaTime);
			}
			
			//update current frame and swap frame models
			gameModelCurrentFrame->drawableObjects.getFirst()->translate(Vector3D<GLfloat>(0.5*(deltaTime/1000.0), 0.5*(deltaTime / 1000.0), 0));
			(*gameModelContainer)->drawableObjects.getFirst()->translate(Vector3D<GLfloat>(0.5*(deltaTime / 1000.0), 0.5*(deltaTime / 1000.0), 0));

			
			//tell core engine logic is done
			coreWaitable->signal();
			
		}
		
	}

	DrawableObject object;
	GameModel* gameModelCurrentFrame;
	GameModel** gameModelContainer;
	WaitableEvent* logicWaitable;
	WaitableEvent* coreWaitable;
	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;


};