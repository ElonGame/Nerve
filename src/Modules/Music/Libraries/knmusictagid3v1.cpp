#include <QTextCodec>

#include <QDebug>

#include <cstdlib>

#include "../../knglobal.h"
#include "../knmusicglobal.h"

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagBase(parent)
{
    memset(m_emptyTag, 0, 128);
    m_emptyTag[0]='T';m_emptyTag[1]='A';m_emptyTag[2]='G';
    m_emptyTag[127]=0xff;
    m_localCodec=KNGlobal::instance()->codecForCurrentLocale();
}

bool KNMusicTagID3v1::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    //Get raw tag data
    int mediaFileSize=mediaFile.size();
    if(mediaFileSize<128)
    {
        clearCache();
        //If the size is less than 128, it can't contains ID3v1.
        return false;
    }
    //Read the last 128 byte data.
    mediaData.skipRawData(mediaFileSize-128);
    mediaData.readRawData(m_rawTagData, 128);

    //Check the header, if header is not 'TAG', return false.
    if(m_rawTagData[0]!='T' || m_rawTagData[1]!='A' || m_rawTagData[2]!='G')
    {
        return false;
    }
    m_rawByteBackup=m_rawTagData[33];
    m_rawTagData[33]=0;
    m_tagData[Title]=m_localCodec->toUnicode(m_rawTagData+3).simplified();
    m_rawTagData[33]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[63];
    m_rawTagData[63]=0;
    m_tagData[Artist]=m_localCodec->toUnicode(m_rawTagData+33).simplified();
    m_rawTagData[63]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[93];
    m_rawTagData[93]=0;
    m_tagData[Album]=m_localCodec->toUnicode(m_rawTagData+63).simplified();
    m_rawTagData[93]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[97];
    m_rawTagData[97]=0;
    m_tagData[Year]=m_localCodec->toUnicode(m_rawTagData+93).simplified();
    m_rawTagData[97]=m_rawByteBackup;
    if(m_rawTagData[125]==0)
    {
        m_tagData[Track]=QString::number((quint8)m_rawTagData[126]);
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawTagData+97).simplified();
    }
    else
    {
        m_rawByteBackup=m_rawTagData[127];
        m_rawTagData[127]=0;
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawTagData+97, 30).simplified();
        m_rawTagData[127]=m_rawByteBackup;
    }
    m_genreIndex=(quint8)m_rawTagData[127];
    m_tagData[Genre]=KNMusicGlobal::instance()->getGenre(m_genreIndex);
    return true;
}

void KNMusicTagID3v1::clearCache()
{
    m_tagData[Title].clear();
    m_tagData[Artist].clear();
    m_tagData[Album].clear();
    m_tagData[Year].clear();
    m_tagData[Comment].clear();
    m_tagData[Track].clear();
    m_tagData[Genre].clear();
}

QString KNMusicTagID3v1::textData(const int &key) const
{
    return m_tagData[key];
}

void KNMusicTagID3v1::setTextData(const int &key, const QString &data)
{
    m_tagData[key]=data;
}

void KNMusicTagID3v1::writeTag(QFile &mediaFile, QDataStream &mediaData)
{
    memset(m_rawTagData, 0, 128);
    m_rawTagData[0]='T';
    m_rawTagData[1]='A';
    m_rawTagData[2]='G';
    strncpy(m_rawTagData+3, m_localCodec->fromUnicode(m_tagData[Title]).data(), 30);
    strncpy(m_rawTagData+33, m_localCodec->fromUnicode(m_tagData[Artist]).data(), 30);
    strncpy(m_rawTagData+63, m_localCodec->fromUnicode(m_tagData[Album]).data(), 30);
    strncpy(m_rawTagData+93, m_localCodec->fromUnicode(m_tagData[Year]).data(), 4);
    int trackTest=m_tagData[Track].toInt();
    if(trackTest>0 && trackTest<255)
    {
        m_rawTagData[125]=0;
        m_rawTagData[126]=trackTest;
        strncpy(m_rawTagData+97, m_localCodec->fromUnicode(m_tagData[Comment]).data(), 28);
    }
    else
    {
        strncpy(m_rawTagData+97, m_localCodec->fromUnicode(m_tagData[Comment]).data(), 30);
    }
    int genreTest=KNMusicGlobal::instance()->genreIndex(m_tagData[Genre]);
    if(genreTest==-1)
    {
        m_rawTagData[127]=0xff;
    }
    else
    {
        m_rawTagData[127]=genreTest;
    }
    bool isTagEmpty=(memcmp(m_rawTagData, m_emptyTag, 127)==0);
    int mediaFileSize=mediaFile.size();
    if(mediaFileSize>128)
    {
        //If the file size is greater than 128, it might contains ID3v1.
        mediaData.skipRawData(mediaFileSize-128);
        //Read the header
        char header[3];
        mediaData.readRawData(header, 3);
        //If header is not 'TAG', then set the position to the end of file.
        if(header[0]!='T' || header[1]!='A' || header[2]!='G')
        {
            if(isTagEmpty)
            {
                return;
            }
            mediaData.skipRawData(125);
        }
        else
        {
            //Find ID3v1 file.
            //If new tag is empty, that means we need to remove the original tag.
            if(isTagEmpty)
            {
                mediaFile.resize(mediaFileSize-128);
                return;
            }
            mediaFile.reset();
            mediaData.skipRawData(mediaFileSize-128);
        }
    }
    else
    {
        if(isTagEmpty)
        {
            return;
        }
        mediaData.skipRawData(mediaFileSize);
    }
    mediaData.writeRawData(m_rawTagData, 128);
}

quint8 KNMusicTagID3v1::genreIndex() const
{
    return m_genreIndex;
}
