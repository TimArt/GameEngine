//
//  WorldPhysics
//
//  Created by Trystan Jasperson.
//
//
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

class WorldPhysics {
	
public:
	WorldPhysics() : world(b2Vec2(0.0f,-10.0f))
	{
		velocityIterations = 2.0f;
		positionIterations = 6.0f;
		timeStep = 1.0f / 15.0f;
        /*
        create the body first, giving it a position
        */
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, -6.1f);
        /*
         body is created in the world
         */
        b2Body* groundBody = world.CreateBody(&bodyDef);
        
        /*
         create a shape(box for this instance)
         */
		b2PolygonShape groundBox;
		
		//b2PolygonShape wallBox;
		groundBox.SetAsBox(100000000.0f, 0.5f);
		//wallBox.SetAsBox(0.3f, 100.0f);

		
        /*
         assign the box shape and density to the body
         When you attach a shape to a body using a fixture, the shape's coordinates become local to the body. So
         when the body moves, so does the shape.
         */
        
		// groundBody->CreateFixture(&groundBox, 0.0f);
		bodyDef.position.Set(0.0f, 6.1f);
		//groundBody = world.CreateBody(&bodyDef);
		//groundBody->CreateFixture(&groundBox, 0.0f);

		
		bodyDef.position.Set(-8.7f, 0.0f);
		//groundBody = world.CreateBody(&bodyDef);

		//groundBody->CreateFixture(&wallBox, 0.0f);

		//bodyDef.position.Set(8.7f, 0.0f);
		world.SetAllowSleeping(false);
		//groundBody = world.CreateBody(&bodyDef);
		//groundBody->CreateFixture(&wallBox, 0.0f);
		gravityLev = Normal;
	}

	~WorldPhysics()
	{

	}

	enum gravityLevel {
		Normal = 0,
		AntiGrav = 1,
		HighGrav = 2
	};

	/**************************************************************************
	*
	*	remove all the objects in the world
	*
	**************************************************************************/
	void clearWorld() 
	{
        for (auto bodyObject = world.GetBodyList(); bodyObject; )
        {
            b2Body* oldBody = bodyObject;
            bodyObject = bodyObject->GetNext();
            world.DestroyBody(oldBody);
            oldBody = NULL;
        }
	}

    void removeObject (b2Body * bodyToDestroy)
    {
        world.DestroyBody (bodyToDestroy);
    }

	/**************************************************************************
	*
	*	Progress through the world using a set amount of time(timestep), and
	*	Iterate through calculations for velocity and position the set 
	*	number of iterations.
	**************************************************************************/
	void Step()
	{
		world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
	}

	/**************************************************************************
	*
	*	Progress through the world using a frameTimeStep, and
	*	Iterate through calculations for velocity and position the set
	*	number of iterations.
	**************************************************************************/
	void Step(float32 timeStep)
	{
		world.Step(timeStep, this->velocityIterations, this->positionIterations);
	}
	/**************************************************************************
	*
	*	return access to the private world
	*
	**************************************************************************/
	b2World & getWorld()
	{
		return world;
	}

	/**************************************************************************
	*
	*	set the gravity
	*
	**************************************************************************/
	void setGravity(GLfloat gravx,GLfloat gravy)
	{
		world.SetGravity(b2Vec2(gravx,gravy));
	}

	void setGravity(gravityLevel grav)
	{
		gravityLev = grav;
		switch (grav) {
		case Normal:
			world.SetGravity(b2Vec2(0, -9.8));
			break;
		case AntiGrav:
			world.SetGravity(b2Vec2(0, 9.8));
			break;
		case HighGrav:
			world.SetGravity(b2Vec2(0, -18.0));
			break;
		}
	}

	gravityLevel getGravityLevel() {
		return gravityLev;
	}
	/**************************************************************************
	*
	*	set the gravity
	*
	**************************************************************************/
	float32 getTimeStep()
	{
		return timeStep;
	}
	/**************************************************************************
	*
	*	clear all customized forces in the world
	*
	**************************************************************************/
	void clearForces()
	{
		world.ClearForces();
	}

	float getGravity() {
		return world.GetGravity().y;
	}

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the game
		ValueTree worldPhysicsSerialization = ValueTree("WorldPhysics");

		ValueTree gravitySerialization = ValueTree("Gravity");


		int gravityInt;

		switch (gravityLev) {
		case Normal:
			gravityInt = 0;
			break;
		case AntiGrav:
			gravityInt = 1;
			break;
		case HighGrav:
			gravityInt = 2;
			break;
		}

		gravitySerialization.setProperty(Identifier("value"), var(gravityInt), nullptr);

		worldPhysicsSerialization.addChild(gravitySerialization, -1, nullptr);
		

		return worldPhysicsSerialization;
	}

	void parseWorldPhysics(ValueTree levelTree) {

	

		ValueTree worldPhysics = levelTree.getChildWithName(Identifier("Gravity"));

		int gravityLevelInt = worldPhysics.getProperty(Identifier("value"));

		

		switch (gravityLevelInt) {
		case 0:
			setGravity(Normal);
			break;
		case 1:
			setGravity(AntiGrav);
			break;
		case 2:
			setGravity(HighGrav);
			break;
		}

	}
	
private:
	b2World world;
	gravityLevel gravityLev;
	juce::int32 velocityIterations;
	juce::int32 positionIterations;
	float32 timeStep;

	JUCE_LEAK_DETECTOR(WorldPhysics)
};
