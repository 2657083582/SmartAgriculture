#ifndef SWITCH_LABEL_H
#define SWITCH_LABEL_H
#include <QLabel>
#include <QMouseEvent>

class Switch_label:public QLabel
{
    Q_OBJECT
public:
    Switch_label(QWidget *parent = nullptr, bool flag = true, bool able = true);

//    void change_state(bool flag);

//    void setEnabled(bool flag);

    void set_flag(bool flag);

    void change_flag();

    void set_able(bool able);

    bool flag()const;

    bool able()const;

//    bool image()const;

signals:
    void clicked();

    void hovered();

    void unhovered();

    void m_flagChanged();

    void m_ableChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool m_flag;

    bool m_able;

};

#endif // SWITCH_LABEL_H
