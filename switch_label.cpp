#include "switch_label.h"

Switch_label::Switch_label(QWidget *parent, bool flag, bool able)
    :QLabel(parent),m_flag(flag),m_able(able)
{
    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);

    connect(this, &Switch_label::m_flagChanged, this, [&](){
        QString img;
        if(m_flag){
            img=":/滑动按钮（yes）.png";
        }else{
            img=":/滑动按钮（X）.png";
        }
        this->setStyleSheet("image: url("+img+")");
    });

    connect(this, &Switch_label::m_ableChanged, this, [&](){
        QString img;
        if(m_able){
            img=":/滑动按钮（X） (1).png";
        }else if(m_flag){
            img=":/滑动按钮（yes）.png";
        }else{
            img=":/滑动按钮（X）.png";
        }
        this->setStyleSheet("image: url("+img+")");
    });
}

//void Switch_label::change_state(bool flag)
//{
//    if(!m_able) return;
//    QString img=(flag)?":/滑动按钮（yes）.png":":/滑动按钮（X）.png";
//    this->setStyleSheet("image: url("+img+")");
//}

//void Switch_label::setEnabled(bool flag)
//{
//    m_able=flag;
//    QString img=(m_able)?":/滑动按钮（X） (1).png":
//}

void Switch_label::set_flag(bool flag)
{
    if(m_able) return;

    if(m_flag != flag){
        m_flag = flag;
        emit m_flagChanged();
        qDebug()<<"flag changed";
    }
}

void Switch_label::change_flag()
{
    if(m_able) return;
    m_flag = !m_flag;
    emit m_flagChanged();
}

void Switch_label::set_able(bool able)
{
    if(m_able != able){
        m_able = able;
        emit m_ableChanged();
        qDebug()<<"able changed";
    }
}

bool Switch_label::able() const
{
    return m_able;
}

bool Switch_label::flag() const
{
    return m_flag;
}

void Switch_label::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

bool Switch_label::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this){
        if(event->type() == QEvent::Enter){
            emit hovered();
        }else if(event->type() == QEvent::Leave){
            emit unhovered();
        }
    }
    return QLabel::eventFilter(obj,event);
}
