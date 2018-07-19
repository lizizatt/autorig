/*
  ==============================================================================

    Model.h
    Created: 16 Jul 2018 3:59:26pm
    Author:  Liz Izatt

  ==============================================================================
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Pinocchio/mesh.h"
#include "../Pinocchio/pinocchioApi.h"
#include <fbxsdk.h>
#include "WavefrontObjParser.h"

class Model
{
public:
    const static String modelDirectory;
    const String MESHLABSERVER_PATH = "/Applications/Meshlab.app/Contents/MacOS/meshlabserver";
    
public:
    File directory;
    File obj;
    File obj_poisson;
    File mtl;
    File jpg;
    ScopedPointer<WavefrontObjFileWithVertexColors> mesh;
    ScopedPointer<Mesh> mesh_poisson;
    ScopedPointer<Skeleton> skeleton;
    String name;
    bool rigged = false;
    PinocchioOutput pOut;
    
    FbxManager *fbxManager;
    
    Model (File m);
    bool load();
    bool runMeshlab();
    bool rig();
    bool genFBX();
    void CreateTexture(FbxScene* pScene, FbxMesh* pMesh);

};



#endif  // MODEL_H_INCLUDED
