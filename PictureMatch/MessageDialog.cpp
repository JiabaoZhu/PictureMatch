#include "MessageDialog.h"
#include "ui_MessageDialog.h"

#include <QFile>
#include <QMessageBox>
#include <QImage>
#include <QApplication>
#include <QDesktopWidget>

MessageDialog::MessageDialog(Type type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow );
    this->setGeometry(0 ,0, QApplication::desktop()->width(), QApplication::desktop()->height());

    if (MD_ERROR == type)
    {
        ui->widget_back->setStyleSheet("QWidget#widget_back { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/back_lock.png);}");
        ui->lbl_round->setStyleSheet("QLabel { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/round_red.png);}");

    }
    else if (MD_OK == type)
    {
        ui->widget_back->setStyleSheet("QWidget#widget_back { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/back_unlock.png);}");
        ui->lbl_round->setStyleSheet("QLabel { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/round_green.png);}");
    }
    else
    {
        ui->widget_back->setStyleSheet("QWidget#widget_back { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/back_prompt.png);}");
        ui->lbl_round->setStyleSheet("QLabel { background-position: center center;\n    background-repeat: no-repeat;\n    background-image: url(:/Resources/MessageDialog/round_blue.png);}");
    }
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

int MessageDialog::showSelect(QString information)
{
    ui->label_title->setText(information);

    exec();

    return m_select;
}

void MessageDialog::on_btn_left_clicked()
{
    m_select = 1;
    this->done(m_select);
}

void MessageDialog::on_btn_right_clicked()
{
    m_select = 0;
    this->done(m_select);
}
void MessageDialog::on_btn_close_clicked()
{
    m_select = -1;
    this->done(m_select);
}
