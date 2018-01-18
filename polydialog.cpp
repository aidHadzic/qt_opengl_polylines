#include "polydialog.h"
#include "ui_polydialog.h"
#include "draw.h"

PolyDialog::PolyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PolyDialog)
{
    ui->setupUi(this);
}

PolyDialog::~PolyDialog()
{
    delete ui;
}

void PolyDialog::on_drawButton_clicked()
{
    this->w1 = ui->w1->text().toInt();
    if (this->w1 != 0 && this->w1 > 0 && this->w1 < 11) {
        this->x1 = ui->x1->text().toFloat();
        this->x2 = ui->x2->text().toFloat();
        this->x3 = ui->x3->text().toFloat();
        this->x4 = ui->x4->text().toFloat();
        this->x5 = ui->x5->text().toFloat();
        this->x6 = ui->x6->text().toFloat();
        this->y1 = ui->y1->text().toFloat();
        this->y2 = ui->y2->text().toFloat();
        this->y3 = ui->y3->text().toFloat();
        this->y4 = ui->y4->text().toFloat();
        this->y5 = ui->y5->text().toFloat();
        this->y6 = ui->y6->text().toFloat();
        if (this->x1 > 1 || this->x2 > 1 || this->x3 > 1 || this->x4 > 1 || this->x5 > 1 || this->x6 > 1
                || this->x1 < -1 || this->x2 < -1 || this->x3 < -1 || this->x4 < -1 || this->x5 < -1 || this->x6 < -1
                || this->y1 > 1 || this->y2 > 1 || this->y3 > 1 || this->y4 > 1 || this->y5 > 1 || this->y6 > 1
                || this->y1 < -1 || this->y2 < -1 || this->y3 < -1 || this->y4 < -1 || this->y5 < -1 || this->y6 < -1) {
            ui->errorLabel->setText("-1 < coordinate < 1");
        } else {
            this->close();
        }
    } else {
        ui->errorLabel->setText("1 < Width < 10");
    }
}

void PolyDialog::on_cancelButton_clicked()
{
    this->close();
}
