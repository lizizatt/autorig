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
    mtl = directory.getChildFile(name + ".mtl");
    jpg = directory.getChildFile(name + ".jpg");
    
    if (!obj_poisson.existsAsFile()) {
        m.copyFileTo(obj);
        File mtlOrig = m.getSiblingFile(mtl.getFileName());
        mtlOrig.copyFileTo(mtl);
        File jpgOrig = m.getSiblingFile(jpg.getFileName());
        jpgOrig.copyFileTo(jpg);
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
    
    return ret1 == 0;
}

bool Model::rig()
{
    if (rigged) {
        return true;
    }
    
    //poisson mesh pinocchio output
    pOut = autorig(*skeleton, *mesh_poisson);
    //****************
  
    return genFBX();
}

bool Model::genFBX()
{
    
    //assemble fbx
    FbxScene* lScene = FbxScene::Create(fbxManager, "AutorigOutput");
    FbxNode* lNode = FbxNode::Create(lScene, "node");
    FbxMesh* lMesh = FbxMesh::Create(lScene, "mesh");
    
    lNode->SetNodeAttribute(lMesh);
    CreateTexture(lScene, lMesh);
    
    // Add the mesh node to the root node in the scene.
    FbxNode *rootNode = lScene->GetRootNode();
    rootNode->AddChild(lNode);
    
    //set up ways to access arrays
    Array<WavefrontObjFileWithVertexColors::Vertex>& verts = mesh->shapes[0]->mesh.vertices;
    int numV = verts.size();
    Array<WavefrontObjFileWithVertexColors::Vertex>& normals = mesh->shapes[0]->mesh.normals;
    int numN = verts.size();
    Array<WavefrontObjFileWithVertexColors::TextureCoord>& textureCoords = mesh->shapes[0]->mesh.textureCoords;
    int numUV = verts.size();
    Array<WavefrontObjFileWithVertexColors::Index>& indicies = mesh->shapes[0]->mesh.indices;
    int numI = verts.size();
    
    //add vertexes
    lMesh->InitControlPoints(numV);
    FbxVector4* controlPoints = lMesh->GetControlPoints();
    for (int i = 0; i < numV; i++) {
        controlPoints[i] = FbxVector4(verts[i].x, verts[i].y, verts[i].z, 1.0f);
    }
    
    //normals
    FbxLayer *lLayer = lMesh->GetLayer(0);
    if(lLayer == NULL) {
        lMesh->CreateLayer();
        lLayer = lMesh->GetLayer(0);
    }
    FbxLayerElementNormal* lLayerElementNormal= FbxLayerElementNormal::Create(lMesh, "");
    lLayerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);
    lLayerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);
    for (int i = 0; i < numN; i++) {
        lLayerElementNormal->GetDirectArray().Add(FbxVector4(normals[i].x, normals[i].y, normals[i].z, 1.0f));
    }
    lLayer->SetNormals(lLayerElementNormal);
    
    //texture
    FbxLayerElementUV *uvDiffuseLayerElement = FbxLayerElementUV::Create( lMesh, "diffuseUV");
    uvDiffuseLayerElement->SetMappingMode( FbxLayerElement::eByControlPoint );
    uvDiffuseLayerElement->SetReferenceMode( FbxLayerElement::eDirect );
    lMesh->InitTextureUV(numUV);
    for (int i = 0; i < numUV; i++) {
        lMesh->AddTextureUV(FbxVector2(textureCoords[i].x, textureCoords[i].y));
        uvDiffuseLayerElement->GetDirectArray().Add(FbxVector2(textureCoords[i].x, textureCoords[i].y));
    }
    lLayer->SetUVs( uvDiffuseLayerElement, FbxLayerElement::eTextureDiffuse );

    
    lNode->SetShadingMode(FbxNode::eTextureShading);
    
    //indices/polygons
    for (int i = 0; i < indicies.size(); i+=3) {
        lMesh->BeginPolygon();
        lMesh->AddPolygon(indicies[i]);
        lMesh->AddPolygon(indicies[i+1]);
        lMesh->AddPolygon(indicies[i+2]);
        lMesh->EndPolygon();
    }
    
    //****************
    //export
    File fbxOutFile = obj.getSiblingFile(name + ".fbx");
    if (fbxOutFile.existsAsFile()) {
        fbxOutFile.deleteFile();
    }
    
    FbxIOSettings * ios = FbxIOSettings::Create(fbxManager, IOSROOT );
    ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
    
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

void Model::CreateTexture(FbxScene* pScene, FbxMesh* pMesh)
{
    FbxSurfacePhong* lMaterial = NULL;
    
    FbxNode* lNode = pMesh->GetNode();
    if(lNode)
    {
        lMaterial = lNode->GetSrcObject<FbxSurfacePhong>(0);
        if (lMaterial == NULL)
        {
            FbxString lMaterialName = "toto";
            FbxString lShadingName  = "Phong";
            FbxDouble3 lBlack(0.0, 0.0, 0.0);
            FbxDouble3 lWhite(1.0, 1.0, 1.0);
            lMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());
            
            // Generate primary and secondary colors.
            lMaterial->Emissive           .Set(lBlack);
            lMaterial->Ambient            .Set(lWhite);
            lMaterial->AmbientFactor      .Set(1.);
            // Add texture for diffuse channel
            lMaterial->Diffuse           .Set(lWhite);
            lMaterial->DiffuseFactor     .Set(1.);
            lMaterial->TransparencyFactor.Set(0.4);
            lMaterial->ShadingModel      .Set(lShadingName);
            lMaterial->Shininess         .Set(0.5);
            lMaterial->Specular          .Set(lBlack);
            lMaterial->SpecularFactor    .Set(0.3);
            
            lNode->AddMaterial(lMaterial);
        }
    }
    
    FbxFileTexture* lTexture = FbxFileTexture::Create(pScene,"Diffuse Texture");
    
    // Set texture properties.
    lTexture->SetFileName(jpg.getFullPathName().toRawUTF8()); // Resource file is in current directory.
    lTexture->SetTextureUse(FbxTexture::eStandard);
    lTexture->SetMappingType(FbxTexture::eUV);
    lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    lTexture->SetSwapUV(false);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);
    
    
    if (lMaterial)
        lMaterial->Diffuse.ConnectSrcObject(lTexture);
}
