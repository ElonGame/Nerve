#ifndef KNMUSICALBUMMODEL_H
#define KNMUSICALBUMMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicAlbumModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumModel(QObject *parent = 0);
    bool isNoAlbumHidden() const;

signals:

public slots:
    void retranslate();
    void retranslateAndSet();
    void onMusicAdded(const QModelIndex &index);
    void onMusicRemoved(const QModelIndex &index);

protected:
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;
    QString artistName(const int &index) const;

private:
    QString m_variousArtist;
};

#endif // KNMUSICALBUMMODEL_H
