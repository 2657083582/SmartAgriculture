#include "modifylineedit.h"
#include <QKeyEvent>

ModifyLineEdit::ModifyLineEdit(QWidget *parent):QLineEdit(parent)
{

}

void ModifyLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return){
        emit submit();
        qDebug()<<"submit";
    }else if(event->key() == Qt::Key_Escape){
        emit cancel();
    }
    QLineEdit::keyPressEvent(event);
}
