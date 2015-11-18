#ifndef PICTUREMATCH_H
#define PICTUREMATCH_H

#include <QObject>

class SelectLevel;

class PictureMatch : public QObject
{
    Q_OBJECT

public:
    PictureMatch();
    ~PictureMatch();
private:
    SelectLevel *m_selectDlg;
};

#endif // PICTUREMATCH_H
