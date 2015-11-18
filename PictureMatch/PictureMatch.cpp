#include "PictureMatch.h"
#include "SelectLevel.h"

PictureMatch::PictureMatch()
{
    m_selectDlg = new SelectLevel();
    m_selectDlg->show();
}

PictureMatch::~PictureMatch()
{

}
