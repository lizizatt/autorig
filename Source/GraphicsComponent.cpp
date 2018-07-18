/*
  ==============================================================================

    GraphicsComponent.cpp
    Created: 16 Jul 2018 1:34:44pm
    Author:  Liz Izatt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphicsComponent.h"
#include "Main.h"


//==============================================================================
GraphicsComponent::GraphicsComponent()
{
    autoRig = AutoRigApplication::getAutoRig();
    autoRig->AddListener(this);
    
    setOpaque (true);
    context.setRenderer (this);
    context.attachTo (*this);
    context.setContinuousRepainting (false);
    
    startTimer(20);
}

GraphicsComponent::~GraphicsComponent()
{
    context.detach();
}

void GraphicsComponent::newOpenGLContextCreated()
{
    initShaders();
    
}

void GraphicsComponent::timerCallback()
{
    context.triggerRepaint();
}

void GraphicsComponent::bindBuffers()
{
    //neo obj buffers
    //verts
    glGenBuffers(1, &vbo_obj_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_vertices);
    glBufferData(GL_ARRAY_BUFFER, vArray->size() * sizeof(WavefrontObjFileWithVertexColors::Vertex), vArray->getRawDataPointer(), GL_STATIC_DRAW);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    glGenBuffers(1, &vbo_obj_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_colors);
    glBufferData(GL_ARRAY_BUFFER, cArray->size() * sizeof(WavefrontObjFileWithVertexColors::Color), cArray->getRawDataPointer(), GL_STATIC_DRAW);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    //norms
    glGenBuffers(1, &vbo_obj_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_normals);
    glBufferData(GL_ARRAY_BUFFER, nArray->size() * sizeof(WavefrontObjFileWithVertexColors::Vertex), nArray->getRawDataPointer(), GL_STATIC_DRAW);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    //indices
    glGenBuffers(1, &ibo_obj_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iArray->size() * sizeof(WavefrontObjFileWithVertexColors::Index), iArray->getRawDataPointer(), GL_STATIC_DRAW);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    ready = true;
}

void GraphicsComponent::renderOpenGL()
{
    if (!ready) {
        return;
    }
    
    glUseProgram(program);
    
    //mvp matrix
    model = Matrix3D<float>(    1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1);
    
    view = Matrix3D<float>(     scale,0,0,0,
                           0,scale,0,0,
                           0,0,scale,0,
                           0,0,0,1);
    
    view = view * view.rotated(Vector3D<float>(rotationAboutX * 3.1415 / 180.0,0,0));
    view = view * view.rotated(Vector3D<float>(0, rotationAboutY * 3.1415 / 180.0, 0));
    view = Matrix3D<float>(Vector3D<float>(0,0,-250)) * view;
    
    projection  = Matrix3D<float>::fromFrustum(-5, 5, -5, 5, 10, 500);
    
    mvp = projection * view * model;
    
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, mvp.mat);
    
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnableVertexAttribArray(attribute_pos);
    //glEnableVertexAttribArray(attribute_color);
    glEnableVertexAttribArray(attribute_norm);
    
    //figure out viewing direction
    Matrix3D<float> modelview = model * view;
    glVertexAttrib3f(attribute_viewDir, modelview.mat[2], modelview.mat[6], modelview.mat[10]);
    
    //vertex
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_vertices);
    glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    //color
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_colors);
    glVertexAttribPointer(attribute_color, 3, GL_FLOAT, GL_FALSE, 0, 0);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    //normal
    glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_normals);
    glVertexAttribPointer(attribute_norm, 3, GL_FLOAT, GL_FALSE, 0, 0);
    err = glGetError();
    assert(err == GL_NO_ERROR);
    
    //index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
    err = glGetError( );
    assert(err == GL_NO_ERROR);
    
    //draw neo base
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, iArray->size(), GL_UNSIGNED_INT, 0);
}

void GraphicsComponent::openGLContextClosing()
{
    glDeleteProgram(program);
}

void GraphicsComponent::paint (Graphics& g)
{
}

void GraphicsComponent::resized()
{
}

void GraphicsComponent::RigDone()
{
    obj = new WavefrontObjFileWithVertexColors();
    obj->load(autoRig->activeModel->obj);
    
    vArray = &obj->shapes[0]->mesh.vertices;
    nArray = &obj->shapes[0]->mesh.normals;
    iArray = &obj->shapes[0]->mesh.indices;
    
    bindBuffers();
}

void GraphicsComponent::NewActiveModel()
{
    obj = new WavefrontObjFileWithVertexColors();
    obj->load(autoRig->activeModel->obj);
    
    vArray = &obj->shapes[0]->mesh.vertices;
    nArray = &obj->shapes[0]->mesh.normals;
    iArray = &obj->shapes[0]->mesh.indices;
    obj->shapes[0]->mesh.colors = Array<WavefrontObjFileWithVertexColors::Color>();
    obj->shapes[0]->mesh.colors.resize(vArray->size());;
    cArray = &obj->shapes[0]->mesh.colors;
    
    bindBuffers();
}

void GraphicsComponent::initShaders()
{
    //grab shaders from files
    String vertexshader = String::createStringFromData(BinaryData::vertexshader, BinaryData::vertexshaderSize);
    String fragmentshader = String::createStringFromData(BinaryData::fragmentshader, BinaryData::fragmentshaderSize);
    const char *vs_source = vertexshader.toRawUTF8();
    const char *fs_source = fragmentshader.toRawUTF8();
    //DBG("Vertex Shader Contents:  \n******\n" + vs_source + "\n******\n");
    //DBG("Fragment Shader Contents:  \n******\n" + fs_source + "\n******\n");
    
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    
    //vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    assert(compile_ok);
    
    //fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    assert(compile_ok);
    
    //program
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    assert(link_ok);
    
    attribute_pos = glGetAttribLocation(program, "pos");
    assert(attribute_pos != -1);
    
    attribute_norm = glGetAttribLocation(program, "norm");
    assert(attribute_norm != -1);
    
    attribute_color = glGetAttribLocation(program, "color");
    assert(attribute_color != -1);
    
    attribute_viewDir = glGetAttribLocation(program, "viewDir");
    assert(attribute_viewDir != -1);
    
    uniform_mvp = glGetUniformLocation(program, "m_mvp");
    assert(uniform_mvp != -1);
}
