#ifndef MESSAGEGPS_H
#define MESSAGEGPS_H

#include <QByteArray>
#include <QDateTime>

#include "shoemodellibrary_global.h"

/**
 * @brief GPS信息 26字节
 */
class SHOEMODELLIBRARYSHARED_EXPORT MessageGPS
{
public:
    MessageGPS();
    MessageGPS(const MessageGPS &other);
    MessageGPS(const QByteArray byteArray);

    //  6字节 时间
    QDateTime getDateTime();

    //  1字节 信息长度与参与定位的卫星数
    int getMessageLength();    //前4位
    int getSatelliteCount();   //后4位

    //  4字节 经度  x°y'       (x*60+y)*30000        = result  ==> 16进制
    int getLongitude();

    //  4字节 维度  22°32.7658 (22*60+32.7658)*30000 = 4056974 ==> 16进制
    int getlatitude();

    //  1字节 速度 公里/小时
    int getSpeed();

    //  2字节 状态、航向(332°)
    QString getStatus();      //前6位
    int     getDirection();   //后10位

    //  8字节 保留位

    //  返回json字符串
    QString getJsonString();

    //  原始数据
    QByteArray getData() const;
    void setData(const QByteArray &data);

private:
    QByteArray m_data;
};

#endif // MESSAGEGPS_H
