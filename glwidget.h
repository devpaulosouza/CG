#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    void readOFFFile ( const QString & fileName );

public:
    explicit GLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:

public slots:
    void toggleBackgroundColor(bool toBlack);
    void showFileOpenDialog();
};


#endif // GLWIDGET_H
