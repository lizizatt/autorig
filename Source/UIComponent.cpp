/*
  ==============================================================================

    UIComponent.cpp
    Created: 16 Jul 2018 1:34:29pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "UIComponent.h"

//==============================================================================
UIComponent::UIComponent()
{
    addAndMakeVisible(autoRigLabel);
    addAndMakeVisible(startProcess);
    addAndMakeVisible(outputLabel);
    
    autoRigLabel.setText("AutoRig", dontSendNotification);
    autoRigLabel.setJustificationType(Justification::centred);
    startProcess.setButtonText("Start");
    startProcess.addListener(this);
    
    
}

UIComponent::~UIComponent()
{
}

void UIComponent::paint (Graphics& g)
{
}

void UIComponent::resized()
{
    autoRigLabel.setBounds(0, 0, getWidth(), 30);
    startProcess.setBounds(50, 20, 100, 20);
    outputLabel.setBounds(100, 20, getWidth() - 40, 20);
}

void UIComponent::buttonClicked(Button *b)
{
    if (b == &startProcess) {
        
        FileChooser chooser ("Select an OBJ file to rig...",
                                                  File::nonexistent,
                                                  "*.obj");
        if (chooser.browseForFileToOpen()) {
            File file = chooser.getResult();
        }
    }
}
