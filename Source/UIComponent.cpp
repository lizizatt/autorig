/*
  ==============================================================================

    UIComponent.cpp
    Created: 16 Jul 2018 1:34:29pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "UIComponent.h"
#include "Main.h"

//==============================================================================
UIComponent::UIComponent()
{
    addAndMakeVisible(autoRigLabel);
    addAndMakeVisible(loadOBJ);
    addAndMakeVisible(runMeshlabCleanup);
    addAndMakeVisible(outputLabel);
    
    autoRigLabel.setText("AutoRig", dontSendNotification);
    autoRigLabel.setJustificationType(Justification::centred);
    loadOBJ.setButtonText("Start");
    loadOBJ.addListener(this);
    runMeshlabCleanup.setButtonText("Run Meshlab Cleanup");
    runMeshlabCleanup.addListener(this);
    
    takeKeyboardFocus(Component::FocusChangeType::focusChangedDirectly);
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
    loadOBJ.setBounds(20, 50, 100, 20);
    runMeshlabCleanup.setBounds(loadOBJ.getX(), loadOBJ.getY() + loadOBJ.getHeight(), 100, 20);
    outputLabel.setBounds(20, getHeight() - 40, getWidth() - 40, 20);
}

void UIComponent::buttonClicked(Button *b)
{
    if (b == &loadOBJ) {
        
        FileChooser chooser ("Select an OBJ file to rig...",
                                                  File::nonexistent,
                                                  "*.obj");
        if (chooser.browseForFileToOpen()) {
            File file = chooser.getResult();
            AutoRigApplication::getAutoRig()->Clear();
            AutoRigApplication::getAutoRig()->LoadOBJ(file);
        }
    }
    
    if (b == &runMeshlabCleanup) {
        AutoRigApplication::getAutoRig()->runMeshlabCleanup();
    }
}
