/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphicsComponent.h"
#include "UIComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    ScopedPointer<GraphicsComponent> graphicsComponent;
    ScopedPointer<UIComponent> uiComponent;
    
    OpenGLContext context;
    
public:
    //==============================================================================
    MainContentComponent()
    {
        addAndMakeVisible(graphicsComponent = new GraphicsComponent());
        addAndMakeVisible(uiComponent = new UIComponent());
        setSize (1200, 800);
    }

    void paint (Graphics& g) override
    {
    }

    void resized() override
    {
        float uiWidth = .2;
        uiComponent->setBounds(0, 0, getWidth() * uiWidth, getHeight());
        graphicsComponent->setBounds(uiComponent->getRight(), 0, getWidth() - uiComponent->getRight(), getHeight());
    }


private:
    //==============================================================================

    // private member variables



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
