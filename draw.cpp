#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include "draw.h"

Draw::Draw(QString filename, QWidget *parent, bool fs)
    : QOpenGLWidget(parent)
    , backgroundImage(filename)
    , fullscreen(fs)
    , scribbling(false)
    , rightPress(false)
    , isSetColor(false)
    , rightChangedColor(0)
{
    setupUI();
    setGeometry(100, 100, 800, 600);
    connect(lineColorButton, SIGNAL(clicked(bool)), this, SLOT(setColor()));
}

void Draw::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    loadTextures();

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH_HINT);
    glEnable(GL_LINE_SMOOTH_HINT);
}

void Draw::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawBackGround();
    switch (this->state) {
    case 0:
        drawFreeHand();
        break;
    case 1:
        drawPolyLine();
        break;
    default:
        clearDrawing();
        drawFreeHand();
        break;
    }
    update();
}

void Draw::drawFreeHand()
{
    glLoadIdentity();
    for (int i = 0; i < freeHandPoints.size(); i++) {
        glLineWidth(lineWidths.at(i));
        glColor3f(lineColors.at(i).red() / 255.0, lineColors.at(i).green() / 255.0, lineColors.at(i).blue() / 255.0);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < freeHandPoints.at(i).size() - 1; j = j + 2) {
            glVertex3f(freeHandPoints.at(i).at(j).x(), freeHandPoints.at(i).at(j).y(), 0);
            glVertex3f(freeHandPoints.at(i).at(j + 1).x(), freeHandPoints.at(i).at(j + 1).y(), 0);
        }
        glEnd();
    }
}

void Draw::drawPolyLine()
{
    glLoadIdentity();
    for (int i = 0; i < polyLines.size(); i++) {
        glLineWidth(lineWidths.at(i));
        glColor3f(lineColors.at(i).red() / 255.0, lineColors.at(i).green() / 255.0, lineColors.at(i).blue() / 255.0);

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < polyLines.at(i).size()-1; ++j) {
            glVertex3f(polyLines.at(i).at(j).x, polyLines.at(i).at(j).y,0);
            glVertex3f(polyLines.at(i).at(j + 1).x, polyLines.at(i).at(j + 1).y, 0);
        }
        glEnd();
    }
}

void Draw::drawBackGround()
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);     glVertex3f(-1, -1, 0);
        glTexCoord2f(1, 0);     glVertex3f(1, -1, 0);
        glTexCoord2f(1, 1);     glVertex3f(1, 1, 0);
        glTexCoord2f(0, 1);     glVertex3f(-1, 1, 0);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Draw::clearDrawing()
{
    QMessageBox::StandardButton msg
            = QMessageBox::question(NULL, tr("Asked"), tr("Are you sure you want to clear the screen?")
                                    , QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (msg == QMessageBox::Yes) {
        this->freeHandPoints.clear();
        this->lineColors.clear();
        this->lineWidths.clear();
        this->polyPoints.clear();
        this->polyLines.clear();
    }
}

void Draw::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }
    glViewport(100, 100, (GLint)width, (GLint)height);
}

void Draw::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Delete:
        clearDrawing();
        break;
    case Qt::Key_P: {
        this->freeHandPoints.clear();
        this->lineColors.clear();
        this->lineWidths.clear();
        this->polyPoints.clear();
        this->polyLines.clear();
        PolyDialog pd;
        pd.exec();
        polyPoint p1, p2, p3, p4, p5, p6;
        p1.x = pd.x1; p1.y = pd.y1;
        p2.x = pd.x2; p2.y = pd.y2;
        p3.x = pd.x3; p3.y = pd.y3;
        p4.x = pd.x4; p4.y = pd.y4;
        p5.x = pd.x5; p5.y = pd.y5;
        p6.x = pd.x6; p6.y = pd.y6;
        this->polyPoints.push_back(p1);
        this->polyPoints.push_back(p2);
        this->polyPoints.push_back(p3);
        this->polyPoints.push_back(p4);
        this->polyPoints.push_back(p5);
        this->polyPoints.push_back(p6);
        this->polyLines.push_back(polyPoints);
        this->lineWidths.push_back(pd.w1);
        this->lineColors.push_back(QColor(0, 0, 0));
        break; }
    case Qt::Key_0:
        this->state = 0;
        this->freeHandPoints.clear();
        this->lineColors.clear();
        this->lineWidths.clear();
        this->polyPoints.clear();
        this->polyLines.clear();
        break;
    case Qt::Key_1:
        this->state = 1;
        this->freeHandPoints.clear();
        this->lineColors.clear();
        this->lineWidths.clear();
        this->polyPoints.clear();
        this->polyLines.clear();
        break;
    default:
        event->ignore();
        break;
    }
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    QVector<QPointF> allPoint;
    polyPoint p;
    p.x = (float)(2.0 * event->pos().x() / width() - 1);
    p.y = (float)(-2.0 * event->pos().y() / height() + 1);
    this->polyPoints.push_back(p);
    this->polyLines.push_back(polyPoints);
    lastPoint = event->pos();
    QPointF destPoint = translatePoint(lastPoint);
    allPoint.push_back(destPoint);
    freeHandPoints.push_back(allPoint);
    lineWidths.push_back(lineWidthSpinBox->value());
    if (!rightPress && !isSetColor) {
        if (lineColors.size() != 0) {
            lineColors.push_back(lineColors.at(lineColors.size() - 1));
        } else {
            lineColors.push_back(QColor(0, 0, 0));
        }
    }
    rightPress = false;
    isSetColor = false;
    scribbling = true;

}

void Draw::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton ) && scribbling) {
        currentPoint = event->pos();
        QPointF destPoint = translatePoint(currentPoint);
        QVector<QPointF> allPoint = freeHandPoints.at(freeHandPoints.size() - 1);
        allPoint.push_back(destPoint);
        freeHandPoints.pop_back();
        freeHandPoints.push_back(allPoint);
    }
}

void Draw::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        currentPoint = event->pos();
        QPointF destPoint = translatePoint(currentPoint);
        QVector<QPointF> allPoint = freeHandPoints.at(freeHandPoints.size() - 1);
        allPoint.push_back(destPoint);
        freeHandPoints.pop_back();
        freeHandPoints.push_back(allPoint);
        lastPoint = currentPoint;
        scribbling = false;
    }
}

void Draw::setupUI()
{
    lineWidthLabel = new QLabel(tr("pen width : "), this);
    lineWidthLabel->setGeometry(QRect(10, 10, 70, 20));
    lineWidthSpinBox = new MySpinBox(this);
    lineWidthSpinBox->setRange(1, 10);
    lineWidthSpinBox->setValue(5);
    lineWidthSpinBox->setGeometry(QRect(80, 10, 35, 20));
    QPixmap pixmap;
    pixmap.load(":/resources/color.jpg");
    pixmap = pixmap.scaled(30, 20);
    lineColorButton = new QPushButton(this);
    lineColorButton->setIcon(pixmap);
    lineColorButton->setIconSize(QSize(pixmap.width(), pixmap.height()));
    lineColorButton->setGeometry(QRect(120, 10, 30, 20));
}

void Draw::loadTextures()
{
    QImage *image = new QImage(800, 600, QImage::Format_RGB32);
    image->load(backgroundImage);
    tex = QGLWidget::convertToGLFormat(*image);
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

QPointF Draw::translatePoint(QPointF srcPoint)
{
    QPointF destPoint;
    destPoint.setX(2 * srcPoint.x() / width() - 1);
    destPoint.setY(-2 * srcPoint.y() / height() + 1);
    return destPoint;
}

void Draw::setColor()
{
    isSetColor = true;
    QColorDialog colorDialog;
    lineColors.push_back(colorDialog.getColor());
}
