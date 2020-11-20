#include "glwidget.h"
#include <fstream>
#include <QFileDialog>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
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

    update();
}


void GLWidget :: showFileOpenDialog ()
{
    QByteArray fileFormat = " off ";
    QString fileName ;
    fileName = QFileDialog :: getOpenFileName ( this , " Open File " , QDir :: homePath () , QString ( " %1 Files (*.%2) " )
    . arg ( QString ( fileFormat.toUpper() ) )
    . arg ( QString ( fileFormat )));

    if (! fileName . isEmpty () ) {
        readOFFFile ( fileName );

//        genNormals () ;
//        genTexCoordsCylinder () ;
//        genTangents () ;

//        createVBOs () ;
//        createShaders () ;

        update ();
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
//    stream >> numVertices >> numFaces >> line ;

//    delete [] vertices ;
//    vertices = new QVector4D [ GLWidget::numVertices ];

//    delete [] indices ;

//    indices = new unsigned int [ numFaces * 3];

//    if ( numVertices > 0) {
//        double minLim = std :: numeric_limits < double >:: min () ;
//        double maxLim = std :: numeric_limits < double >:: max () ;
//        QVector4D max ( minLim , minLim , minLim , 1.0) ;
//        QVector4D min ( maxLim , maxLim , maxLim , 1.0) ;

//        for ( unsigned int i = 0; i< numVertices ; i ++) {
//            double x , y , z;
//            stream >> x >> y >> z;
//            max . setX ( qMax ( max .x () , x));
//            max . setY ( qMax ( max .y () , y));
//            max . setZ ( qMax ( max .z () , z));
//            min . setX ( qMin ( min .x () , x));
//            min . setY ( qMin ( min .y () , y));
//            min . setZ ( qMin ( min .z () , z));

//            vertices [i] = QVector4D (x , y , z , 1.0) ;

//        }

//        QVector4D midpoint = ( min + max ) * 0.5;
//        double invdiag = 1 / ( max - min ) . length () ;

//        for ( unsigned int i = 0; i < numVertices ; i ++) {
//            vertices [i] = ( vertices [i] - midpoint )* invdiag ;
//            vertices [i ]. setW (1) ;
//        }
//    }

//    for ( unsigned int i = 0; i < numFaces ; i ++) {
//        unsigned int a , b , c;
//        stream >> line >> a >> b >> c ;
//        indices [ i * 3
//        ] = a;
//        indices [ i * 3 + 1] = b ;
//        indices [ i * 3 + 2] = c ;
//    }

    stream.close();
}
