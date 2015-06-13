/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>
#include <QPainter>

#define HEIGHT 256
#define WIDTH 256

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setGeometry(0, 0, 900, 900);
    counter = 0;
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();

    // Clean up
    glDeleteFramebuffers(1, &fboA);
    glDeleteFramebuffers(1, &fboB);

    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
}

void MainWidget::timerEvent(QTimerEvent *)
{
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    // Enable depth buffer
    //glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);

    initShaders();
    initTextures();
    initFrameBuffers();

}


void MainWidget::resizeGL(int width, int height)
{
    glViewport(0, 0 , width, height);

    aspect = float(height) / float(width);

}

void MainWidget::paintGL()
{

    if (counter % 2 == 0)
    {
        //bind FBO A to set textureA as the output texture.
        glBindFramebuffer(GL_FRAMEBUFFER, fboA);
        //set the viewport to be the size of the texture
        glViewport(0,0, WIDTH, HEIGHT);

        //clear the ouput texture
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //bind our automata shader
        glUseProgram(program_automata.programId());

        //glActiveTexture(GL_TEXTURE0); //make texture register 0 active
        glBindTexture(GL_TEXTURE_2D, textureB); //bind textureB as out input texture

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(0); //unbind the shader

        glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind the FBO


    }  else
    {
        //... do the same as above except bind fboB and pass in textureA...
        //bind FBO B to set textureB as the output texture.
        glBindFramebuffer(GL_FRAMEBUFFER, fboB);
        //set the viewport to be the size of the texture
        glViewport(0,0, HEIGHT, WIDTH);

        //clear the ouput texture
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //bind our automata shader
        glUseProgram(program_automata.programId());

        //glActiveTexture(GL_TEXTURE0); //make texture register 1 active
        glBindTexture(GL_TEXTURE_2D, textureA); //bind textureA as out input texture

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(0); //unbind the shader

        glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind the FBO

    }

//    QPainter painter;
//    painter.begin(this);
//    painter.beginNativePainting();

    makeCurrent();
    glViewport(0, 0, this->width(), this->height());

    glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glUseProgram(program_display.programId());

    if (counter % 2 == 0)
    {

        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureA);
    }
    else
    {
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureB);

    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

//    painter.endNativePainting();
//    painter.end();

    //qDebug() << QString("Counter = %1").arg(counter);
    counter++;
}

void MainWidget::initTextures()
{

     static const GLint swizzles[] = { GL_RED, GL_RED, GL_RED, GL_ONE };

    glGenTextures(1, &textureA);
    glBindTexture(GL_TEXTURE_2D, textureA);
    glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, WIDTH, HEIGHT);

    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &textureB);
    glBindTexture(GL_TEXTURE_2D, textureB);
    glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, WIDTH, HEIGHT);

    unsigned char * datain = new unsigned char[WIDTH * HEIGHT];

    generate_texture(datain, WIDTH, HEIGHT);

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0, 0,
                    WIDTH, HEIGHT,
                    GL_RED, GL_UNSIGNED_BYTE,
                    datain);

    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void MainWidget::initFrameBuffers()
{
    //create fboA and attach texture A to it
    glGenFramebuffers(1, &fboA);
    glBindFramebuffer(GL_FRAMEBUFFER, fboA);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureA, 0);
    checkFramebufferStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &fboB);
    glBindFramebuffer(GL_FRAMEBUFFER, fboB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureB, 0);
    checkFramebufferStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void MainWidget::generate_texture(unsigned char * data, int width, int height)
 {

    unsigned char val;
    // Seed the random-number generator with the current time so that
    // the numbers will be different every time we run.
    srand( (unsigned)time( NULL ) );

    for (int i = 0; i < width * height; i+=1)
    {
        float random = (float)(rand()) / RAND_MAX;
        if (random > 0.7)
        {
            val = 0x00;
        } else
        {
            val = 0xFF;
        }
      data[i] = val;

    }
}

bool MainWidget::initShaders()
{

    if(!program_display.create())
        return false;

    // Compile vertex shader
    if (!program_display.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        return false;

    // Compile fragment shader
    if (!program_display.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        return false;

    // Link shader pipeline
    if (!program_display.link())
        return false;

    // Bind shader pipeline for use
    if (!program_display.bind())
        return false;

    if(!program_automata.create())
        return false;

    // Compile vertex shader
    if (!program_automata.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/automata.vsh"))
        return false;

    // Compile fragment shader
    if (!program_automata.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/automata.fsh"))
        return false;

    // Link shader pipeline
    if (!program_automata.link())
        return false;

    // Bind shader pipeline for use
    if (!program_automata.bind())
        return false;

    uniform_cell_width = glGetUniformLocation( program_automata.programId(), "du");
    if (uniform_cell_width >= 0 )
        glUniform1f(uniform_cell_width, 1.0/(float)WIDTH); //pass in the width of the cells
    else
        qDebug() << "Uniform du not found ";

    uniform_cell_height = glGetUniformLocation( program_automata.programId(), "dv");
    if (uniform_cell_height >= 0 )
        glUniform1f(uniform_cell_height, 1.0/(float)HEIGHT); //pass in the height of the cells
    else
        qDebug() << "Uniform dv not found ";


    return true;

}

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool MainWidget::checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        qDebug() << "Framebuffer complete.";
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        qDebug() << "[ERROR] Framebuffer incomplete: Attachment is NOT complete.";
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        qDebug() << "[ERROR] Framebuffer incomplete: No image is attached to FBO.";
        return false;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        qDebug() << "[ERROR] Framebuffer incomplete: Draw buffer.";
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        qDebug() << "[ERROR] Framebuffer incomplete: Read buffer.";
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        qDebug() << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation.";
        return false;

    default:
        qDebug() << "[ERROR] Framebuffer incomplete: Unknown error.";
        return false;
    }
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{

    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
     default:
        break;
     }
}
