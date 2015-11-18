#ifndef RUNGAME_H
#define RUNGAME_H

#include <QDialog>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QVector>

#include "PictureMatchingCore.h"

#define BTN_IMAGE_VALUE "imageValue"
#define BTN_ROW_VALUE "rowValue"
#define BTN_COL_VALUE "colValue"

namespace Ui {
class RunGame;
}

class RunGame : public QDialog
{
    Q_OBJECT

public:
    // 行数, 列数, 时间s, 图片组, 图片的种类
    explicit RunGame(int row, int col, int totalTime, QString imageFileFamily, int imageKind, QWidget *parent = 0);
    ~RunGame();

    void keyPressEvent(QKeyEvent *keyEvent);

    void timerEvent(QTimerEvent *);

    // 创建地图
    void createMap();

    // 开始游戏
    void startGame();

    // 暂停
    void pauseGame();

    // 继续
    void continueGame();

    // 设置已使用时间 s
    void setUseTime(long useTime);

    // 设置图片家族
    // 设置完成后需要更新图片
    void setImageFileFamily(QString);

    // 更新图片
    void updateImage();

    // 重新排列
    void rearrange();

private slots:
    void on_btn_pause_clicked();
    void on_btn_continue_clicked();
    void on_btn_rearrange_clicked();
    void on_btn_back_clicked();
    void on_btn_image_clicked();

private:
    Ui::RunGame *ui;
    PictureMatchingCore m_core;

    bool m_isRun;               // 是否为运行状态
    bool m_isPause;             // 是否为暂停状态

    int m_runTimeId;            // 倒计时的定时器的Id
    long m_totalTime;           // 游戏总时间 ms
    long m_useTime;             // 已经使用的时间 ms

    int m_rowNumber;            // 行数
    int m_colNumber;            // 列数
    int m_imageKind;            // 图片种类

    QString m_imageFileFamily;  // 图片家族

    QPushButton *m_oldPressBtn; // 上一个按下的按钮

    // 所有按钮
    QVector<QPushButton *> m_imageBtns;

private:
    int m_imageBtnWidth;
    int m_imageBtnHeight;
    int m_imageBtnLeft;
    int m_imageBtnTop;
};

#endif // RUNGAME_H
