#ifndef POLYDIALOG_H
#define POLYDIALOG_H

#include <QDialog>

namespace Ui {
class PolyDialog;
}

class PolyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolyDialog(QWidget *parent = 0);
    ~PolyDialog();

private slots:
    void on_drawButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::PolyDialog *ui;

public:
    float x1, x2, x3, x4, x5, x6;
    float y1, y2, y3, y4, y5, y6;
    int w1;
};

#endif
