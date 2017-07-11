#include "messagegps.h"

MessageGPS::MessageGPS()
{
    QByteArray content;
    content = content.fromHex("0A03170F32179C026B3F3E0C22AD651F3460");
    setData(content);
}

MessageGPS::MessageGPS(const MessageGPS &other)
{
    m_data = other.m_data;
}

MessageGPS::MessageGPS(const QByteArray byteArray)
{
    m_data = byteArray;
}

QDateTime MessageGPS::getDateTime()
{
    return QDateTime();
}

int MessageGPS::getMessageLength()
{
    return 0;
}

int MessageGPS::getSatelliteCount()
{
    return 0;
}

int MessageGPS::getLongitude()
{
    return 0;
}

int MessageGPS::getlatitude()
{
    return 0;
}

int MessageGPS::getSpeed()
{
    return 0;
}

QString MessageGPS::getStatus()
{
    return QString();
}

int MessageGPS::getDirection()
{
    return 0;
}

QString MessageGPS::getJsonString()
{
    return QString("{}");
}

void MessageGPS::setData(const QByteArray &data)
{
    m_data = data;
}

QByteArray MessageGPS::getData() const
{
    return m_data;
}
