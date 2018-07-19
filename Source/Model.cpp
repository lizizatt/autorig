/*
  ==============================================================================

    Model.cpp
    Created: 16 Jul 2018 3:59:26pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "Model.h"
#include "Main.h"

const String Model::modelDirectory = "~/AutoRig/Models/";

Model::Model(File m)
{
    if (!m.existsAsFile() || m.getFileExtension() != ".obj") {
        return;
    }
    
    fbxManager = AutoRigApplication::getAutoRig()->fbxManager;
    
    name = m.getFileNameWithoutExtension();
    directory = File(modelDirectory + name);
    if (!File(directory).exists()) {
        directory.createDirectory();
    }
    
    obj = directory.getChildFile(m.getFileName());
    obj_poisson = directory.getChildFile(m.getFileNameWithoutExtension() + "_poisson" + m.getFileExtension());
    obj_cleaned = directory.getChildFile(m.getFileNameWithoutExtension() + "_cleaned" + m.getFileExtension());
    mtl = directory.getChildFile(name + ".mtl");
    jpg = directory.getChildFile(name + ".jpg");
    
    if (!obj_poisson.existsAsFile()) {
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
    mesh_poisson = new Mesh(obj_poisson.getFullPathName().toRawUTF8());
    mesh = new WavefrontObjFileWithVertexColors();
    mesh->load(obj);
    skeleton = new HumanSkeleton();
    return mesh->shapes.size() > 0 && mesh_poisson->vertices.size() > 0;
}


bool Model::runMeshlab()
{
    File filterScript = File::getSpecialLocation(File::currentApplicationFile).getChildFile("contents").getChildFile("Resources").getChildFile("poissonreconstruct.mlx");
    String meshlabCommand = MESHLABSERVER_PATH + " -i " + obj.getFullPathName() + " -o " + obj_poisson.getFullPathName() + " -s " + filterScript.getFullPathName();
    
    int ret1 = system(meshlabCommand.toRawUTF8());
    cout << "Ran meshlab poisson generation with return value " << ret1 << "\n";
    
    filterScript = File::getSpecialLocation(File::currentApplicationFile).getChildFile("contents").getChildFile("Resources").getChildFile("clean.mlx");
    meshlabCommand = MESHLABSERVER_PATH + " -i " + obj.getFullPathName() + " -o " + obj_cleaned.getFullPathName() + " -s " + filterScript.getFullPathName();
    
    int ret2 = system(meshlabCommand.toRawUTF8());
    cout << "Ran meshlab clean with return value " << ret2 << "\n";
    
    return ret1 == 0 && ret2 == 0;
}

bool Model::rig()
{
    if (rigged) {
        return true;
    }
    
    //poisson mesh pinocchio output
    PinocchioOutput pout = autorig(*skeleton, *mesh_poisson);
    
    //make fbx file
    FbxScene* lScene = FbxScene::Create(fbxManager, "AutorigOutput");
    FbxNode* lNode = FbxNode::Create(lScene, "node");
    FbxMesh* lMesh = FbxMesh::Create(lScene, "mesh");
    
    //export
    File fbxOutFile = obj.getSiblingFile(name + ".fbx");
    
    FbxIOSettings * ios = FbxIOSettings::Create(fbxManager, IOSROOT );
    fbxManager->SetIOSettings(ios);
    
    FbxExporter* lExporter = FbxExporter::Create(fbxManager, "");
    
    bool res = lExporter->Initialize(fbxOutFile.getFullPathName().toRawUTF8(), -1, fbxManager->GetIOSettings());
    if (!res) {
        cout << "Failed to initialize exporter for " + name + "\n";
    }
    else {
        res = lExporter->Export(lScene);
        if (res) {
            cout << "Successfully exported " + name + ".fbx";
        }
        else {
            cout << "Failed to export " + name + ".fbx";
        }
    }
    
    return true;
}
