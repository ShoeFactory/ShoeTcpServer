#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "shoehttpaccesss.h"
#include "shoeutilslibrary.h"

ShoeHttpAccesss * ShoeHttpAccesss::httpAccess = NULL;

//QString api_common = "http://www.shakeda.com:5555/api/v1.0" ;
QString api_common = "http://127.0.0.1:5555/api/v1.0" ;

QString api_gps = "/position/add";

ShoeHttpAccesss::ShoeHttpAccesss()
{
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(receivedReply(QNetworkReply*)));
}

ShoeHttpAccesss *ShoeHttpAccesss::getInstance()
{
    if(httpAccess == NULL)
        httpAccess = new ShoeHttpAccesss();
    return httpAccess;
}

void ShoeHttpAccesss::postGPS(QJsonObject GPS_JsonPara)
{
    postRequest(api_common + api_gps, GPS_JsonPara);
}

void ShoeHttpAccesss::getRequest(QString url, QJsonObject jsonParas)
{
    Q_UNUSED(url);
    Q_UNUSED(jsonParas);
}

void ShoeHttpAccesss::postRequest(QString url, QJsonObject jsonParas)
{
    QNetworkRequest *request = new QNetworkRequest();

    // 设置头
    request->setUrl(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    //JsonString转换成 FormData
    QByteArray postData =  ShoeUtilsLibrary::UrlFromStrFromJsonObject(jsonParas);

    // 发送
    post(*request,postData);
}

void ShoeHttpAccesss::receivedReply(QNetworkReply *reply)
{
    if (reply->error())
    {
        ShoeUtilsLibrary::DebugTimeString("http request error:" + reply->errorString());
    }
    else
    {
        QByteArray receivedData = reply->readAll();
        ShoeUtilsLibrary::DebugTimeString("http request receive:" + QString(receivedData));
    }
    reply->deleteLater();
}

