#include "shoetcpclient.h"
#include "shoeutilslibrary.h"

ShoeTcpClient::ShoeTcpClient(QObject *parent)
    :QObject(parent)
{

}

bool ShoeTcpClient::initialize(QHostAddress address, int port)
{
    bool bRet(false);

    do
    {
        m_socket = new ShoeTcpSocket(this);

        connect(m_socket, SIGNAL(onConnected(int)), this, SIGNAL(onConnected()));
        connect(m_socket, SIGNAL(onDisonnected(int)), this, SIGNAL(onDisconnected()));
        connect(m_socket, SIGNAL(onPacketReceived(Packet, int)), this, SIGNAL(onPacketReceived(Packet)));
        connect(m_socket, SIGNAL(onPacketSend(Packet,int)), this, SIGNAL(onPacketSend(Packet)));
        connect(m_socket, SIGNAL(onErrorOccurred(QString, int)), this, SIGNAL(onErrorOccurred(QString)));

        bool bConnected(false);

        for (int i=0; i<CONNECT_TRY_TIMES; i++)
        {
            // 与Tcp服务器连接，连接成功后发出connected信号
            m_socket->connectToHost(address, port);

            if (!m_socket->waitForConnected(WAIT_CONNECT_TIMEOUT))
            {
                ShoeUtilsLibrary::DebugTimeString("wait for connect timeout");
                continue;
            }
            else
            {
                bConnected = true;
                break;
            }

        }

        if (!bConnected)
        {
            break;
        }

        bRet = true;
    } while(false);

    return bRet;
}

bool ShoeTcpClient::finalize()
{
    bool bRet(false);

    do
    {
        m_socket->disconnectFromHost();

        bRet = (m_socket->state() == QAbstractSocket::UnconnectedState ||
                m_socket->waitForDisconnected());
    } while(false);

    return bRet;
}

bool ShoeTcpClient::sendPacket(const Packet &packet)
{
    return m_socket->sendPacket(packet);
}

bool ShoeTcpClient::sendData(const QByteArray &data)
{
    return m_socket->sendData(data);
}
