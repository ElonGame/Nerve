#include <QScrollBar>
#include <QTimeLine>
#include <QSortFilterProxyModel>

#include <QDebug>

#include "../Libraries/knmusiccategorymodel.h"

#include "knmusiccategorylist.h"

KNMusicCategoryList::KNMusicCategoryList(QWidget *parent) :
    QListView(parent)
{
    //Set viewport properties.
    viewport()->setContentsMargins(0,0,0,0);
    viewport()->installEventFilter(this);

    //Set listview properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setIconSize(QSize(40, 40));
    setLineWidth(0);
    setLayoutMode(QListView::Batched);
    setMinimumWidth(200);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSpacing(2);
    setUniformItemSizes(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set scroll bar properties.
    verticalScrollBar()->setSingleStep(8);
    verticalScrollBar()->setPageStep(8);

    //Set palette.
    m_backgroundColor=QColor(m_minGrey, m_minGrey, m_minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Text, QColor(0x8f, 0x8f, 0x8f, 0xaf));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    //Set mouse sensative animation time line.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x50);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicCategoryList::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(m_minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicCategoryList::changeBackground);
}

void KNMusicCategoryList::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    QListView::enterEvent(e);
}

void KNMusicCategoryList::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    QListView::leaveEvent(e);
}

void KNMusicCategoryList::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    int baseGrey=((frameData-m_minGrey)>>1)+m_minGrey;
    m_palette.setColor(QPalette::Base, QColor(baseGrey, baseGrey, baseGrey));
    m_palette.setColor(QPalette::Window, QColor(baseGrey, baseGrey, baseGrey));
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    int textParam=(frameData<<1)+95;
    m_palette.setColor(QPalette::Text, QColor(textParam,
                                              textParam,
                                              textParam,
                                              0xAF+frameData));
    setPalette(m_palette);
}
