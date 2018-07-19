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
    
    addAndMakeVisible(loadOBJ);
    addAndMakeVisible(outputLabel);
    addAndMakeVisible(loadExisting);
    addAndMakeVisible(models);
    addAndMakeVisible(rig);
    
    loadOBJ.setButtonText("Load File");
    loadOBJ.addListener(this);
    loadExisting.setButtonText("Load Existing");
    loadExisting.addListener(this);
    rig.setButtonText("Rig");
    rig.addListener(this);
    
    loadOBJ.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    loadOBJ.setColour(TextButton::ColourIds::buttonOnColourId, Colours::grey);
    loadOBJ.setColour(TextButton::ColourIds::textColourOffId, Colours::whitesmoke);
    loadOBJ.setColour(TextButton::ColourIds::textColourOnId, Colours::whitesmoke);
    loadExisting.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    loadExisting.setColour(TextButton::ColourIds::buttonOnColourId, Colours::grey);
    loadExisting.setColour(TextButton::ColourIds::textColourOffId, Colours::whitesmoke);
    loadExisting.setColour(TextButton::ColourIds::textColourOnId, Colours::whitesmoke);
    rig.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    rig.setColour(TextButton::ColourIds::buttonOnColourId, Colours::grey);
    rig.setColour(TextButton::ColourIds::textColourOffId, Colours::whitesmoke);
    rig.setColour(TextButton::ColourIds::textColourOnId, Colours::whitesmoke);
    
    background = ImageCache::getFromMemory(BinaryData::Hackathon_18_FINAL_jpg, BinaryData::Hackathon_18_FINAL_jpgSize);
    
    takeKeyboardFocus(Component::FocusChangeType::focusChangedDirectly);
    
    ModelsUpdated();
}

UIComponent::~UIComponent()
{
}

void UIComponent::paint (Graphics& g)
{
    g.drawImage(background, 0, 0, getWidth(), getHeight(), 0, 0, background.getWidth(), background.getHeight());
}

void UIComponent::resized()
{
    loadOBJ.setBounds(20, 20, getWidth() / 2 - 40, 30);
    loadExisting.setBounds(loadOBJ.getRight() + 20, 20, getWidth() / 2 - 40, 20);
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
