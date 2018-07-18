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
    autoRig = AutoRigApplication::getAutoRig();
    autoRig->AddListener(this);
    
    addAndMakeVisible(autoRigLabel);
    addAndMakeVisible(loadOBJ);
    addAndMakeVisible(outputLabel);
    addAndMakeVisible(loadExisting);
    addAndMakeVisible(models);
    addAndMakeVisible(rig);
    
    autoRigLabel.setText("AutoRig", dontSendNotification);
    autoRigLabel.setJustificationType(Justification::centred);
    loadOBJ.setButtonText("Start");
    loadOBJ.addListener(this);
    loadExisting.setButtonText("Load Existing");
    loadExisting.addListener(this);
    rig.setButtonText("Rig");
    rig.addListener(this);
    
    takeKeyboardFocus(Component::FocusChangeType::focusChangedDirectly);
    
    ModelsUpdated();
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
    loadOBJ.setBounds(20, 100, 100, 20);
    loadExisting.setBounds(loadOBJ.getRight(), 100, 100, 20);
    models.setBounds(loadExisting.getX(), loadExisting.getBottom(), loadExisting.getWidth(), loadExisting.getHeight());
    outputLabel.setBounds(20, getHeight() - 40, getWidth() - 40, 20);
    
    rig.setBounds(20, 200, getWidth() - 40, 30);
}

void UIComponent::buttonClicked(Button *b)
{
    if (b == &loadOBJ) {
        
        FileChooser chooser ("Select an OBJ file to rig...",
                                                  File::nonexistent,
                                                  "*.obj");
        if (chooser.browseForFileToOpen()) {
            outputLabel.setText("Loading obj...", dontSendNotification);
            outputLabel.setColour(Label::ColourIds::textColourId, Colours::purple);
            File file = chooser.getResult();
            autoRig->Clear();
            bool res = autoRig->LoadOBJ(file);
            autoRig->SetActive(autoRig->models.size() - 1);
            Post(res, "LoadOBJ succeeded", "LoadOBJ failed");
            outputLabel.setText("Loaded obj.", dontSendNotification);
            outputLabel.setColour(Label::ColourIds::textColourId, Colours::green);
        }
    }
    
    if (b == &loadExisting) {
        int id = models.getSelectedItemIndex();
        if (id >= 0 && id < autoRig->models.size()) {
            autoRig->Clear();
            autoRig->SetActive(id);
        }
    }
    
    if (b == &rig) {
        outputLabel.setText("Rig running...", dontSendNotification);
        outputLabel.setColour(Label::ColourIds::textColourId, Colours::purple);
        autoRig->StartRig();
    }
}

void UIComponent::ModelsUpdated()
{
    models.clear();
    for (int i = 0; i < autoRig->models.size(); i++) {
        models.addItem(autoRig->models[i]->name, i+1);
    }
}

void UIComponent::RigDone()
{
    outputLabel.setText("Rig done.", dontSendNotification);
    outputLabel.setColour(Label::ColourIds::textColourId, Colours::green);
}

void UIComponent::Post(bool error, String success, String fail) {
    if (error) {
        cout << error << "\n";
    }
    else {
        cout << success << "\n";
    }
}
