#ifndef KNMUSICALBUMMODEL_H
#define KNMUSICALBUMMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicAlbumModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumModel(QObject *parent = 0);
    bool isNoAlbumHidden() const;
    void resetModel();
    QString indexArtist(const QModelIndex &index) const;

signals:
    void albumRemoved(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();
    void onMusicAdded(const QModelIndex &index);
    void onMusicRemoved(const QModelIndex &index);
    void onMusicRecover(const QModelIndex &index);

protected:
    void updateImage(const int &index);
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;
    QString artistName(const int &index) const;
    void onActionRemoveRow(const int &index);

private:
    struct AlbumExtras
    {
        QString artist;
        bool variousArtist;
    };
    QList<AlbumExtras> m_extraList;
    QString m_variousArtist;
};

#endif // KNMUSICALBUMMODEL_H
