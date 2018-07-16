/*
  ==============================================================================

    GraphicsComponent.cpp
    Created: 16 Jul 2018 1:34:44pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphicsComponent.h"


//==============================================================================
GraphicsComponent::GraphicsComponent()
{
    setOpaque (true);
    context.setRenderer (this);
    context.attachTo (*this);
    context.setContinuousRepainting (true);
}

GraphicsComponent::~GraphicsComponent()
{
    context.detach();
}

void GraphicsComponent::newOpenGLContextCreated()
{
}

void GraphicsComponent::renderOpenGL()
{
    OpenGLHelpers::clear (Colours::black);
}

void GraphicsComponent::openGLContextClosing()
{
}

void GraphicsComponent::paint (Graphics& g)
{
}

void GraphicsComponent::resized()
{
}
