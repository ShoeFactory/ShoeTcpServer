#ifndef SHOEHELPER_H
#define SHOEHELPER_H

#include "shoecommlibrary_global.h"
#include "shoecommdefination.h"

class SHOECOMMLIBRARYSHARED_EXPORT ShoeHelper
{
public:

    /**
     * 调试窗口中输出带时间戳的信息
     */
    static void qDebugString(const QString& text)
    {
        QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
        qDebug()<< strTime << text;
    }

    /**
     * 输出带时间戳的信息
     */
    static QString timePrefixedString(const QString &text)
    {
        return QDateTime::currentDateTime().toString("hh:mm:ss.zzz ") + text;
    }

    static QString msgTypeString(const MsgType &type)
    {
        QStringList msgTypeList;

        msgTypeList << "登录 0x01"
                    << "GPS 0x12"
                    << "状态 0x13"
                    << "字符 0x15"
                    << "合并 0x16"
                    << "服务 0x80"
                    << "错误 0x00";

      return msgTypeList.value(type);
    }
};

#endif // SHOEHELPER_H
