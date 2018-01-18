#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QStackedWidget(parent)
    , index(0)
{
    QStringList backroundList;
    backroundList <<":/resources/nature1.jpg"<<":/resources/nature2.jpg"<<":/resources/nature3.jpg"<<":/resources/nature4.jpg";
    for (int i = 0; i < 4; i++) {
        draw[i] = new Draw(backroundList.at(i), this, false);
        this->addWidget(draw[i]);
    }
    setCurrentIndex(0);
    this->resize(800, 600);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_PageUp:
            if ((--index) < 0) {
                index = 3;
            }
            this->setCurrentIndex(index);
            break;
        case Qt::Key_PageDown:
            (++index) %= 4;
            this->setCurrentIndex(index);
            break;
    }
}
