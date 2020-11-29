#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>


class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    void readOFFFile ( const QString & fileName );
    void genNormals();
    void genTextCoordsCylinder();
    void genTangents();

    void genTexCoordsCylinder () ;
    void createVBOs () ;
    void destroyVBOs () ;
    void createShaders () ;
    void destroyShaders () ;

    QPointF pixelPosToViewPos ( const QPointF &p ) ;

    unsigned int numVertices ;
    unsigned int numFaces ;
    QVector4D * vertices ;
    QVector3D * normals ;
    QVector2D * texCoords ;
    QVector4D * tangents ;
    unsigned int * indices ;

    QGLBuffer *vboVertices ;
    QGLBuffer *vboNormals ;
    QGLBuffer *vboTexCoords ;
    QGLBuffer *vboTangents ;
    QGLBuffer *vboIndices ;

    QGLShader * vertexShader ;
    QGLShader * fragmentShader ;
    QGLShaderProgram * shaderProgram ;
    unsigned int currentShader ;

    int texID [2];

    QMatrix4x4 modelViewMatrix ;
    QMatrix4x4 projectionMatrix ;

//    Camera camera ;
//    Light light ;
//    Material material ;

    double zoom ;

    unsigned int fpsCounter;

    QTimer timer ;

public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~ GLWidget () ;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:
    void statusBarMessage ( QString ns ) ;

public slots:
    void toggleBackgroundColor(bool toBlack);
    void showFileOpenDialog();
    void animate () ;
};


#endif // GLWIDGET_H
