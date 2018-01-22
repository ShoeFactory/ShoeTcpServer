#ifndef SHOECOMMDEFINATION_H
#define SHOECOMMDEFINATION_H

#include <QByteArray>
#include <QVariant>
#include <QDataStream>
#include <QDateTime>
#include "shoecommlibrary_global.h"
/**
 * 数据包类型
 *
 * 数据包格式修改 牵扯：
 * 1. wrapPacket
 * 2. extratPacket
 *
 * 3. receiveRule
 * 4. sendRule
 *
 */

typedef enum MsgType
{
    PacketType_Login = 0,       // 登录
    PacketType_Login_Disallow,  // 禁止
    PacketType_HearBeat,        // 心跳包
    PacketType_WifiPosition,    // wifi定位
    PacketType_Position,        // 定位
    PacketType_UpdateTime,      // 更新时间
    PacektType_SyncSetting,     // 同步设置
    PacketType_GPS,             // GPS

    PacketType_status,          // 状态
    PacketType_string,          // 字符串
    PacketType_GPSStatus,       // GPS与状态
    PacketType_Server,          // 服务器发给定位器

    PacketType_Count
}MsgType;
extern QStringList MsgTypeStringList;
SHOECOMMLIBRARYSHARED_EXPORT QString MsgTypeString(const MsgType &type);

/**
 * 数据包 上层运用
 */
typedef struct Packet
{
    MsgType    msgType;         // 包类型
    QByteArray msgContent;      // 包内容为特定字节
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
