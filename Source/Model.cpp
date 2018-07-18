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
    }
    else {
        cout << "Do not need to run meshlab on model " << name << "\n";
        return;
    }
    
    bool res = runMeshlab();
    if (res) {
        cout << "Ran meshlab on model " << name << "\n";
    }
    else {
        cout << "Failed to run meshlab on model" << name << "\n";
    }
    
}

bool Model::load()
{
    mesh = new Mesh(obj.getFullPathName().toRawUTF8());
    skeleton = new HumanSkeleton();
    return mesh->vertices.size() > 0;
}


bool Model::runMeshlab()
{
    File filterScript = File::getSpecialLocation(File::currentApplicationFile).getChildFile("contents").getChildFile("Resources").getChildFile("poissonreconstruct.mlx");
    String meshlabCommand = MESHLABSERVER_PATH + " -i " + obj.getFullPathName() + " -o " + obj.getFullPathName() + " -s " + filterScript.getFullPathName();
    
    int ret = system(meshlabCommand.toRawUTF8());
    cout << "Ran meshlab cleanup with return value " << ret << "\n";
    
    return ret == 0;
}

bool Model::rig()
{
    if (rigged) {
        return true;
    }
    
    autorig(*skeleton, *mesh);
    return true;
}
