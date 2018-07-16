#include "Main.h"

Component* createMainContentComponent();

void AutoRigApplication::initialise (const String& commandLine)
{
    autoRig = new AutoRig();
    mainWindow = new MainWindow (getApplicationName());
}

void AutoRigApplication::shutdown()
{
    mainWindow = nullptr; // (deletes our window)
}
void AutoRigApplication::systemRequestedQuit()
{
    quit();
}

void AutoRigApplication::anotherInstanceStarted (const String& commandLine)
{
}

AutoRigApplication::MainWindow::MainWindow (String name)  : DocumentWindow (name,
                                            Colours::lightgrey,
                                            DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setContentOwned (createMainContentComponent(), true);
    setResizable (true, true);
    
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

void AutoRigApplication::MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

START_JUCE_APPLICATION (AutoRigApplication)
