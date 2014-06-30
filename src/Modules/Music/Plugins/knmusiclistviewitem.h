#ifndef KNMUSICLISTVIEWITEM_H
#define KNMUSICLISTVIEWITEM_H

#include "../Base/knmusicvieweritem.h"

class KNMusicListView;
class KNMusicLibraryModelBase;
class KNMusicSortModel;
class KNMusicListviewItem : public KNMusicViewerItem
{
    Q_OBJECT
public:
    explicit KNMusicListviewItem(QObject *parent = 0);
    ~KNMusicListviewItem();
    QWidget *viewerWidget();
    void applyPlugin();

signals:

public slots:
    void onActionResort();
    void onActionSearch(const QString &text);
    void onActionShowIndex(const QModelIndex &index);
    void onActionRemoveOriginalItem(const QModelIndex &index);
    void retranslate();
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setBackend(KNMusicBackend *backend);

protected slots:
    void onActionShowContextMenu(const QPoint &position);

private:
    KNMusicSortModel *m_listViewModel;
    KNMusicListView *m_libraryView;
    QString m_captionTitle;
};

#endif // KNMUSICLISTVIEWITEM_H
