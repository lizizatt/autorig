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

//==============================================================================
/*
*/
class UIComponent    : public Component, public ButtonListener
{
public:
    UIComponent();
    ~UIComponent();

    void paint (Graphics&);
    void resized();

    void buttonClicked(Button *b) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
    
    Label autoRigLabel;
    TextButton loadOBJ;
    TextButton runMeshlabCleanup;
    Label outputLabel;
};


#endif  // UICOMPONENT_H_INCLUDED
