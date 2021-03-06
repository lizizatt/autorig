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

#include "MyWindow.h"
#include "../Pinocchio/skeleton.h"
#include <OpenGL/GL.h>
#include "Main.h"

static HumanSkeleton human;

MyWindow *win = NULL;

Window::Window(MyWindow* window)
: DocumentWindow("RenderWindow", Colours::black, DocumentWindow::allButtons, true), window(window) {
    setContentNonOwned(window, true);
    setSize(window->w, window->h);
    setVisible(true);
    setEnabled(true);
    setUsingNativeTitleBar(true);
    toFront(false);
    setDraggable(true);
}

MyWindow::MyWindow() : flatShading(true), floor(true), skeleton(false)
{
    autoRig = AutoRigApplication::getAutoRig();
    
    setSize(w, h);
    resetTransform();

    openGLContext.setContinuousRepainting(false);
    startTimer(20);
}

void MyWindow::timerCallback()
{
    openGLContext.triggerRepaint();
}

void MyWindow::render() {
    
    int i;
    if (ready) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        double left = -.1;
        double bottom = -.1;
        double right = 1.1;
        double top = 1.1;
        if(w > 1 && h > 1) {
            if(w > h) {
                right = -0.1 + 1.2 * w / h;
            }
            if(h > w) {
                bottom = 1.1 - 1.2 * h / w;
            }
        }

        double scale = 1. / 1000.;
        left = -w * scale;
        right = w * scale;
        bottom = -h * scale;
        top = h * scale;
        glFrustum(left, right, bottom, top, 5., 30.);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Transform------
    Vector3 trans = transform.getTrans();
    glTranslated(trans[0], trans[1], -10 + trans[2]);
    
    double scale = transform.getScale();
    glScaled(scale, scale, scale);
    
    PinQuaternion::Quaternion<> r = transform.getRot();
    double ang = r.getAngle();
    if(fabs(ang) > 1e-6) {
        Vector3 ax = r.getAxis();
        glRotated(ang * 180. / M_PI, ax[0], ax[1], ax[2]);
    }

    //Draw----------
    if(floor)
        drawFloor();

    vector<const Mesh *> ms(meshes.size());
    for(i = 0; i < (int)meshes.size(); ++i) {
        ms[i] = &(meshes[i]->getMesh());
    }

    //shadows
    if(floor) {
        Vector3 lightRay = transform.getRot().inverse() * Vector3(1, 2, 2);
        if(lightRay[1] == 0)
            lightRay[1] = 1e-5;
        lightRay = -lightRay / lightRay[1];
            
        glDisable(GL_LIGHTING);
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        float matr[16] = {1,0,0,0, (float)lightRay[0],0,(float)lightRay[2],0, 0,0,1,0, 0,0.01f,0,1};
        glMultMatrixf(matr);
        glDepthMask(0);
        for(i = 0; i < (int)ms.size(); ++i)
            drawMesh(*(ms[i]), flatShading);                
        glDepthMask(1);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    static GLfloat colr[4] = {1.f, .9f, .75f, 1.0f };
    static GLfloat colrb[4] = {1.f, .9f, .75f, 1.0f };
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colr);
    glMaterialfv( GL_BACK, GL_AMBIENT_AND_DIFFUSE, colrb);

    //draw meshes
    for(i = 0; i < (int)meshes.size(); ++i) {
        drawMesh(*(ms[i]), flatShading, Vector3(), true);
    }

    //draw lines
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    for(i = 0; i < (int)lines.size(); ++i) {
        glColor3d(lines[i].color[0], lines[i].color[1], lines[i].color[2]);
        glLineWidth((float)lines[i].thickness);
        glBegin(GL_LINES);
        glVertex3d(lines[i].p1[0], lines[i].p1[1], lines[i].p1[2]);
        glVertex3d(lines[i].p2[0], lines[i].p2[1], lines[i].p2[2]);
        glEnd();
    }

    if(skeleton) {
        glLineWidth(5);
        for(i = 0; i < (int)meshes.size(); ++i) {
            vector<Vector3> v = meshes[i]->getSkel();
            if(v.size() == 0)
                continue;
            glColor3d(.5, 0, 0);

            const vector<int> &prev = human.fPrev();
            glBegin(GL_LINES);
            for(int j = 1; j < (int)prev.size(); ++j) {
                int k = prev[j];
                glVertex3d(v[j][0], v[j][1], v[j][2]);
                glVertex3d(v[k][0], v[k][1], v[k][2]);
            }
            glEnd();
        }
    }
    
    glSwapAPPLE();
}

void MyWindow::resetTransform()
{
    transform = Transform<>(PinQuaternion::Quaternion<>(Vector3(1, 0, 0), .2), 1.5, Vector3(-0.66, -0.66, 0));
}

void MyWindow::initialise()
{
    static GLfloat pos[4] = {5.0, 5.0, 10.0, 1.0 };

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glLightfv( GL_LIGHT0, GL_POSITION, pos );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE);
    glDisable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    
    ready = true;
}

void MyWindow::shutdown()
{
    
}

void MyWindow::drawMesh(const Mesh &m, bool flatShading, Vector3 trans, bool shadow)
{
    int i;
    Vector3 normal;
    
    bool color = false;
    if (color && shadow) {
        glDisable(GL_LIGHTING);
    }
    glBegin(GL_TRIANGLES);
    for(i = 0; i < (int)m.edges.size(); ++i) {
        int v = m.edges[i].vertex;
        const Vector3 &p = m.vertices[v].pos;

        if(!flatShading) {
            normal = m.vertices[v].normal;
            glNormal3d(normal[0], normal[1], normal[2]);
        }
        else if(i % 3 == 0) {
            const Vector3 &p2 = m.vertices[m.edges[i + 1].vertex].pos;
            const Vector3 &p3 = m.vertices[m.edges[i + 2].vertex].pos;
        
            normal = ((p2 - p) % (p3 - p)).normalize();
            glNormal3d(normal[0], normal[1], normal[2]);
        }
        
        //determine color
        if (color && shadow && m.vertices[v].color.size() > 0) {
            glColor4f(m.vertices[v].color[0], m.vertices[v].color[1], m.vertices[v].color[2], 1.0f);
        }
        
        glVertex3d(p[0] + trans[0], p[1] + trans[1], p[2] + trans[2]);
    }
    glEnd();
    if (color && shadow) {
        glEnable(GL_LIGHTING);
    }
}

void MyWindow::drawFloor()
{
    int i;
    Mesh floor;
    floor.vertices.resize(4);
    for(i = 0; i < 4; ++i) {
        floor.vertices[i].normal = Vector3(0, 1, 0);
        floor.vertices[i].pos = 10. * Vector3(((i + 0) % 4) / 2, 0, ((i + 1) % 4) / 2) - Vector3(4.5, 0, 4.5);
    }
    floor.edges.resize(6);
    for(i = 0; i < 6; ++i)
        floor.edges[i].vertex = (i % 3) + ((i > 3) ? 1 : 0);

    static GLfloat colrb[4] = {0.5f, .9f, .75f, 1.0f };
    static GLfloat colr[4] = {0.5f, .6f, .9f, 1.0f };
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colr);
    glMaterialfv( GL_BACK, GL_AMBIENT_AND_DIFFUSE, colrb);

    glShadeModel(GL_SMOOTH);
    drawMesh(floor, false);
    glShadeModel( flatShading ? GL_FLAT : GL_SMOOTH);

    glColor4d(.5, .6, .9, .3);
    glLineWidth(1.);
    int gridSize = 20;
    double y = floor.vertices[0].pos[1];
    double minX = floor.vertices[1].pos[0];
    double maxX = floor.vertices[2].pos[0];
    double minZ = floor.vertices[1].pos[2];
    double maxZ = floor.vertices[3].pos[2];
    double stepX = (maxX - minX) / double(gridSize);
    double stepZ = (maxZ - minZ) / double(gridSize);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    for(i = 0; i <= gridSize; ++i) {
        glVertex3d(minX + i * stepX, y, minZ);
        glVertex3d(minX + i * stepX, y, maxZ);
        glVertex3d(minX, y, minZ + i * stepZ);
        glVertex3d(maxX, y, minZ + i * stepZ);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
