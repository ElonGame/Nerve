#ifndef KNMUSICPLAYLISTLISTVIEW_H
#define KNMUSICPLAYLISTLISTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVariant>
#include <QList>
#include <QScrollArea>
#include <QLineEdit>
#include <QLinearGradient>

class QBoxLayout;
class QResizeEvent;
class QSignalMapper;
class QEvent;
class QPropertyAnimation;
class QTimeLine;
class KNMusicPlaylistItemEditor : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistItemEditor(QWidget *parent = 0);

signals:
    void cancelEdit();
    void ensureEdit();

protected:
    void keyReleaseEvent(QKeyEvent *event);
};

class KNMusicPlaylistListviewItem : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListviewItem(QWidget *parent = 0);
    void setText(const QString &text);
    void setData(const QVariant &data);
    void setCreateMode();
    void startCreateMode();
    void select();
    void unselect();
    QVariant data() const;

signals:
    void requireDelete();
    void requireAdd(const QString &text);
    void itemClicked();

public slots:
    void setTextColorParam(const int &frame);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionReheight(const int &itemHeight);
    void onActionCancelEdit();
    void onActionEnsureEdit();
    void onActionCreateFinished();
    void onActionAnimeDelete();

private:
    QBoxLayout *m_layout;
    QPalette m_palette, m_textPalette;
    KNMusicPlaylistItemEditor *m_editor;
    QVariant m_data;
    QLabel *m_icon, *m_text;
    QColor m_originalTextColor;
    QPropertyAnimation *m_editorInAnime;
    QTimeLine *m_createAnime, *m_deleteAnime;
    int m_itemHeight=30;
    bool m_isPressed=false, m_createMode=false, m_unselected=true;
};

class KNMusicPlaylistListviewHeader : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListviewHeader(QWidget *parent = 0);
    ~KNMusicPlaylistListviewHeader();
    int addItem(KNMusicPlaylistListviewItem *item);
    void removeItem(const int &index);
    void clear();
    void setStartParam(const int &param);
    void setItemLayout(QBoxLayout *itemLayout);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionItemClicked();

private:
    QPalette m_palette;
    QBoxLayout *m_itemLayout;
    QLinearGradient m_background;
    int m_currentIndex=-1;
    QList<KNMusicPlaylistListviewItem *> m_items;
};

class KNMusicPlaylistListviewContent : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListviewContent(QWidget *parent = 0);
    ~KNMusicPlaylistListviewContent();
    int addHeader(const QString &text);
    int addItem(const QString &text);
    void addCreatePlaylist();
    void clear();
    void clearHeader();
    bool setCurrentHeader(const int &index);

signals:
    void requireSetHeaderColorParam(const int &param);
    void requireSetTextColorParam(const int &param);
    void requireAddPlaylist(const QString &title);

public slots:
    void setHeaderColorParam(const int &param);
    void setTextColorParam(const int &param);

private slots:
    void createCancel();
    void createApply(const QString &caption);

private:
    void disconnectCreatedItem();
    int m_createdIndex=-1;
    KNMusicPlaylistListviewHeader *m_currentHeader;
    KNMusicPlaylistListviewItem *m_createdItem=nullptr;
    QList<KNMusicPlaylistListviewHeader *> m_headers;
    QBoxLayout *m_mainLayout;
    QSignalMapper *m_clickedMapper;
};

class KNMusicPlaylistListview : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListview(QWidget *parent = 0);
    int addHeader(const QString &text);
    int addItem(const QString &text);
    void clearHeader();
    bool setCurrentHeader(const int &index);

signals:
    void requireAddPlaylist(const QString &title);

public slots:
    void createPlaylist();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionChangeBackground(const int &frame);

private:
    KNMusicPlaylistListviewContent *m_content;
    QPalette m_palette;
    QColor m_baseColor=QColor(0,0,0);
    int m_textBaseParam=0x70;
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNMUSICPLAYLISTLISTVIEW_H