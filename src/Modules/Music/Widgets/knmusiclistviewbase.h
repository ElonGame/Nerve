#ifndef KNMUSICLISTVIEWBASE_H
#define KNMUSICLISTVIEWBASE_H

#include <QTreeView>

class QSortFilterProxyModel;
class KNMusicModel;
class KNMusicDetailTooltip;
class KNMusicListViewHeader;
class KNMusicListViewBase : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListViewBase(QWidget *parent = 0);
    virtual void resetHeader();
    void moveToFirst(const int &logicalHeaderIndex);
    void setHeaderAlignment(const int &logicalHeaderIndex,
                            Qt::Alignment alignment);
    void setModel(QAbstractItemModel *model);
    void setSourceModel(KNMusicModel *musicModel);

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

protected slots:
    virtual void onActionSort(int logicalIndex, Qt::SortOrder order);

private slots:
    void onSectionVisibleChanged(const int &index,
                                 const bool &visible);
    void onItemActived(const QModelIndex &index);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void startDrag(Qt::DropActions supportedActions);

private:
    KNMusicListViewHeader *m_headerWidget;
    QModelIndex m_detailIndex;
    KNMusicDetailTooltip *m_musicDetailTooltip;
    KNMusicModel *m_musicModel;
    QSortFilterProxyModel *m_proxyModel;
};

#endif // KNMUSICLISTVIEWBASE_H
