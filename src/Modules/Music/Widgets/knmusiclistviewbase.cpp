#include "knmusiclistviewbase.h"

#include <QCursor>
#include <QDrag>
#include <QHelpEvent>
#include <QMimeData>
#include <QModelIndexList>
#include <QMouseEvent>
#include <QList>
#include <QScopedPointer>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QToolTip>
#include <QUrl>

#include <QDebug>

#include "../../knlocale.h"

#include "../Libraries/knmusicmodel.h"

#include "knmusiclistviewheader.h"
#include "knmusicratingdelegate.h"
#include "knmusicdetailtooltip.h"

KNMusicListViewBase::KNMusicListViewBase(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setIndentation(0);
    setMouseTracking(true);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //setSelectionMode(QAbstractItemView::ExtendedSelection);

    horizontalScrollBar()->setSingleStep(5);
    horizontalScrollBar()->setPageStep(5);
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    //Set locale.
    retranslate();
    connect(KNLocale::instance(), &KNLocale::languageChanged,
            this, &KNMusicListViewBase::retranslateAndSet);

    m_mimeData=new QMimeData;
    m_dragAction=new QDrag(this);

    //Set music header.
    m_headerWidget=new KNMusicListViewHeader(this);
    setHeader(m_headerWidget);
    connect(m_headerWidget, &KNMusicListViewHeader::requireChangeVisible,
            this, &KNMusicListViewBase::onSectionVisibleChanged);

    m_musicDetailTooltip=new KNMusicDetailTooltip(this);
    connect(this, &KNMusicListViewBase::activated,
            this, &KNMusicListViewBase::onItemActived);
}

KNMusicListViewBase::~KNMusicListViewBase()
{
    m_dragAction->deleteLater();
}

void KNMusicListViewBase::resetHeader()
{
    setItemDelegateForColumn(KNMusicGlobal::Rating,
                             new KNMusicRatingDelegate(this));
    setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicListViewBase::onActionSort);
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}

void KNMusicListViewBase::moveToFirst(const int &logicalHeaderIndex)
{
    m_headerWidget->moveToFirst(logicalHeaderIndex);
}

void KNMusicListViewBase::setHeaderAlignment(const int &logicalHeaderIndex,
                                             Qt::Alignment alignment)
{
    model()->setHeaderData(logicalHeaderIndex,
                           Qt::Horizontal,
                           QVariant(alignment),
                           Qt::TextAlignmentRole);
}

void KNMusicListViewBase::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    m_proxyModel=static_cast<QSortFilterProxyModel *>(model);
}

void KNMusicListViewBase::setSourceModel(KNMusicModel *musicModel)
{
    m_musicModel=musicModel;
    m_musicDetailTooltip->setMusicModel(musicModel);
}

void KNMusicListViewBase::retranslate()
{
    ;
}

void KNMusicListViewBase::retranslateAndSet()
{
    retranslate();
}

void KNMusicListViewBase::onActionSort(int logicalIndex, Qt::SortOrder order)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(order);
    if(currentIndex().isValid())
    {
        scrollTo(currentIndex(), PositionAtCenter);
    }
}

void KNMusicListViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if(event->button()==Qt::RightButton &&
          rect().contains(event->pos()))
    {
        QModelIndex posTest=indexAt(event->pos());
        if(posTest.isValid())
        {
            emit requireShowContextMenu(event->globalPos(),
                                        posTest);
        }
    }
}

void KNMusicListViewBase::leaveEvent(QEvent *event)
{
    if(!rect().contains(mapFromGlobal(QCursor::pos())))
    {
        m_musicDetailTooltip->hide();
    }
    QTreeView::leaveEvent(event);
}

void KNMusicListViewBase::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes=selectedIndexes();
    if(indexes.size()>0)
    {
        QModelIndex currentIndex;
        QList<int> rows;
        for(int i=0, listCount=indexes.size();
            i<listCount;
            i++)
        {
            currentIndex=m_proxyModel->mapToSource(indexes.at(i));
            if(!rows.contains(currentIndex.row()))
            {
                rows.append(currentIndex.row());
            }
        }
        m_mimeData->clear();
        QList<QUrl> fileUrlList;
        for(int i=0, fileCount=rows.size();
            i<fileCount;
            i++)
        {
            fileUrlList.append(QUrl::fromLocalFile(m_musicModel->filePathFromIndex(rows.at(i))));
        }
        m_mimeData->setUrls(fileUrlList);
        m_dragAction->setMimeData(m_mimeData);
        m_dragAction->exec(supportedActions, Qt::CopyAction);
    }
}

bool KNMusicListViewBase::event(QEvent *event)
{
    if(event->type()==QEvent::ToolTip)
    {
        QHelpEvent *helpEvent=static_cast<QHelpEvent *>(event);
        QPoint realPos=helpEvent->pos();
        realPos.setY(realPos.y()-header()->height());
        QModelIndex index=realPos.y()<0?QModelIndex():indexAt(realPos);
        if(index.isValid())
        {
            m_musicDetailTooltip->setTooltip(m_proxyModel->mapToSource(index),
                                             mapToGlobal(helpEvent->pos()));
            m_musicDetailTooltip->showTooltip();
        }
        else
        {
            m_musicDetailTooltip->hide();
        }
        return true;
    }
    return QTreeView::event(event);
}

void KNMusicListViewBase::onSectionVisibleChanged(const int &index,
                                              const bool &visible)
{
    setColumnHidden(index, !visible);
}

void KNMusicListViewBase::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}
