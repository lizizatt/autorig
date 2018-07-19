/*
  ==============================================================================

    AutoRig.cpp
    Created: 16 Jul 2018 2:37:52pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "AutoRig.h"

AutoRig::AutoRig()
: Thread("autorig")
{
    startThread();
}

AutoRig::~AutoRig()
{
    stopThread(1000);
}

void AutoRig::Clear()
{
    activeModel = nullptr;
}

bool AutoRig::LoadOBJ(File file)
{
    Model *m = new Model(file);
    if (m->obj.existsAsFile()) {
        models.add(m);
        activeModel = m;
        return true;
    }
    delete m;
    return false;
}

void AutoRig::SetActive(int index)
{
    activeModel = models[index];
    activeModel->load();
    for (int i = 0; i < listeners.size(); i++) {
        listeners[i]->NewActiveModel();
    }
}

void AutoRig::StartRig()
{
    if (activeModel != nullptr) {
        startRig = true;
    }
}

void AutoRig::GenFBX()
{
    if (activeModel != nullptr) {
        genFbx = true;
    }
}

void AutoRig::run()
{
    fbxManager = FbxManager::Create();
    
    Array<File> files;
    File(Model::modelDirectory).findChildFiles(files, File::findDirectories, false);
    for (int i = 0; i < files.size(); i++) {
        LoadOBJ(files[i].getChildFile(files[i].getFileNameWithoutExtension() + ".obj"));
    }
    cout << "AutoRig loaded, found " << files.size() << " existing files.\n";
    
    for (int i = 0; i < listeners.size(); i++) {
        listeners[i]->ModelsUpdated();
    }
    
    while (!threadShouldExit()) {
        if (startRig) {
            //start rigging!
            startRig = false;
            activeModel->rig();
            for (int i = 0; i < listeners.size(); i++) {
                listeners[i]->RigDone();
            }
        }
        if (genFbx) {
            genFbx = false;
            activeModel->genFBX();
        }
        sleep(10);
    }
}

void AutoRig::AddListener(AutoRigListener *l)
{
    listeners.add(l);
}

void AutoRig::RemoveListener(AutoRigListener *l)
{
    listeners.removeAllInstancesOf(l);
}
