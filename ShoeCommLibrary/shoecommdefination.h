#ifndef SHOECOMMDEFINATION_H
#define SHOECOMMDEFINATION_H

#include <QByteArray>
#include <QVariant>
#include <QDataStream>
#include <QDateTime>

/**
 * 数据包类型
 */
typedef enum MsgType
{
    PacketType_Login = 0,       // 登录
    PacketType_GPS,             // GPS
    PacketType_status,          // 状态
    PacketType_string,          // 字符串
    PacketType_GPSStatus,       // GPS与状态
    PacketType_Server,          // 服务器发给定位器
    PacketType_Count
}MsgType;

/**
 * 数据包 上次运用
 */
typedef struct Packet
{
    MsgType    msgType;         // 包类型
    QByteArray msgContent;      // 包内容为特定字节
    quint16    msgNumber;       // 包序号
}Packet;
Q_DECLARE_METATYPE(Packet)

/**
 * @brief 链接时参数
 */
enum
{
    WAIT_CONNECT_TIMEOUT = 3000, /**< 连接服务器超时时间 */
    CONNECT_TRY_TIMES = 1,       /**< 连接尝试次数 */
};

#endif // SHOECOMMDEFINATION_H
