/*
  ==============================================================================

    Main.h
    Created: 16 Jul 2018 2:59:41pm
    Author:  Liz Izatt

  ==============================================================================
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AutoRig.h"


//==============================================================================
class AutoRigApplication  : public JUCEApplication
{
public:
    AutoRigApplication() {}
    
    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; } //
    
    
    static AutoRig* getAutoRig() {return ((AutoRigApplication*) AutoRigApplication::getInstance())->autoRig;}
    
    
    void initialise (const String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    
    void anotherInstanceStarted (const String& commandLine) override;
    
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name);
        
        void closeButtonPressed() override;
        
    private:
    };
    
    
private:
    ScopedPointer<AutoRig> autoRig;
    ScopedPointer<MainWindow> mainWindow;

    
};

#endif  // MAIN_H_INCLUDED
