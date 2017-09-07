#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableObject.h"

class GameLoop : public Thread {
public:
	GameLoop() : Thread("GameLogic") {

	}

	~GameLoop() {
		gameModelCurrentFrame = nullptr;
		gameModelSwapFrameA = nullptr;
		gameModelSwapFrameB = nullptr;
	}

	void setGameModels(GameModel* currentFrame, GameModel* swapFrameA, GameModel* swapFrameB) {
		gameModelCurrentFrame = currentFrame;
		gameModelSwapFrameA = swapFrameA;
		gameModelSwapFrameB = swapFrameB;
	}
	

private:
	void run() {
		int i = 0;
		//Enter main game loop
		while (!threadShouldExit()) {
			i++;
			if (i > 10000000) {
				gameModelCurrentFrame->drawableObjects.getFirst()->translate(Vector3D<GLfloat>(0.01,0.01,0));
				i = 0;
			}
			

			
			
			

		}
		
	}

	DrawableObject object;
	GameModel* gameModelCurrentFrame;
	GameModel* gameModelSwapFrameA;
	GameModel* gameModelSwapFrameB;

};