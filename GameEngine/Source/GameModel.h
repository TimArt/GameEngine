#pragma once
#include "DrawableObject.h"
#include "../JuceLibraryCode/JuceHeader.h"

class GameModel {

public:

	GameModel() {
		drawableObjects.add(new DrawableObject());
	}
	~GameModel() {

	}

	OwnedArray<DrawableObject> drawableObjects;


private:



};