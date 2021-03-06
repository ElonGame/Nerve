#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QAbstractItemView>
#include <QLabel>

class QBoxLayout;
class QMouseEvent;
class QPaintEvent;
class QPropertyAnimation;
class QResizeEvent;
class QParallelAnimationGroup;
class QTimeLine;
class KNMusicBackend;
class KNScrollLabel;
class KNMusicLibraryModelBase;
class KNMusicAlbumSongListView;
class KNMusicAlbumDetailModel;
class KNMusicAlbumModel;
class KNMusicCategorySortFilterModel;
class KNRightSideShadow;
class KNLeftSideShadow;
class KNMusicAlbumArtwork : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumArtwork(QWidget *parent = 0);

signals:
    void requireShowArtwork();
    void requireHideArtwork();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
};

class KNMusicAlbumSongDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumSongDetail(QWidget *parent = 0);
    ~KNMusicAlbumSongDetail();
    void setAlbumText(const QString &name);
    void setArtistText(const QString &name);
    void setYearText(const QString &name);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void selectItem(const QModelIndex &index);
    void resetHeader();
    void resetSongState();
    void setMusicBackend(KNMusicBackend *backend);
    void setSourceModel(KNMusicLibraryModelBase *model);

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position);

public slots:
    void hideDetailInfo();
    void showDetailInfo();

private:
    void setDetailInfoVisible(bool visible);
    KNScrollLabel *m_albumCaption;
    QLabel *m_artistCaption, *m_yearCaption;
    KNMusicAlbumSongListView *m_albumSongs;
    QBoxLayout *m_mainLayout, *m_artistYearLayout;
    QBoxLayout *m_detailLayout;
};

class KNMusicAlbumDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumDetail(QWidget *parent = 0);
    ~KNMusicAlbumDetail();
    void setAlbumArt(const QPixmap &pixmap,
                     const QSize &size);
    void setAlbumText(const QString &name);
    void setArtistText(const QString &name);
    void setYearText(const QString &value);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void setSourceModel(KNMusicLibraryModelBase *model);
    void selectItem(const QModelIndex &index);
    void resetHeader();
    void resetSongState();
    void disableArtworkExpand();
    void setMusicBackend(KNMusicBackend *backend);

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position);
    void requireFlyBack();
    void requireFlyOut();

public slots:
    void hideDetailWidget();
    void showDetailWidget();
    void expandDetail();
    void foldDetail();
    void flyAway();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void hideDetailContent();
    void showDetailContent();
    void showArtwork();
    void hideArtwork();
    void raiseArtwork();
    void raisePanel();

private:
    KNMusicAlbumArtwork *m_albumArt;
    KNMusicAlbumSongDetail *m_songPanel;
    KNRightSideShadow *m_rightShadow;
    KNLeftSideShadow *m_leftShadow;
    QBoxLayout *m_infoListLayout, *m_artInfoLayout;
    QPropertyAnimation *m_showExpand, *m_showShrink, *m_hideExpand, *m_hideShrink,
                       *m_flyOut, *m_coverExpand, *m_coverShrink;
    bool m_albumArtExpanding=false;
    int m_shadowWidth=40;
};

class KNMusicAlbumView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit KNMusicAlbumView(QWidget *parent = 0);
    QModelIndex indexAt(const QPoint &point) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QRect visualRect(const QModelIndex &index) const;
    void setModel(QAbstractItemModel *model);
    void setCategoryModel(KNMusicCategorySortFilterModel *model);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void selectCategoryItem(const QString &value);
    void selectItem(const QModelIndex &index);
    void selectMusicItem(const QModelIndex &index);
    int gridMinimumWidth() const;
    void setGridMinimumWidth(int gridMinimumWidth);
    void resetHeader();
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setMusicBackend(KNMusicBackend *backend);
    void setFilterFixedString(const QString &text);

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position);

public slots:
    void selectAlbum(const QModelIndex &index);

protected slots:
    void updateGeometries();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                                        Qt::KeyboardModifiers modifiers);
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void expandAlbumDetail(const QModelIndex &index);
    void onActionHideAlbumDetail();
    void onActionHideAlbumDetailFinished();
    void onActionAlbumRemoved(const QModelIndex &index);
    void onActionFlyAwayAlbumDetail();
    void onActionFlyAwayAlbumDetailFinished();
    void onActionScrolling();
    void onActionAlbumArtistChagned(const QModelIndex &index);

private:
    void syncDetailData();
    void flyAwayAlbumDetail();
    void foldAlbumDetail();
    QRect itemRect(const QModelIndex &index) const;
    void updateParameters();
    void paintAlbum(QPainter *painter,
                    const QRect &rect,
                    const QModelIndex &index);
    int m_gridMinimumWidth=124;
    int m_gridWidth=124;
    int m_gridHeight=144;
    int m_spacing=13;
    int m_maxColumnCount=1;
    int m_firstVisibleIndex=0;
    int m_lineCount=0;
    int m_iconSizeParam=124;
    int m_spacingHeight=154, m_spacingWidth=134;
    int m_minGrey=0x30;
    bool m_hidingAlbum=false, m_flyingAlbum=false;
    QPalette m_palette;
    QColor m_backgroundColor;
    QTimeLine *m_scrollTimeLine;
    QPropertyAnimation *m_albumShow, *m_albumHide,
                       *m_albumThrow;
    QParallelAnimationGroup *m_flyawayGroup;
    KNMusicAlbumDetail *m_albumDetail;
    QModelIndex m_detailIndex, m_pressedIndex;
    KNMusicAlbumDetailModel *m_detailModel;
    KNMusicAlbumModel *m_model;
    KNMusicLibraryModelBase *m_musicModel;
    KNMusicCategorySortFilterModel *m_proxyModel;
};

#endif // KNMUSICALBUMVIEW_H
