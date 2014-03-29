#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QIcon>

#include "knmusicmodel.h"

#include <QStandardItemModel>

class QSplitter;
class KNMusicArtistItem;
class KNMusicCategoryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryModel(QObject *parent = 0);
    KNMusicArtistItem *artistItem(const QModelIndex &index);
    void resetModel();
    QString noCategoryText() const;
    void setNoCategoryText(const QString &noCategoryText);

signals:

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();
    void onMusicAdded(const QModelIndex &index);
    void setSourceModel(QAbstractItemModel *sourceModel);

protected:
    virtual QIcon itemIcon(const int &index) const;
    virtual QString categoryName(const int &index) const;
    KNMusicModel *m_sourceModel;

private:
    QIcon m_noAlbumArtIcon;
    QString m_noCategoryText;
};

#endif // KNMUSICCATEGORYMODEL_H
