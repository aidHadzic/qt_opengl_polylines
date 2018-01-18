#ifndef DRAW_H
#define DRAW_H

#include "polydialog.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QImage>
#include <QColor>
#include <QSpinBox>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QColorDialog;
QT_END_NAMESPACE

class MySpinBox : public QSpinBox
{
public:
    MySpinBox(QWidget *parent = 0) : QSpinBox(parent){}
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE
    {
        if (event->key() == Qt::Key_Down) {
            setValue(value() - 1);
        } else if(event->key() == Qt::Key_Up) {
            setValue(value() + 1);
        } else {
            event->ignore();
        }
    }
};

class Draw : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Draw(QString filename, QWidget *parent = 0, bool fs = false);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int, int) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    void setupUI();
    void loadTextures();
    void drawFreeHand();
    void drawPolyLine();
    void drawBackGround();
    void clearDrawing();
    QPointF translatePoint(QPointF);

private slots:
    void setColor();

private:
    struct polyPoint {
        float x;
        float y;
    };
    QVector<QVector<QPointF>> freeHandPoints;
    QVector<int> lineWidths;
    QVector<QColor> lineColors;

    QVector<QVector<polyPoint>> polyLines;
    QVector<polyPoint> polyPoints;

    QString backgroundImage;
    QImage tex;

    QPointF lastPoint;
    QPointF currentPoint;

    GLuint texture[2];

    bool fullscreen;
    bool scribbling;
    bool rightPress;
    bool isSetColor;

    int state;
    int rightChangedColor;

    QLabel *lineWidthLabel;
    MySpinBox *lineWidthSpinBox;
    QPushButton *lineColorButton;
};

#endif
