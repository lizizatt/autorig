/*
  ==============================================================================

    Model.cpp
    Created: 16 Jul 2018 3:59:26pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "Model.h"

const String Model::modelDirectory = "~/AutoRig/Models/";

Model::Model(File m)
{
    if (!m.existsAsFile() || m.getFileExtension() != ".obj") {
        return;
    }
    
    name = m.getFileNameWithoutExtension();
    directory = File(modelDirectory + name);
    if (!File(directory).exists()) {
        directory.createDirectory();
    }
    
    obj = directory.getChildFile(m.getFileName());
    mtl = directory.getChildFile(name + ".mtl");
    jpg = directory.getChildFile(name + ".jpg");
    
    if (m.getFullPathName() != obj.getFullPathName()) {
        m.copyFileTo(obj);
        File mtlOrig = m.getSiblingFile(mtl.getFileName());
        mtlOrig.copyFileTo(mtl);
        File jpgOrig = m.getSiblingFile(jpg.getFileName());
        mtlOrig.copyFileTo(jpg);
        cout << "Loaded new model " << name << "\n";
    }
    else {
        cout << "Loaded existing model " << name << "\n";
    }
}
