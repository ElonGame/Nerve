#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

#include "knmusicratingeditor.h"

KNMusicRatingEditor::KNMusicRatingEditor(QWidget *parent) :
    QWidget(parent)
{
    //Set pixmap data.
    m_star=QPixmap(":/Music/Resources/Music/stars/star.png");
    m_unstar=QPixmap(":/Music/Resources/Music/stars/unstar.png");

    //Set properties.
    setContentsMargins(0,0,0,0);
    setMouseTracking(true);
    setAutoFillBackground(true);
}

void KNMusicRatingEditor::paintEvent(QPaintEvent *event)
{
    //Do the original paint.
    QWidget::paintEvent(event);
    //Set the painter.
    QPainter painter(this);
    //Set the pen and brush.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Window));
    painter.drawRect(rect());
    //Paint the star number.
    for(int i=0; i<m_starNum; i++)
    {
        painter.drawPixmap(i*m_starSizeHint,
                           0,
                           m_star);
    }
    for(int i=m_starNum; i<5; i++)
    {
        painter.drawPixmap(i*m_starSizeHint,
                           0,
                           m_unstar);
    }
}

void KNMusicRatingEditor::mouseMoveEvent(QMouseEvent *event)
{
    //When the mouse is moving, change the value.
    int star=starAtPosition(event->x());
    if(star!=-1)
    {
        setStarNum(star);
        update();
    }
}

void KNMusicRatingEditor::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    emit editingFinished();
}

int KNMusicRatingEditor::starAtPosition(int x)
{
    int starNum=x<m_halfStarSizeHint?0:(x/m_starSizeHint+1);
    if(starNum<0)
    {
        return -1;
    }
    return starNum>5?5:starNum;
}

int KNMusicRatingEditor::starNum() const
{
    return m_starNum;
}

void KNMusicRatingEditor::setStarNum(int starNum)
{
    m_starNum = starNum;
}

int KNMusicRatingEditor::starSizeHint() const
{
    return m_starSizeHint;
}

void KNMusicRatingEditor::setStarSizeHint(int starSizeHint)
{
    m_starSizeHint=starSizeHint;
    m_editorWidth=m_starSizeHint*5;
    m_halfStarSizeHint=m_starSizeHint>>1;
    m_scaleStar=m_star.scaled(m_starSizeHint, m_starSizeHint, Qt::KeepAspectRatio);
    m_scaleUnstar=m_unstar.scaled(m_starSizeHint, m_starSizeHint, Qt::KeepAspectRatio);
}
