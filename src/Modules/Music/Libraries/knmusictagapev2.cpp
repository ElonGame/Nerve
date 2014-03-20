#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagapev2.h"

KNMusicTagAPEv2::KNMusicTagAPEv2(QObject *parent) :
    KNMusicTagBase(parent)
{
}

QString KNMusicTagAPEv2::tagStringData(const QString &frameKey) const
{
    return QString::fromUtf8(m_frameDatas[frameKey]).simplified();
}

bool KNMusicTagAPEv2::readTag(const QString &filePath)
{
    clearCache();
    QFile mediaFile(filePath);
    if(mediaFile.size()<32)
    {
        //A file even can't contains a header.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    m_headerPosition=0;
    if(checkAPEHeaderAt(m_headerPosition, mediaData))
    {
        return true;
    }
    m_headerPosition=mediaFile.size()-32;
    if(checkAPEHeaderAt(m_headerPosition, mediaData))
    {
        return readTagAt(m_headerPosition, mediaData);
    }
    return false;
}

void KNMusicTagAPEv2::clearCache()
{
    memset(m_apeHeader, 0, 32);
}

bool KNMusicTagAPEv2::checkAPEHeaderAt(int position,
                                       QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(position);
    mediaData.readRawData(m_apeHeader, 32);
    m_preambleCheck[8]='\0';
    memcpy(m_preambleCheck, m_apeHeader, 8);
    bool headerExsist=(strcmp(m_preambleCheck, m_apePreamble)==0);
    if(headerExsist)
    {
        m_versionNumber=((((quint32)m_apeHeader[11])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[10])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[9]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[8])&0b00000000000000000000000011111111);
        m_tagSize=((((quint32)m_apeHeader[15])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[14])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[13]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[12])&0b00000000000000000000000011111111);
        m_itemCount=((((quint32)m_apeHeader[19])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[18])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[17]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[16])&0b00000000000000000000000011111111);
        m_tagsFlags=((((quint32)m_apeHeader[23])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[22])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[21]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[20])&0b00000000000000000000000011111111);
    }
    return headerExsist;
}

bool KNMusicTagAPEv2::readTagAt(int position, QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(position-m_tagSize+32);
    char *rawTagData=new char[m_tagSize];
    mediaData.readRawData(rawTagData, m_tagSize);

    //All process code here.
    quint32 currentPosition=0;
    quint32 currentFrameSize, currentFlag;
    int labelStart, labelEnd, labelLength;
    for(quint32 i=0; i<m_itemCount; i++)
    {
        currentFrameSize=((((quint32)rawTagData[currentPosition+3])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[currentPosition+2])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)rawTagData[currentPosition+1]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)rawTagData[currentPosition])&0b00000000000000000000000011111111);
        currentFlag=((((quint32)rawTagData[currentPosition+7])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[currentPosition+6])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)rawTagData[currentPosition+5]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)rawTagData[currentPosition+4])&0b00000000000000000000000011111111);
        labelStart=currentPosition+8;
        labelEnd=labelStart;
        while(rawTagData[labelEnd]!=0)
        {
            labelEnd++;
        }
        labelLength=labelEnd-labelStart+1;
        char *frameLabel=new char[labelLength+1];
        memcpy(frameLabel, rawTagData+labelStart, labelLength);
        char *rawFrameData=new char[currentFrameSize+1];
        memset(rawFrameData, 0, sizeof(char)*currentFrameSize);
        memcpy(rawFrameData, rawTagData+labelEnd+1, currentFrameSize);
        QByteArray frameData;
        frameData.append(rawFrameData);
        m_frameDatas[frameLabel]=rawFrameData;
        delete[] rawFrameData;
        delete[] frameLabel;
        currentPosition=labelEnd+currentFrameSize+1;
    }
    //All process code above.

    delete[] rawTagData; //Don't touch this.
    return true;
}