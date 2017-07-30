#include <QHostAddress>
#include "shoetcpserver.h"

ShoeTcpServer::ShoeTcpServer(QObject *parent)
    : QTcpServer(parent)
{

}

bool ShoeTcpServer::initialize(int port)
{
    bool bRet(false);

    do
    {
        // 在指定的端口对任意地址进行监听
        if (!listen(QHostAddress::Any, port))
        {
            break;
        }

        bRet = true;
        emit onListeningStatus(true);
    } while(false);

    return bRet;
}

bool ShoeTcpServer::finalize()
{
    if ( isListening() )
    {
        // 不在监听新连接
        close();

        // 关闭现有的连接
        foreach (ShoeTcpSocket *socket, m_socketList)
        {
            socket->close();
        }

        emit onListeningStatus(false);
    }
    return true;
}

void ShoeTcpServer::incomingConnection(qintptr nSocketDescriptor)
{
    // 创建一个新的Socket与客户端通信
    ShoeTcpSocket* socket = new ShoeTcpSocket(this);
    socket->setSocketID(nSocketDescriptor);
    socket->setSocketDescriptor(nSocketDescriptor);

    connect(socket, SIGNAL(onConnected(int)), this, SIGNAL(onConnected(int)));
    connect(socket, SIGNAL(onDisonnected(int)), this, SLOT(slotDeleteDisconnected(int)));
    connect(socket, SIGNAL(onPacketSend(Packet,int)), this, SIGNAL(onPacketSend(Packet, int)));
    connect(socket, SIGNAL(onPacketReceived(Packet, int)), this, SIGNAL(onPacketReceived(Packet,int)));
    connect(socket, SIGNAL(onErrorOccurred(QString, int)), this, SIGNAL(onErrorOccurred(QString,int)));

    // 高调显示 新的连接产生了
    emit socket->onConnected(nSocketDescriptor);

    // 加入客户端套接字列表以便管理
    m_socketList.append(socket);
}

void ShoeTcpServer::slotDeleteDisconnected(const int nDescriptor)
{
    ShoeTcpSocket* socket = qobject_cast<ShoeTcpSocket*>(sender());

    if(socket)
    {
        m_socketList.removeOne(socket);
        socket->deleteLater();
    }

    emit onDisconnected(nDescriptor);
}

bool ShoeTcpServer::sendPacket(const Packet &packet, const int nDescriptor)
{
    QList<ShoeTcpSocket*>::iterator it;

    for(it=m_socketList.begin(); it!=m_socketList.end(); it++)
    {
        ShoeTcpSocket* item = *it;

        if ( nDescriptor == item->socketDescriptor())
        {
            return item->sendPacket(packet);
        }
    }

    return false;
}
