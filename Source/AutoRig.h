/*
  ==============================================================================

    AutoRig.h
    Created: 16 Jul 2018 2:37:52pm
    Author:  Liz Izatt

  ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "Model.h"

#ifndef AUTORIG_H_INCLUDED
#define AUTORIG_H_INCLUDED

class AutoRigListener {
public:
    virtual void ModelsUpdated() {}
};

class AutoRig : public Thread
{
public:
    const String MESHLABSERVER_PATH = "/Applications/Meshlab.app/Contents/MacOS/meshlabserver";

    OwnedArray<Model> models;
    bool runMeshlabCleanup = false;
    
    Model* activeModel = nullptr;
    
public:
    AutoRig();
    ~AutoRig();
    
    void Clear();
    bool LoadOBJ(File file);
    void SetActive(int index);
    bool RunMeshlabCleanup();
    
    void AddListener(AutoRigListener *l);
    void RemoveListener(AutoRigListener *l);
    
    void run();
    
private:
    void meshlabCleanup();
    Array<AutoRigListener*> listeners;
};


#endif  // AUTORIG_H_INCLUDED
