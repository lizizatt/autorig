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
#include "../DemoUI/MyWindow.h"
#include "../DemoUI/defmesh.h"

//==============================================================================
/*
*/
class UIComponent    : public Component, public ButtonListener, public AutoRigListener, public Timer, public ComboBoxListener
{
public:
    UIComponent();
    ~UIComponent();
    
    int count = 0;

    void paint (Graphics&);
    void resized();

    void buttonHelper(Button &b);
    void buttonClicked(Button *b) override;
    
    void timerCallback() override;
    
    virtual void ModelsUpdated() override;
    virtual void RigDone() override;
    virtual void NewActiveModel() override;
    virtual void comboBoxChanged(ComboBox *cb) override;
    
    void Post(bool error, String success, String fail);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIComponent)
    
    int activeAnimation = 0;
    Array<File> animationFiles;
    ComboBox models;
    TextButton loadExisting;
    TextButton loadOBJ;
    TextButton rig;
    TextButton openFolder;
    TextButton genFbx;
    Label outputLabel;
    AutoRig *autoRig;
    ComboBox animations;
    
    Image background;
    
    ScopedPointer<Window> glWindow;
    ScopedPointer<MyWindow> glComp;
    StaticDisplayMesh *m;
};


#endif  // UICOMPONENT_H_INCLUDED
