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
    virtual void RigDone() {}
    virtual void NewActiveModel() {}
};

class AutoRig : public Thread
{
public:

    OwnedArray<Model> models;
    bool startRig = false;
    bool genFbx = false;
    
    Model* activeModel = nullptr;
    
    FbxManager* fbxManager;
    
public:
    AutoRig();
    ~AutoRig();
    
    void Clear();
    bool LoadOBJ(File file);
    void SetActive(int index);
    void StartRig();
    void GenFBX();
    
    void AddListener(AutoRigListener *l);
    void RemoveListener(AutoRigListener *l);
    
    void run();
    
private:
    Array<AutoRigListener*> listeners;
};


#endif  // AUTORIG_H_INCLUDED
