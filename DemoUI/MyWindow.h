/*
Copyright (c) 2007 Ilya Baran

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "../JuceLibraryCode/JuceHeader.h"

#include "../Pinocchio/mesh.h"
#include "../Pinocchio/transform.h"
#include "DisplayMesh.h"
#include "AutoRig.h"


using namespace PinQuaternion;

struct LineSegment
{
    LineSegment() : thickness(1.) {}
    LineSegment(const Vector3 &inP1, const Vector3 &inP2,
        const Vector3 &inColor = Vector3(1, 1, 1), double inThickness = 1.)
        : p1(inP1), p2(inP2), color(inColor), thickness(inThickness) {}

    Vector3 p1, p2, color;
    double thickness;
};

class MyWindow : public OpenGLAppComponent, public Timer
{
public:
    int w = 800;
    int h = 600;
    
public:
    MyWindow();

    virtual ~MyWindow() {}
    virtual void render() override;
    
    virtual void paint(Graphics g) {}
    virtual void resized() {}
    
    virtual void initialise() override;
    virtual void shutdown() override;
    
    void timerCallback() override;

    void addMesh(DisplayMesh *inM) { meshes.push_back(inM); }
    void addLine(const LineSegment &l) { lines.push_back(l); }
    void clearLines() { lines.clear(); }
    void clearMeshes() {meshes.clear();}
    
private:
    AutoRig *autoRig;
    bool flatShading = false;
    bool floor = true;
    bool skeleton = true;
    Transform<> transform;
    vector<DisplayMesh *> meshes;
    vector<LineSegment> lines;

    bool ready = false;
    
    void resetTransform();
    void initGL();
    void drawMesh(const Mesh &m, bool flatShading, Vector3 trans = Vector3(), bool shadow = false);
    void drawFloor();
};

class Window : public DocumentWindow
{
public:
    Window(MyWindow* window);
    MyWindow* window;
    
    virtual void paint(Graphics g) {}
    virtual void resized() {}
};

#endif //MYWINDOW_H
