#ifndef KNMUSICHEADERWIDGET_H
#define KNMUSICHEADERWIDGET_H

#include <QModelIndex>
#include <QThread>

#include "../../Base/knstdlibheaderwidget.h"

class QAbstractItemModel;
class QBoxLayout;
class QResizeEvent;
class KNSearchBox;
class KNMusicPlaylistManagerBase;
class KNMusicInfoCollector;
class KNMusicLibraryModelBase;
class KNMusicBackend;
class KNMusicLRCViewer;
class KNMusicHeaderPlayer;
class KNMusicHeaderWidget : public KNStdLibHeaderWidget
{
    Q_OBJECT
public:
    explicit KNMusicHeaderWidget(QWidget *parent = 0);
    void setPlaylistManager(KNMusicPlaylistManagerBase *manager);
    void setMusicModel(KNMusicLibraryModelBase *model);
    void setBackend(KNMusicBackend *backend);
    void setAlbumArt(const QPixmap &albumArt);
    void setTitle(const QString &string);
    void setArtist(const QString &string);
    void setAlbum(const QString &string);
    KNMusicHeaderPlayer *player();

signals:
    void requireSearch(const QString &text);
    void requireShowInCurrent(const QModelIndex &index);
    void requireUpdatePlaylistModel(QAbstractItemModel *playlistModel);
    void requireLostFocus();
    void requireShowMusicPlayer();
    void requireHideMusicPlayer();

public slots:
    void retranslate();
    void retranslateAndSet();
    void setSearchFocus();
    void clearSearch();
    void onActionPlayMusic(const QString &filePath);

private slots:
    void onActionPlayListPrev();
    void onActionPlayListNext();
    void onActionCurrentFinished();

private:
    void playCurrent();
    void loadCurrentLyrics();
    void resetPlayer();
    QBoxLayout *m_mainLayout;
    QWidget *m_visualEffect;
    KNSearchBox *m_searchBox;
    KNMusicLibraryModelBase *m_musicModel;
    KNMusicLRCViewer *m_lrcViewer;
    KNMusicPlaylistManagerBase *m_playlistManager;
    KNMusicHeaderPlayer *m_headerPlayer;
    KNMusicInfoCollector *m_infoCollector;
    QString m_searchPlaceHolder, m_currentPath;
};

#endif // KNMUSICHEADERWIDGET_H
