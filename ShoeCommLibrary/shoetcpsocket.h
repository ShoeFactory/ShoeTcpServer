#ifndef SHOETCPSOCKET_H
#define SHOETCPSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include "shoecommdefination.h"

class ShoeTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    ShoeTcpSocket(QObject *parent=NULL);

public:
    void setSocketID(const int nDescriptor);
    bool sendPacket(const Packet& packet);
    bool sendData(const QByteArray &data);

    // 发送规则
    bool sendRule(const Packet& packet);
    bool sendRule(const QByteArray &data);

    // 解析规则
    void receiveRule(QByteArray &recvBuffer);

signals:
    void onConnected(const int nDescriptor);
    void onDisonnected(const int nDescriptor);
    void onPacketSend(Packet, const int nDescriptor);
    void onPacketReceived(Packet, const int nDescriptor);
    void onErrorOccurred(QString, const int nDescriptor);

private:
    QByteArray recvBuffer;
    int socketID;

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotDataReceived();
    void slotErrorOccurred(QAbstractSocket::SocketError error);
};

#endif // SHOETCPSOCKET_H
