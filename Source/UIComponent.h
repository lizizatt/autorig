/*
  ==============================================================================

    UIComponent.h
    Created: 16 Jul 2018 1:34:29pm
    Author:  Liz Izatt

  ==============================================================================
*/

#ifndef UICOMPONENT_H_INCLUDED
#define UICOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AutoRig.h"

//==============================================================================
/*
*/
class UIComponent    : public Component, public ButtonListener, public AutoRigListener
{
public:
    UIComponent();
    ~UIComponent();

    void paint (Graphics&);
    void resized();

    void buttonHelper(Button &b);
    void buttonClicked(Button *b) override;
    
    virtual void ModelsUpdated() override;
    virtual void RigDone() override;
    
    void Post(bool error, String success, String fail);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
    
    ComboBox models;
    TextButton loadExisting;
    TextButton loadOBJ;
    TextButton rig;
    TextButton openFolder;
    Label outputLabel;
    AutoRig *autoRig;
    
    Image background;
};


#endif  // UICOMPONENT_H_INCLUDED
