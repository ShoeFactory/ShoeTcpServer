#ifndef MESSAGELOGIN_H
#define MESSAGELOGIN_H

#include <QByteArray>
#include "shoemodellibrary_global.h"

class SHOEMODELLIBRARYSHARED_EXPORT MessageLogin
{
public:

    MessageLogin();
    MessageLogin(const MessageLogin &other);

    // 留作以后的标识
    QString getIMEI();

    // 返回核心
    QByteArray getData() const;
    void setData(QByteArray data);

    void setIMEI(QByteArray imei);
    void setVersion(QByteArray version);

private:
    QByteArray m_imei;
    QByteArray m_version;
};
#endif // MESSAGELOGIN_H
