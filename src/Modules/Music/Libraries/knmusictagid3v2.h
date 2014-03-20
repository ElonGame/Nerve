#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QMap>
#include <QList>
#include <QByteArray>
#include <QPixmap>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagID3v2 : public KNMusicTagBase
{
    Q_OBJECT
public:
    struct ID3v2Data
    {
        //Version
        int version;
        int revision;
        //Flags
        bool unsynchronisation;
        bool extendedHeader;
        bool experimentalIndicator;
        //Frames
        QStringList frameID;
        QList<QByteArray> frameData;
    };

    struct ID3v2Image
    {
        QString description;
        QPixmap image;
    };

    explicit KNMusicTagID3v2(QObject *parent = 0);
    QString fromID3v2String(const QByteArray &value);
    QString id3v2String(const QString &frameID);
    int version();

    bool readTag(const QString &filePath);

    QPixmap tagImage(const int &index) const;
    QPixmap firstAvaliableImage() const;

signals:

public slots:

private:
    void processAPIC(const QByteArray &value);
    QMap<int, ID3v2Image> m_tagImages;
    ID3v2Data m_tagData;
    QTextCodec *m_beCodec, *m_leCodec, *m_isoCodec, *m_windowsCodec;
};

#endif // KNMUSICTAGID3V2_H