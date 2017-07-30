#include <QJsonDocument>
#include <QDateTime>
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>

#include "shoeutilslibrary.h"


ShoeUtilsLibrary::ShoeUtilsLibrary()
{

}

QString ShoeUtilsLibrary::TimePrefixedString(const QString &text)
{
    return QDateTime::currentDateTime().toString("hh:mm:ss.zzz ") + text;
}

void ShoeUtilsLibrary::DebugTimeString(const QString& text)
{
    qDebug()<< TimePrefixedString(text);
}

QString ShoeUtilsLibrary::JsonStringFromJsonObject(QJsonObject object){
    // 先得到document
    QJsonDocument doc(object);

    // 得出字节流
    QByteArray byteArray;
    byteArray = doc.toJson(QJsonDocument::Compact);

    // 返回字符串
    QString str;
    str.append(byteArray);
    return str;
}

QJsonObject ShoeUtilsLibrary::JsonObjectFromJsonString(QString string){
    // 先得到字节流
    QByteArray byteArray;
    byteArray.append(string);

    // 转成document
    QJsonDocument doc;
    doc = doc.fromJson(byteArray);

    // 返回jsonobject
    return doc.object();
}

QByteArray ShoeUtilsLibrary::UrlFromStrFromJsonObject(QJsonObject jsonObject)
{
    // 利用query做中间人, 把JsonObject转换成 FormData
    QUrlQuery query;

    // 遍历JsonObject, 将键值对插入query中
    QJsonObject::const_iterator it;
    for(it=jsonObject.begin(); it!=jsonObject.end();it++)
    {
        query.addQueryItem(it.key(), it.value().toString());
    }

    return query.toString(QUrl::FullyEncoded).toUtf8();
}
