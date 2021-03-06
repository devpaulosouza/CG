﻿#include "glwidget.h"
#include <fstream>
#include <QFileDialog>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <QtGlobal>
#include <stdlib.h>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    vertices = NULL ;
    normals = NULL ;
    texCoords = NULL ;
    tangents = NULL ;
    indices = NULL ;

    vboVertices = NULL ;
    vboNormals = NULL ;
    vboTexCoords = NULL ;
    vboTangents = NULL ;
    vboIndices = NULL ;

    shaderProgram = NULL ;
    vertexShader = NULL ;
    fragmentShader = NULL ;
    currentShader = 0;

    zoom = 0.0;

    fpsCounter = 0;
}

GLWidget ::~ GLWidget ()
{
    destroyVBOs () ;
    destroyShaders () ;
}

void GLWidget :: initializeGL ()
{
}

void GLWidget :: resizeGL ( int width , int height )
{
}

void GLWidget :: paintGL ()
{
    glClear ( GL_COLOR_BUFFER_BIT );
}

void GLWidget :: toggleBackgroundColor ( bool toBlack )
{
    if ( toBlack )
        glClearColor (0 , 0 , 0 , 1) ;
    else
        glClearColor (1 , 1 , 1 , 1) ;

    updateGL();
}


void GLWidget :: showFileOpenDialog ()
{
    QByteArray fileFormat = "off";
    QString fileName ;
    fileName = QFileDialog :: getOpenFileName (
                this ,
                " Open File " ,
                QDir :: homePath () ,
                QString ( " %1 Files (*.%2) " )
                . arg ( QString ( fileFormat.toUpper() ) )
                . arg ( QString ( fileFormat ))
    );

    if (! fileName . isEmpty () ) {
        readOFFFile ( fileName );

        qDebug("gen normals");
        genNormals () ;

        qDebug("gen tex coords");
        genTexCoordsCylinder () ;

        qDebug("gen tangents");
        genTangents () ;

        qDebug("create vbos");
        createVBOs () ;

        qDebug("create shaders");
        createShaders () ;

        qDebug("update");
        updateGL();
    }
}


void GLWidget :: readOFFFile ( const QString & fileName )
{
    std::ifstream stream ;
    stream . open ( fileName.toUtf8(), std::ifstream::in );

    if (! stream . is_open () ) {
        qWarning (" Cannot open file . ");
        return ;
    }

    std::string line ;

    stream >> line ;
    stream >> numVertices >> numFaces >> line ;

    delete [] vertices ;
    vertices = new QVector4D [ numVertices];

    delete [] indices ;

    indices = new unsigned int [ numFaces * 3];

    if ( numVertices > 0) {
        double minLim = std :: numeric_limits < double >:: min () ;
        double maxLim = std :: numeric_limits < double >:: max () ;
        QVector4D max ( minLim , minLim , minLim , 1.0) ;
        QVector4D min ( maxLim , maxLim , maxLim , 1.0) ;

        for ( unsigned int i = 0; i< numVertices ; i ++) {
            float x , y , z;
            stream >> x >> y >> z;
            max . setX ( qMax ( max .x () , x));
            max . setY ( qMax ( max .y () , y));
            max . setZ ( qMax ( max .z () , z));
            min . setX ( qMin ( min .x () , x));
            min . setY ( qMin ( min .y () , y));
            min . setZ ( qMin ( min .z () , z));

            vertices [i] = QVector4D (x , y , z , 1.0) ;

        }

        QVector4D midpoint = ( min + max ) * 0.5;
        double invdiag = 1 / ( max - min ) . length () ;

        for ( unsigned int i = 0; i < numVertices ; i ++) {
            vertices [i] = ( vertices [i] - midpoint )* invdiag ;
            vertices [i ]. setW (1) ;
        }
    }

    for ( unsigned int i = 0; i < numFaces ; i ++) {
        unsigned int a , b , c;
        stream >> line >> a >> b >> c ;
        indices [ i * 3] = a;
        indices [ i * 3 + 1] = b ;
        indices [ i * 3 + 2] = c ;
    }

    stream.close();
}

void GLWidget :: createVBOs ()
{
    destroyVBOs () ;

    vboVertices = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboVertices -> create () ;
    vboVertices -> bind () ;
    vboVertices -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboVertices -> allocate ( vertices , numVertices *
    sizeof ( QVector4D )) ;

    delete [] vertices ;
    vertices = NULL ;

    vboNormals = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboNormals -> create () ;
    vboNormals -> bind () ;
    vboNormals -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboNormals -> allocate ( normals , numVertices *sizeof ( QVector3D ) );

    delete [] normals ;
    normals = NULL ;

    vboTexCoords = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboTexCoords -> create () ;
    vboTexCoords -> bind () ;
    vboTexCoords -> setUsagePattern ( QGLBuffer :: StaticDraw ) ;
    vboTexCoords -> allocate ( texCoords , numVertices *sizeof ( QVector2D ));

    delete [] texCoords ;
    texCoords = NULL ;

    vboTangents = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboTangents -> create () ;
    vboTangents -> bind () ;
    vboTangents -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboTangents -> allocate ( tangents , numVertices * sizeof ( QVector4D )) ;

    delete [] tangents ;
    tangents = NULL ;

    vboIndices = new QGLBuffer ( QGLBuffer :: IndexBuffer );
    vboIndices -> create () ;
    vboIndices -> bind () ;
    vboIndices -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboIndices -> allocate ( indices , numFaces * 3 *sizeof ( unsigned int ));

    delete [] indices ;
    indices = NULL ;

}

void GLWidget :: destroyVBOs ()
{
    if ( vboVertices ) {
        vboVertices -> release () ;
        delete vboVertices ;
        vboVertices = NULL ;
    }

    if ( vboNormals ) {
        vboNormals -> release () ;
        delete vboNormals ;
        vboNormals = NULL ;
    }
    if ( vboTexCoords ) {
        vboTexCoords -> release () ;
        delete vboTexCoords ;
        vboTexCoords = NULL ;
    }
    if ( vboTangents ) {
        vboTangents -> release () ;
        delete vboTangents ;
        vboTangents = NULL ;
    }
    if ( vboIndices ) {
        vboIndices -> release () ;
        delete vboIndices ;
        vboIndices = NULL ;
    }
}


void GLWidget :: createShaders ()
{
    destroyShaders () ;
    QString vertexShaderFile [] = {
    ":/shaders/vgouraud.glsl" ,
    ":/shaders/vphong.glsl" ,
    ":/shaders/vtexture.glsl" ,
    ":/shaders/vnormal.glsl"
    };
    QString fragmentShaderFile [] = {
    ":/shaders/fgouraud.glsl" ,
    ":/shaders/fphong.glsl" ,
    ":/shaders/ftexture.glsl" ,
    ":/shaders/fnormal.glsl"
    };

    vertexShader = new QGLShader ( QGLShader :: Vertex ) ;
    if (! vertexShader -> compileSourceFile ( vertexShaderFile [
    currentShader ]) )
    qWarning () << vertexShader -> log () ;

    fragmentShader = new QGLShader ( QGLShader :: Fragment );
    if (! fragmentShader -> compileSourceFile (fragmentShaderFile [ currentShader ]) )
        qWarning () << fragmentShader -> log () ;

    shaderProgram = new QGLShaderProgram ;
    shaderProgram -> addShader ( vertexShader );
    shaderProgram -> addShader ( fragmentShader ) ;

    if (! shaderProgram -> link () )
    qWarning () << shaderProgram -> log () << endl ;
}

void GLWidget :: destroyShaders ()
{
    delete vertexShader ;
    vertexShader = NULL ;

    delete fragmentShader ;
    fragmentShader = NULL ;

    if ( shaderProgram ) {
        shaderProgram -> release () ;
        delete shaderProgram ;
        shaderProgram = NULL ;
    }
}

void GLWidget :: animate ()
{
    updateGL () ;
}

void GLWidget :: genNormals ()
{
    delete [] normals ;
    normals = new QVector3D [ numVertices ];

    for (unsigned int i =0; i < numFaces; ++i){
        unsigned int i1 = indices[i * 3];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D v1 = vertices [ i1 ]. toVector3D () ;
        QVector3D v2 = vertices [ i2 ]. toVector3D () ;
        QVector3D v3 = vertices [ i3 ]. toVector3D () ;

        QVector3D faceNormal = QVector3D :: crossProduct ( v2 - v1 , v3 - v1 ) ;

        normals [ i1 ] += faceNormal ;
        normals [ i2 ] += faceNormal ;
        normals [ i3 ] += faceNormal ;

        for ( unsigned int i = 0; i < numVertices ; i ++)
            normals [ i ]. normalize () ;
    }
}

void GLWidget :: genTexCoordsCylinder ()
{
    delete [] texCoords ;
    texCoords = new QVector2D [ numVertices ];

    double minLim = std :: numeric_limits < double >:: min () ;
    double maxLim = std :: numeric_limits < double >:: max () ;
    QVector2D max ( minLim , minLim ) ;
    QVector2D min ( maxLim , maxLim ) ;

    for ( unsigned int i = 0; i < numVertices ; i ++) {
        QVector2D pos = vertices [ i ]. toVector2D () ;
        max . setX ( qMax ( max . x () , pos . x () ) ) ;
        max . setY ( qMax ( max . y () , pos . y () ) ) ;
        min . setX ( qMin ( min . x () , pos . x () ) ) ;
        min . setY ( qMin ( min . y () , pos . y () ) ) ;
    }

    QVector2D size = max - min ;
    for ( unsigned int i = 0; i < numVertices ; i ++) {
        double x = 2.0 * ( vertices [ i ]. x () - min . x () ) /
        size . x () - 1.0;
        texCoords [ i ] = QVector2D ( acos ( x ) / M_PI , ( vertices [i ]. y () - min .y () ) / size . y () ) ;
    }
}

void GLWidget :: genTangents ()
{
    delete [] tangents ;
    tangents = new QVector4D [ numVertices ];
    QVector3D * bitangents = new QVector3D [ numVertices ];

    for (unsigned int i = 0; i < numFaces; ++i) {
        unsigned int i1 = indices[i * 3];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D E = vertices [ i1 ]. toVector3D () ;
        QVector3D F = vertices [ i2 ]. toVector3D () ;
        QVector3D G = vertices [ i3 ]. toVector3D () ;

        QVector2D stE = texCoords [ i1 ];
        QVector2D stF = texCoords [ i2 ];
        QVector2D stG = texCoords [ i3 ];

        QVector3D P = F - E;
        QVector3D Q = G - E;

        QVector2D st1 = stF - stE ;
        QVector2D st2 = stG - stE ;

        QMatrix2x2 M;

        M (0 ,0) = st2 . y () ; M (0 ,1) = - st1 . y () ;
        M (1 ,0) = - st2 . x () ; M (1 ,1) = st1 . x () ;
        M *= (1.0 / ( st1 . x () * st2 . y () - st2 . x () * st1 . y () ) ) ;

        QVector4D T = QVector4D (M (0 ,0) *P.x () +M (0 ,1) *Q.x () ,
                                 M (0 ,0) *P.y () +M (0 ,1) *Q.y () ,
                                 M (0 ,0) *P.z () +M (0 ,1) *Q.z () ,
                                 0.0) ;

        QVector3D B = QVector3D (M (1 ,0) *P.x () +M (1 ,1) *Q.x () ,
                                 M (1 ,0) *P.y () +M (1 ,1) *Q.y () ,
                                 M (1 ,0) *P.z () +M (1 ,1) *Q.z () );

        tangents [ i1 ] += T;
        tangents [ i2 ] += T;
        tangents [ i3 ] += T;

        bitangents [ i1 ] += B;
        bitangents [ i2 ] += B;
        bitangents [ i3 ] += B;
    }


    for ( unsigned int i = 0; i < numVertices ; i ++) {
        const QVector3D & n = normals [ i ];
        const QVector4D & t = tangents [i ];

        tangents [i] = (t - n * QVector3D :: dotProduct (n , t. toVector3D () )). normalized () ;

        QVector3D b = QVector3D :: crossProduct (n , t. toVector3D () );

        double hand = QVector3D :: dotProduct (b , bitangents [i ]) ;
        tangents [i ]. setW (( hand < 0.0) ? -1.0 : 1.0) ;

    }
    delete [] bitangents ;
}
