#include "shoecommdefination.h"

QStringList MsgTypeStringList =
        QStringList()  << "登录    0x01"
                       << "禁止    0x44"
                       << "心跳    0x08"
                       << "Wifi   0x69"
                       << "定位    0x17"
                       << "更新时间 0x30"
                       << "同步设置 0x57"
                       << "GPS     0x10"

                       << "状态    0x13"
                       << "字符    0x15"
                       << "合并    0x16"
                       << "服务    0x80"

                       << "错误    0x00";

QString MsgTypeString(const MsgType &type)
{
    return MsgTypeStringList.value(type);
}
