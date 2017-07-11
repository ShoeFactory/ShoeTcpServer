#ifndef SHOETCPSERVER_H
#define SHOETCPSERVER_H

#include <QTcpServer>
#include <QObject>

#include "shoecommlibrary_global.h"
#include "shoecommdefination.h"
#include "shoetcpsocket.h"

class SHOECOMMLIBRARYSHARED_EXPORT ShoeTcpServer:public QTcpServer
{
    Q_OBJECT
public:
    ShoeTcpServer(QObject* parent=NULL);
    bool initialize(int port);
    bool finalize();
    bool sendPacket(const Packet& packet, const int nDescriptor);

signals:
    void onListeningStatus(bool isListening);
    void onConnected(const int nDescriptor);
    void onDisconnected(const int nDescriptor);
    void onPacketSend(const Packet& packet, const int nDescriptor);
    void onPacketReceived(const Packet& packet, const int nDescriptor);
    void onErrorOccurred(const QString& errorString, const int nDescriptor);

protected:
    void incomingConnection(qintptr nSocketDescriptor) override;

private:
    /**< 客户Socket管理链表 */
    QList<ShoeTcpSocket*> m_socketList;

private slots:
    void slotDeleteDisconnected(const int nDescriptor);
};

#endif // SHOETCPSERVER_H
