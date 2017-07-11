#include <QString>
#include "messagelogin.h"

MessageLogin::MessageLogin()
{
    QByteArray IMEI;
    IMEI = IMEI.fromHex("0123456789012345");

    setData(IMEI);
}

MessageLogin::MessageLogin(const MessageLogin &other)
{
    m_data = other.m_data;
}

MessageLogin::MessageLogin(const QByteArray byteArray)
{
    m_data = byteArray;
}

QString MessageLogin::getIMEI()
{
    return getData().toHex();
}

void MessageLogin::removeIMEI()
{
    QByteArray IMEI;
    IMEI.clear();
    setData(IMEI);
}

QByteArray MessageLogin::getData() const
{
    return m_data;
}

void MessageLogin::setData(const QByteArray &data)
{
    m_data = data;
}
