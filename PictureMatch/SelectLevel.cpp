#include "SelectLevel.h"
#include "RunGame.h"
#include "ui_SelectLevel.h"
#include "MessageDialog.h"

SelectLevel::SelectLevel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectLevel)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/Main/Resources/Main/Head.png"));
    ui->lineEdit_image_number->setValidator(new QIntValidator(10, 50, this));
    ui->lineEdit_time->setValidator(new QIntValidator(20, 600, this));
}

SelectLevel::~SelectLevel()
{
    delete ui;
}


void SelectLevel::on_btn_start_clicked()
{
    int imageNumber = ui->lineEdit_image_number->text().toInt();
    int time = ui->lineEdit_time->text().toInt();
    if (imageNumber < 10)
    {
        MessageDialog dlg(MessageDialog::MD_ERROR);
        dlg.showSelect(QString::fromLocal8Bit("图片数目应该大于10个").toUtf8().data());
        return;
    }
    if (time < 20)
    {
        MessageDialog dlg(MessageDialog::MD_ERROR);
        dlg.showSelect(QString::fromLocal8Bit("时间应该大于20秒").toUtf8().data());
        return;
    }
    QString cell = ui->comboBox_cell_number->currentText();
    int row = cell.left(2).toInt();
    int col = cell.right(2).toInt();
    m_runDlg = new RunGame(row, col, time, ":/Difficult/Resources/RunGame/Image/Difficult/", imageNumber);
    m_runDlg->startGame();
    m_runDlg->exec();
}

void SelectLevel::on_btn_exit_clicked()
{
    this->done(0);
}
