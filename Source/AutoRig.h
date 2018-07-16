/*
  ==============================================================================

    AutoRig.h
    Created: 16 Jul 2018 2:37:52pm
    Author:  Liz Izatt

  ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef AUTORIG_H_INCLUDED
#define AUTORIG_H_INCLUDED

class AutoRig
{
private:
    const String MESHLAB_PATH = "/Applications/Meshlab.app";
    const String MESHLABSERVER_PATH = "/Applications/Meshlab.app/Contents/MacOS/meshlabserver";

    
    
public:
    AutoRig();
    ~AutoRig();
    
    void Clear();
    bool LoadOBJ(File file);
    bool runMeshlabCleanup();
    
};


#endif  // AUTORIG_H_INCLUDED
