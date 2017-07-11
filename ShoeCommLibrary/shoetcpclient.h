#ifndef SHOETCPCLIENT_H
#define SHOETCPCLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>

#include "shoecommlibrary_global.h"
#include "shoecommdefination.h"

#include "shoetcpsocket.h"

class SHOECOMMLIBRARYSHARED_EXPORT ShoeTcpClient : public QObject
{
    Q_OBJECT
public:
    ShoeTcpClient(QObject * parent=NULL);
    bool initialize(QHostAddress address, int port);
    bool finalize();

    bool sendPacket(const Packet& packet);

signals:
    void onConnected();
    void onDisconnected();
    void onPacketSend(const Packet& Packet);
    void onPacketReceived(const Packet& packet);
    void onErrorOccurred(const QString& errorString);

private:
    ShoeTcpSocket* m_socket;
};

#endif // SHOETCPCLIENT_H
