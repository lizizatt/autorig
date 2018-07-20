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
#include "motion.h"

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
    addAndMakeVisible(openFolder);
    addAndMakeVisible(genFbx);
    
    glComp = new MyWindow();
    glWindow = new Window(glComp);
    
    loadOBJ.setButtonText("Load File");
    loadOBJ.addListener(this);
    loadExisting.setButtonText("Load Existing");
    loadExisting.addListener(this);
    rig.setButtonText("Rig");
    rig.addListener(this);
    openFolder.setButtonText("Open Folder");
    openFolder.addListener(this);
    genFbx.addListener(this);
    genFbx.setButtonText("Generate FBX");
    
    buttonHelper(loadOBJ);
    buttonHelper(loadExisting);
    buttonHelper(rig);
    buttonHelper(openFolder);
    buttonHelper(genFbx);
    
    background = ImageCache::getFromMemory(BinaryData::Hackathon_18_FINAL_jpg, BinaryData::Hackathon_18_FINAL_jpgSize);
    
    takeKeyboardFocus(Component::FocusChangeType::focusChangedDirectly);
    
    ModelsUpdated();
}

void UIComponent::buttonHelper(Button &b)
{
    b.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
    b.setColour(TextButton::ColourIds::buttonOnColourId, Colours::grey);
    b.setColour(TextButton::ColourIds::textColourOffId, Colours::whitesmoke);
    b.setColour(TextButton::ColourIds::textColourOnId, Colours::whitesmoke);
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
    openFolder.setBounds(loadExisting.getX(), rig.getBottom() + 5, getWidth() / 2 - 40, 20);
    genFbx.setBounds(loadOBJ.getX(), rig.getBottom() + 5, getWidth() / 2 - 40, 20);
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
    
    if (b == &openFolder && autoRig->activeModel != nullptr) {
        autoRig->activeModel->obj.revealToUser();
    }
    
    if (b == &genFbx) {
        autoRig->GenFBX();
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
    MessageManagerLock lock;
    outputLabel.setText("Rig done.", dontSendNotification);
    outputLabel.setColour(Label::ColourIds::textColourId, Colours::green);
    
    File motionFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile("contents").getChildFile("Resources").getChildFile("walk.txt");
    
    
    glComp->addMesh(new DefMesh(*autoRig->activeModel->mesh_poisson, *autoRig->activeModel->skeleton, autoRig->activeModel->pOut.embedding, *autoRig->activeModel->pOut.attachment, new Motion(motionFile.getFullPathName().toRawUTF8())));
}

void UIComponent::NewActiveModel()
{
    glComp->addMesh(new StaticDisplayMesh(*autoRig->activeModel->mesh_poisson));
}

void UIComponent::Post(bool error, String success, String fail) {
    if (error) {
        cout << error << "\n";
    }
    else {
        cout << success << "\n";
    }
}
