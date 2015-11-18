#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H

#include <QDialog>

namespace Ui {
class SelectLevel;
}

class RunGame;

class SelectLevel : public QDialog
{
    Q_OBJECT

public:
    explicit SelectLevel(QWidget *parent = 0);
    ~SelectLevel();

private:
    Ui::SelectLevel *ui;
    RunGame *m_runDlg;

private slots:
    void on_btn_start_clicked();
    void on_btn_exit_clicked();
};

#endif // SELECTLEVEL_H
