/*
  ==============================================================================

    GraphicsComponent.h
    Created: 16 Jul 2018 1:34:44pm
    Author:  Liz Izatt

  ==============================================================================
*/

#ifndef GRAPHICSCOMPONENT_H_INCLUDED
#define GRAPHICSCOMPONENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
class GraphicsComponent    : public Component, public OpenGLRenderer
{
public:
    GraphicsComponent();
    ~GraphicsComponent();
    
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override;
    virtual void openGLContextClosing() override;
    
    void paint (Graphics&);
    void resized();
    
public:
    OpenGLContext context;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicsComponent)
};




#endif  // GRAPHICSCOMPONENT_H_INCLUDED
