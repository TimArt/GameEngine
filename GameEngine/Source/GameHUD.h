//
//  GameHUD.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** Renders a Heads Up Display with a transparent background.
 */
class GameHUD : public Component
{
public:

    GameHUD()
    {
        addAndMakeVisible(slider);
        setOpaque(false);
    }
    
    ~GameHUD()
    {
        
    }
    
    
    // JUCE Callbacks ==========================================================
    
    void paint (Graphics &g) override
    {
		g.drawText("OpenGL with Shaders Yo", 25, 30, 300, 30, Justification::left);
		g.drawLine(20, 30, 170, 30);
		g.drawLine(20, 60, 170, 60);
    }
    
    void resized () override
    {
        slider.setBounds(20, 75, 200, 50);
    }
    
private:
    Slider slider;
    
    
};
