/*
  ==============================================================================

    GraphicsComponent.h
    Created: 16 Jul 2018 1:34:44pm
    Author:  Liz Izatt

  ==============================================================================
*/

#ifndef GRAPHICSCOMPONENT_H_INCLUDED
#define GRAPHICSCOMPONENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "Autorig.h"

//==============================================================================
/*
 */
class GraphicsComponent    : public Component, public OpenGLRenderer, public AutoRigListener, public Timer
{
public:
    GraphicsComponent();
    ~GraphicsComponent();
    
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override;
    virtual void openGLContextClosing() override;
    
    
    void paint (Graphics&) override;
    void resized() override;
    void RigDone() override;
    void NewActiveModel() override;
    
    void timerCallback() override;
    
public:
    OpenGLContext context;
#include "WavefrontObjParser.h"
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicsComponent)
    
    void initShaders();
    void bindBuffers();
    
    AutoRig* autoRig;
    
    ScopedPointer<WavefrontObjFileWithVertexColors> obj;
    
    Array<WavefrontObjFileWithVertexColors::Vertex> *vArray;
    Array<WavefrontObjFileWithVertexColors::Color> *cArray;
    Array<WavefrontObjFileWithVertexColors::Vertex> *nArray;
    Array<WavefrontObjFileWithVertexColors::Index> *iArray;
    
    bool ready = false;
    
    GLint program;
    GLint attribute_pos;
    GLint attribute_norm;
    GLint attribute_color;
    GLint attribute_viewDir;
    GLint uniform_mvp;
    
    //vbo params
    GLuint texture_id;
    GLuint vbo_obj_vertices;
    GLuint vbo_obj_colors;
    GLuint vbo_obj_normals;
    GLuint ibo_obj_elements;
    
    float scale = 1.0f;
    Matrix3D<float> model, view, projection, mvp;
    float rotationAboutX;
    float rotationAboutY;
    GLenum err;
};




#endif  // GRAPHICSCOMPONENT_H_INCLUDED
