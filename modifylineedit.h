#ifndef MODIFYLINEEDIT_H
#define MODIFYLINEEDIT_H

#include <QObject>
#include <QLineEdit>

class ModifyLineEdit:public QLineEdit
{
    Q_OBJECT
public:
    ModifyLineEdit(QWidget *parent=nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    //void keyReleaseEvent(QKeyEvent *event);

signals:
    void submit();
    void cancel();
};

#endif // MODIFYLINEEDIT_H
