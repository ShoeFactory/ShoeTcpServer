#ifndef SHOEHTTPACCESSS_H
#define SHOEHTTPACCESSS_H

#include <QNetworkAccessManager>
#include <QJsonObject>

class ShoeHttpAccesss: public QNetworkAccessManager
{
    Q_OBJECT
public:
    static ShoeHttpAccesss *getInstance();

    void postGPS(QJsonObject GPS_JsonPara);


    //状态刷新 用异步
    void getRequest(QString url, QJsonObject jsonParas = QJsonObject());
    void postRequest(QString url, QJsonObject jsonParas = QJsonObject());

private slots:
    void receivedReply(QNetworkReply *reply);

private:
    ShoeHttpAccesss();
    static ShoeHttpAccesss *httpAccess;
};

#define ShoeHttpAccessor ShoeHttpAccesss::getInstance()
#endif // SHOEHTTPACCESSS_H
