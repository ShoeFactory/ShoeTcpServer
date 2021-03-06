﻿#include <QJsonObject>
#include "messagegps.h"
#include "shoeutilslibrary.h"

MessageGPS::MessageGPS()
{

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
    QByteArray datetimeByteArray = m_data.mid(0, 6);
    int year   = datetimeByteArray[0];
    int month  = datetimeByteArray[1];
    int day    = datetimeByteArray[2];
    int hour   = datetimeByteArray[3];
    int minute = datetimeByteArray[4];
    int second = datetimeByteArray[5];

    QDate date;
    date.setDate(year+2000, month, day);

    QTime time;
    time.setHMS(hour, minute, second);

    QDateTime datetime;
    datetime.setDate(date);
    datetime.setTime(time);

    return datetime;
}

void MessageGPS::getMessageLengthAndSatelliteCount(int &ML, int &SC)
{
    char mlsc = m_data.at(6);

    ML = (mlsc & 0xF0) >> 4;

    SC = (mlsc & 0x0F);
}

QString MessageGPS::getLongitudeString()
{
    QByteArray longitudeByteArray = m_data.mid(7, 4);

    bool ok;
    unsigned int longitude = longitudeByteArray.toHex().toUInt(&ok, 16);

    double temp = longitude / 30000.0;

    int degree = int(temp / 60);
    double smallDegree = temp -(int(temp / 60)) * 60;

    return QString::number(degree) + "°" + QString::number(smallDegree, 'f', 4) + "'";
}

quint32 MessageGPS::getLongitude()
{
    QByteArray longitudeByteArray = m_data.mid(7, 4);

    bool ok;
    unsigned int longitude = longitudeByteArray.toHex().toUInt(&ok, 16);

    return longitude;
}

QString MessageGPS::getLatitudeString()
{
    QByteArray latitudeByteArray = m_data.mid(11, 4);

    bool ok;
    unsigned int latitude = latitudeByteArray.toHex().toUInt(&ok, 16);

    double temp = latitude / 30000.0;

    int degree = int(temp / 60);
    double smallDegree = temp -(int(temp / 60)) * 60;

    return QString::number(degree) + "°" + QString::number(smallDegree, 'f', 4) + "'";
}

quint32 MessageGPS::getLatitude()
{
    QByteArray latitudeByteArray = m_data.mid(11, 4);

    bool ok;
    unsigned int latitude = latitudeByteArray.toHex().toUInt(&ok, 16);

    return latitude;
}

int MessageGPS::getSpeed()
{
    unsigned char mlsc = m_data.at(15);
    return mlsc;
}

void MessageGPS::getStatusAndDirection(QString &status, int &direction)
{
    QByteArray statusAndDirectionByteArray = m_data.right(2);

    char statusChar = statusAndDirectionByteArray.at(0);

    QString positioned   = (statusChar & 0x10) ? "在定位" : "不在定位";
    QString weastOrEast  = (statusChar & 0x08) ? "西经" : "东经";
    QString northOrSouth = (statusChar & 0x04) ? "北纬" : "南纬";

    status = positioned + " " + weastOrEast + " " + northOrSouth;

    QByteArray hex = statusAndDirectionByteArray.toHex();
    bool ok;
    short total = hex.toShort(&ok, 16);
    direction = (int)(total & 0x03FF);
}

QJsonObject MessageGPS::getjsonObject()
{
    QJsonObject object;

    // 日期
    QString dateTimeString = getDateTime().toString("yyyy-MM-dd hh:mm:ss");
    object.insert("datetime", QJsonValue(dateTimeString));

    // 消息长度 卫星个数
    int messageLength=0;
    int satelliteCount=0;
    getMessageLengthAndSatelliteCount(messageLength, satelliteCount);
    object.insert("message_length", QJsonValue(messageLength));
    object.insert("satellite_count", QJsonValue(satelliteCount));

    // 经度
    qint64 longitude = (qint64)getLongitude();
    object.insert("longitude", QJsonValue(longitude));

    // 维度
    qint64 latitude = (qint64)getLatitude();
    object.insert("latitude", QJsonValue(latitude));

    // 速度
    int speed = getSpeed();
    object.insert("speed", QJsonValue(speed));

    // 状态 航向
    QString status;
    int direction;
    getStatusAndDirection(status, direction);

    object.insert("status", QJsonValue(status));
    object.insert("direction", QJsonValue(direction));

    return object;
}

void MessageGPS::setData(const QByteArray &data)
{
    m_data = data;
}

QByteArray MessageGPS::getData() const
{
    return m_data;
}
