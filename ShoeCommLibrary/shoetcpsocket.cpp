#include "shoetcpsocket.h"
#include "shoeutilslibrary.h"

/*Packet == WrappedPacket 之间类型的转换*/
QList<qint8> PacketTypeList = QList<qint8>() << 0x01 << 0x44 << 0x08 << 0x69 << 0x17 << 0x30 << 0x57 << 0x10
                                             << 0x13 << 0x15 << 0x16 << 0x80 << 0x00;
static int HexToEnumOfMsgType(qint8 type)
{
    return PacketTypeList.indexOf(type);
}

static MsgType EnumToHexOfMsgType(int index)
{
    return (MsgType)PacketTypeList.value(index);
}

typedef struct WrappedPacket
{
    qint16      start;          //  起始位   2
    quint8      length;         //  长度     1
    qint8       msgType;        //  包类型   1
    QByteArray  msgContent;     //  包内容   N
    qint16      stop;           //  停止位   2
}WrappedPacket;
Q_DECLARE_METATYPE(WrappedPacket)

static QDataStream& operator << (QDataStream& stream, const WrappedPacket& packet)
{
    stream << packet.start;

    stream << packet.length;
    stream << packet.msgType;

    int contentLength = packet.msgContent.size();
    for(int i=0; i<contentLength; i++)
    {
        qint8 temp =  packet.msgContent.at(i);
        stream << temp;
    }

    stream << packet.stop;
    return stream;
}

static QDataStream& operator >> (QDataStream& stream, WrappedPacket& packet)
{
    stream >> packet.start;
    stream >> packet.length;

    // 协议号
    stream >> packet.msgType;

    // 内容长度 = 长度 - 协议号长度
    int contentLength = packet.length-1;
    QByteArray content;
    content.resize(contentLength);
    for(int i=0; i<contentLength; i++)
    {
        qint8 temp;
        stream >> temp;
        content[i] = temp;
    }
    packet.msgContent = content;

    stream >> packet.stop;
    return stream;
}

static WrappedPacket wrapPacket(const Packet& packet)
{
    WrappedPacket result;

    result.start = 0x7878;
    result.stop  = 0x0D0A;

    result.msgType    = EnumToHexOfMsgType(packet.msgType);
    result.msgContent = packet.msgContent;

    // 长度 = 协议号+内容 协议号一般是1个字节
    result.length = (quint8) (result.msgContent.size() + 1);

    return result;
}

static Packet extractPacket(const WrappedPacket& wrappedPacket)
{
    Packet result;
    result.msgContent = wrappedPacket.msgContent;
    result.msgType = (MsgType)HexToEnumOfMsgType(wrappedPacket.msgType);
    return result;
}


ShoeTcpSocket::ShoeTcpSocket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this, &QAbstractSocket::connected, this, &ShoeTcpSocket::slotConnected);
    connect(this, &QAbstractSocket::disconnected, this, &ShoeTcpSocket::slotDisconnected);
    connect(this, &QIODevice::readyRead, this, &ShoeTcpSocket::slotDataReceived);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotErrorOccurred(QAbstractSocket::SocketError)));
}

void ShoeTcpSocket::setSocketID(const int nDescriptor)
{
    this->socketID = nDescriptor;
}

bool ShoeTcpSocket::sendPacket(const Packet &packet)
{
    return sendRule(packet);
}

bool ShoeTcpSocket::sendRule(const Packet &packet)
{
    bool bRet(false);

    do
    {
        // 将Packet转换成WrappedPacket
        WrappedPacket wrappedPacket = wrapPacket(packet);

        // 将WrappedPacket转换成ByteArray
        QByteArray sendByte;
        QDataStream out(&sendByte, QIODevice::WriteOnly);
        out << wrappedPacket;

        // 开始发送网络字节
        qint64 sendLen = write(sendByte);

        // 等待写入完成
        if (!waitForBytesWritten())
        {
            qCritical() << "client write bytes to socket failed.";
            break;
        }

//        // 检验是否完整发送
//        if (sendLen < (qint64)(wrappedPacket.length + 5))
//        {
//            qCritical() << "client write bytes to socket failed.";
//            break;
//        }

        bRet = true;
    } while(false);

    // 通知外界已发送
    if(bRet)
        emit onPacketSend(packet, socketDescriptor());

    return bRet;
}

void ShoeTcpSocket::receiveRule(QByteArray &recvBuffer)
{
    while(true)
    {
        int totalLen = recvBuffer.size();

        //不够包头的数据直接就不处理。
        if( totalLen < (int)(sizeof(quint16) + sizeof(quint8))  )
        {
            break;
        }

        //与QDataStream绑定，方便操作。
        QDataStream in(recvBuffer);

        // 找结束点
        int stopPosition = recvBuffer.indexOf(QByteArray::fromHex("0d0a"));

        // 没找到 退出
        if(stopPosition == -1)
        {
            break;
        }

        //数据包已收够，进行解析
        WrappedPacket wrappedPacket;
        in.device()->seek(0);

        in >> wrappedPacket.start;
        in >> wrappedPacket.length;
        wrappedPacket.length = stopPosition-3;
        in >> wrappedPacket.msgType;
        QByteArray content;
        for(int i=0; i<stopPosition-4; i++)
        {
            qint8 temp;
            in >> temp;
            content.append(temp);
        }
        wrappedPacket.msgContent = content;
        in >> wrappedPacket.stop;

        // 通知外界已接收
        emit onPacketReceived(extractPacket(wrappedPacket), socketDescriptor());

        //用多余的数据更新缓存
        recvBuffer = recvBuffer.right(totalLen - (stopPosition+2));
    }
}

void ShoeTcpSocket::slotConnected()
{
    setSocketID(socketDescriptor());
    emit onConnected(socketID);
}

void ShoeTcpSocket::slotDisconnected()
{
    emit onDisonnected(socketID);
}

void ShoeTcpSocket::slotDataReceived()
{
    if ( bytesAvailable() <= 0 )
    {
        return;
    }

    //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
    QByteArray buffer = readAll();

    ShoeUtilsLibrary::DebugTimeString(buffer.toHex());

    //上次缓存加上这次数据
    recvBuffer.append(buffer);

    //要是扩展的话 扩展这个就行
    receiveRule(recvBuffer);
}

void ShoeTcpSocket::slotErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    QString errorString = this->errorString();
    emit onErrorOccurred(errorString, socketID);
}
