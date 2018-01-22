#ifndef SHOEUTILSLIBRARY_H
#define SHOEUTILSLIBRARY_H

#include <QJsonObject>

#include "shoeutilslibrary_global.h"

class SHOEUTILSLIBRARYSHARED_EXPORT ShoeUtilsLibrary
{

public:
    ShoeUtilsLibrary();

    // 输出格式带上时间
    static QString TimePrefixedString(const QString &text);
    static void    DebugTimeString(const QString &text);

    // Json对象 和 字符串之间的转换
    static QString     JsonStringFromJsonObject(QJsonObject object);
    static QJsonObject JsonObjectFromJsonString(QString string);

    // 从 JsonObject 得到 url 表单
    static QByteArray  UrlFromStrFromJsonObject(QJsonObject jsonObject);
};

#endif // SHOEUTILSLIBRARY_H
