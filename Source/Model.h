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

class Model
{
public:
    const static String modelDirectory;
    
public:
    File directory;
    File obj;
    File mtl;
    File jpg;
    Mesh m;
    int meshLabCleanTally = 0;
    String name;
    
    Model (File m);
    bool load();
};



#endif  // MODEL_H_INCLUDED
