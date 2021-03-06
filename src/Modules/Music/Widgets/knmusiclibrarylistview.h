#ifndef KNMUSICLIBRARYLISTVIEW_H
#define KNMUSICLIBRARYLISTVIEW_H

#include <QMimeData>

#include "../Libraries/knmusiclibrarymodel.h"

#include "knmusiclistviewbase.h"

class QDrag;
class QSortFilterProxyModel;
class KNMusicBackend;
class KNMusicLibraryModelBase;
class KNMusicDetailTooltip;
class KNMusicListViewHeader;
class KNMusicLibraryListview : public KNMusicListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicLibraryListview(QWidget *parent = 0);
    ~KNMusicLibraryListview();
    void resetHeader();
    void setModel(QAbstractItemModel *model);
    void setSourceModel(KNMusicLibraryModelBase *musicModel);
    void setMusicBackend(KNMusicBackend *backend);

signals:
    void requireShowContextMenu(const QPoint &position);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void closeEvent(QCloseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void hideEvent(QHideEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    bool event(QEvent *event);

private slots:
    void onSectionVisibleChanged(const int &index,
                                 const bool &visible);
    void onItemActived(const QModelIndex &index);

private:
    void setSelectedRows();
    KNMusicListViewHeader *m_headerWidget;
    QModelIndex m_detailIndex;
    KNMusicDetailTooltip *m_musicDetailTooltip;
    KNMusicLibraryModelBase *m_musicModel;
    QSortFilterProxyModel *m_proxyModel;
    QDrag *m_dragAction;
    QMimeData *m_mimeData;
};

#endif // KNMUSICLIBRARYLISTVIEW_H
