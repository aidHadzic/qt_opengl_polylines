#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include "draw.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_HEADER

class MainWindow : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
private:
    Draw *draw[4];
    int index;
};

#endif
