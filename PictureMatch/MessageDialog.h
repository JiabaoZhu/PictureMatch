#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    enum Type
    {
        MD_INFO = 1,
        MD_ERROR,
        MD_OK
    };

public:
    explicit MessageDialog(Type type = MD_INFO, QWidget *parent = 0);
    ~MessageDialog();

public:
    int showSelect(QString information = "");

private:
    Ui::MessageDialog *ui;
    int m_select;

protected slots:
    void on_btn_close_clicked();
    void on_btn_left_clicked();
    void on_btn_right_clicked();
};

#endif // MESSAGEDIALOG_H
