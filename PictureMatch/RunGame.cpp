#include <QPixmap>
#include <QMessageBox>
#include <QPainter>

#include "RunGame.h"
#include "ui_RunGame.h"
#include "MessageDialog.h"

RunGame::RunGame(int row, int col, int totalTime, QString imageFileFamily, int imageKind, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunGame)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    setWindowIcon(QIcon(":/Main/Resources/Main/Head1.png"));

    {
        m_isRun = false;
        m_isPause = false;

        m_runTimeId = 0;
        m_useTime = 0;

        m_rowNumber = row;
        m_colNumber = col;
        m_totalTime = totalTime * 1000;
        m_imageFileFamily = imageFileFamily;
        m_imageKind = imageKind;

        m_oldPressBtn = NULL;
    }
    createMap();
}

RunGame::~RunGame()
{
    delete ui;
}

void RunGame::setImageFileFamily(QString imageFileFamily)
{
    m_imageFileFamily = imageFileFamily;
}

void RunGame::createMap()
{
    for (int i = 0; i < m_imageBtns.size(); i++)
    {
        m_imageBtns[i]->deleteLater();
    }
    m_imageBtns.clear();
    m_imageBtns.resize(m_rowNumber * m_colNumber);

    for (int i = 0; i < m_rowNumber; i++)
    {
        for (int j = 0; j < m_colNumber; j++)
        {
            QPushButton *btn = new QPushButton(ui->widget);
            btn->setCheckable(true);
            // "QPushButton { border-style: outset; }"
            btn->setStyleSheet("QPushButton { border-color: beige; }"
                               "QPushButton:checked { background-color: rgb(15, 50, 200); }");
            btn->setProperty(BTN_ROW_VALUE, i+1);
            btn->setProperty(BTN_COL_VALUE, j+1);
            m_imageBtns[i*m_colNumber + j] = btn;
        }
    }
}

void RunGame::startGame()
{
    m_isRun = true;
    m_isPause = false;

    m_useTime = 0;

    ui->progressBar_time->setValue(100);
    ui->progressBar_time->setVisible(true);
    ui->widget->setVisible(true);

    ui->btn_pause->show();
    ui->btn_continue->hide();

    m_core.CreateMap(m_rowNumber, m_colNumber, m_imageKind);
    updateImage();

    m_runTimeId = startTimer(20);
}

void RunGame::pauseGame()
{
    killTimer(m_runTimeId);
    ui->btn_pause->hide();
    ui->btn_continue->show();
    ui->btn_continue->setFocus();

    for (int i = 0; i < m_imageBtns.size(); i++)
    {
        m_imageBtns[i]->hide();
    }
}

void RunGame::continueGame()
{
    ui->btn_continue->hide();
    ui->btn_pause->show();
    ui->btn_pause->setFocus();

    for (int i = 0; i < m_imageBtns.size(); i++)
    {
        m_imageBtns[i]->show();
    }
    m_runTimeId = startTimer(20);
}

void RunGame::setUseTime(long useTime)
{
    m_useTime = useTime * 1000;
}

void RunGame::updateImage()
{
    // 获得 widget 的大小
    int widgetWidth = ui->widget->width();
    int widgetHeight = ui->widget->height();

    // 获得图片可能的尺寸
    int imageWidth = widgetWidth / m_colNumber;
    int imageHeight = widgetHeight / m_rowNumber;

    // 使用最小的尺寸
    if (imageWidth > imageHeight)
    {
        imageWidth = imageHeight;
    }
    imageHeight = imageWidth;

    // 获得top与left坐标
    int x = (widgetWidth - imageWidth * m_colNumber) / 2;
    int y = (widgetHeight - imageHeight * m_rowNumber) / 2;

    m_imageBtnWidth = imageWidth;
    m_imageBtnHeight = imageHeight;
    m_imageBtnTop = y;
    m_imageBtnLeft = x;

    for (int i = 0; i < m_colNumber; i++)
    {
        for (int j = 0; j < m_rowNumber; j++)
        {
            int value = m_core.GetCell(j+1, i+1);
            if (CELL_NO_HAS_IMAGE != value)
            {
                QPushButton * btn = m_imageBtns[j*m_colNumber + i];
                btn->setFocusPolicy(Qt::NoFocus);
                btn->resize(QSize(imageWidth, imageHeight));
                btn->move(x + imageWidth * i, y + imageHeight * j);

                btn->setProperty(BTN_IMAGE_VALUE, value);
                QPixmap image;
                QString imageFile = QString("%1.png").arg(value);
                image.load(m_imageFileFamily + imageFile);
                btn->setIcon(image);
                btn->setIconSize(QSize(imageWidth - 6, imageHeight - 6));
                btn->show();
                connect(btn, SIGNAL(clicked()), this, SLOT(on_btn_image_clicked()));
            }
        }
    }

}

void RunGame::rearrange()
{
    m_core.ReLayout();
    updateImage();
}

void RunGame::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Escape)
    {
        ui->btn_back->click();
    }
}

void RunGame::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_runTimeId)
    {
        m_useTime += 20;

        if (m_core.IsEmpty())
        {
            MessageDialog dlg(MessageDialog::MD_ERROR);
            killTimer(m_runTimeId);
            dlg.showSelect(QString::fromLocal8Bit("图片被清空了, 您赢了").toUtf8().data());
            this->done(0);
        }

        // 时间到
        if (m_useTime >= m_totalTime)
        {
            MessageDialog dlg(MessageDialog::MD_ERROR);
            killTimer(m_runTimeId);
            dlg.showSelect(QString::fromLocal8Bit("时间到, 您输了").toUtf8().data());
            this->done(0);
        }

        ui->progressBar_time->setValue((m_totalTime - m_useTime)* 10000 / (m_totalTime));
        ui->label_time_number->setText(QString::number((m_totalTime - m_useTime) / 1000));
    }
}

void RunGame::on_btn_pause_clicked()
{
    disconnect(ui->btn_pause, SIGNAL(clicked(bool)), this, SLOT(on_btn_pause_clicked));
    pauseGame();
    connect(ui->btn_pause, SIGNAL(clicked(bool)), this, SLOT(on_btn_pause_clicked));
}

void RunGame::on_btn_continue_clicked()
{
    disconnect(ui->btn_continue, SIGNAL(clicked(bool)), this, SLOT(on_btn_continue_clicked));
    continueGame();
    connect(ui->btn_continue, SIGNAL(clicked(bool)), this, SLOT(on_btn_continue_clicked));
}

void RunGame::on_btn_rearrange_clicked()
{
    disconnect(ui->btn_rearrange, SIGNAL(clicked(bool)), this, SLOT(on_btn_rearrange_clicked));
    rearrange();
    connect(ui->btn_rearrange, SIGNAL(clicked(bool)), this, SLOT(on_btn_rearrange_clicked));
}

void RunGame::on_btn_back_clicked()
{
    disconnect(ui->btn_back, SIGNAL(clicked(bool)), this, SLOT(on_btn_back_clicked));
    MessageDialog dlg;
    if (1 == dlg.showSelect(QString::fromLocal8Bit("是否退出？").toUtf8().data()))
    {
        this->done(0);
    }
    connect(ui->btn_back, SIGNAL(clicked(bool)), this, SLOT(on_btn_back_clicked));
}


void RunGame::on_btn_image_clicked()
{
    QPushButton *btn = (QPushButton *)this->sender();

    if(m_oldPressBtn == NULL)
    {
        m_oldPressBtn = btn;
    }
    else if(m_oldPressBtn != btn)
    {
        int nowRow = btn->property(BTN_ROW_VALUE).toInt();
        int nowCol = btn->property(BTN_COL_VALUE).toInt();

        int oldRow = m_oldPressBtn->property(BTN_ROW_VALUE).toInt();
        int oldCol = m_oldPressBtn->property(BTN_COL_VALUE).toInt();

        std::vector<int> result = m_core.Connect(oldRow, oldCol, nowRow, nowCol);
        if (!result.empty())
        {
            m_core.SetCell(oldRow, oldCol, CELL_NO_HAS_IMAGE);
            m_core.SetCell(nowRow, nowCol, CELL_NO_HAS_IMAGE);

            btn->hide();
            m_oldPressBtn->hide();

            m_oldPressBtn = NULL;
        }
        else
        {
            m_oldPressBtn->setChecked(false);
            m_oldPressBtn = btn;
            btn->setChecked(true);
        }
    }
}
