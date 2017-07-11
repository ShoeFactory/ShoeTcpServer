#ifndef MESSAGELOGIN_H
#define MESSAGELOGIN_H

#include <QByteArray>
#include "shoemodellibrary_global.h"

class SHOEMODELLIBRARYSHARED_EXPORT MessageLogin
{
public:
    MessageLogin();
    MessageLogin(const MessageLogin &other);
    MessageLogin(const QByteArray byteArray);

    //  8字节
    QString getIMEI();

    void removeIMEI();//用于服务器回复客户端登录请求

    QByteArray getData() const;
    void setData(const QByteArray &data);

private:
    QByteArray m_data;
};
#endif // MESSAGELOGIN_H
