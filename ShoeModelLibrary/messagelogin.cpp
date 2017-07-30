#include <QString>
#include "messagelogin.h"

MessageLogin::MessageLogin()
{

}

MessageLogin::MessageLogin(const MessageLogin &other)
{
    m_imei = other.m_imei;
    m_version = other.m_version;
}

QString MessageLogin::getIMEI()
{
    return m_imei.toHex();
}

QByteArray MessageLogin::getData() const
{
    QByteArray result;

    result.append(m_imei);
    result.append(m_version);

    return result;
}

void MessageLogin::setData(QByteArray data)
{
    m_imei.clear();
    m_imei = data.left(8);

    m_version.clear();
    m_version=data.right(1);
}

void MessageLogin::setIMEI(QByteArray imei)
{
    m_imei = imei;
}

void MessageLogin::setVersion(QByteArray version)
{
    m_version = version;
}
