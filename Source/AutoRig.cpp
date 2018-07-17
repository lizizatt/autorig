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
    if (m->obj.existsAsFile() && m->mtl.existsAsFile() && m->jpg.existsAsFile()) {
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
}

bool AutoRig::RunMeshlabCleanup()
{
    runMeshlabCleanup = true;
    return true;
}

void AutoRig::meshlabCleanup()
{
    if (activeModel == nullptr) {
        return;
    }
    
    File filterScript = File::getSpecialLocation(File::currentApplicationFile).getChildFile("contents").getChildFile("Resources").getChildFile("trymakemanifold.mlx");
    String meshlabCommand = MESHLABSERVER_PATH + " -i " + activeModel->obj.getFullPathName() + " -o " + activeModel->obj.getFullPathName() + " -s " + filterScript.getFullPathName();
    
    int ret = system(meshlabCommand.toRawUTF8());
    cout << "Ran meshlab cleanup with return value " << ret << "\n";
}

void AutoRig::run()
{
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
        
        if (runMeshlabCleanup) {
            meshlabCleanup();
            runMeshlabCleanup = false;
            activeModel->meshLabCleanTally++;
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
