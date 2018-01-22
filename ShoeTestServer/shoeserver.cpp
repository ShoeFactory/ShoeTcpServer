#include <QJsonValue>

#include "shoeutilslibrary.h"

#include "ui_shoeserver.h"
#include "shoeserver.h"
#include "shoehttpaccesss.h"

#include "messagelogin.h"
#include "messagegps.h"

ShoeServer::ShoeServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoeServer)
{
    ui->setupUi(this);
    commServer = new ShoeTcpServer(this);

    connect(commServer, &ShoeTcpServer::onListeningStatus, this, &ShoeServer::slotListeningStatus);
    connect(commServer, &ShoeTcpServer::onConnected, this, &ShoeServer::slotConnected);
    connect(commServer, &ShoeTcpServer::onDisconnected, this, &ShoeServer::slotDisconnected);
    connect(commServer, &ShoeTcpServer::onPacketSend, this, &ShoeServer::slotPacketsend);
    connect(commServer, &ShoeTcpServer::onPacketReceived, this, &ShoeServer::slotPacketReceived);
    connect(commServer, &ShoeTcpServer::onErrorOccurred, this, &ShoeServer::slotErrorOccurred);
}

ShoeServer::~ShoeServer()
{
    delete ui;
}

void ShoeServer::receiveLogin(const int nDescriptor, QByteArray content)
{
    // 解析：登录包
    MessageLogin loginReceived;
    loginReceived.setData(content);

    // 逻辑：匹配 nDescriptor 与 IMEI
    m_descriptorIMEI.insert(nDescriptor, loginReceived.getIMEI());

    // 回复：成功
    MessageLogin loginReply;
    sendPacket(nDescriptor, PacketType_Login, loginReply.getData());
}

void ShoeServer::receiveGPS(const int nDescriptor, QByteArray content)
{
    // 解析：GPS包
    MessageGPS gpsReceived;
    gpsReceived.setData(content);

    // 逻辑：写入数据库

    //![1] GPS 给出 jsonobject
    QJsonObject jsonObject = gpsReceived.getjsonObject();

    //![2] 改造JsonObject
    QString IMEI = m_descriptorIMEI.value(nDescriptor);
    jsonObject.insert("IMEI", IMEI);

    //![3] 发送写数据库请求
    ShoeHttpAccessor->postGPS(jsonObject);

    // 回复：无
}

void ShoeServer::receivePosition(const int nDescriptor, QByteArray content)
{
    // 解析：定位包

    // 逻辑：写入数据库

    // 回复：无
    Q_UNUSED(nDescriptor)
    Q_UNUSED(content)
}

void ShoeServer::receiveUpdateTime(const int nDescriptor, QByteArray content)
{
    Q_UNUSED(nDescriptor)
    Q_UNUSED(content)
}

void ShoeServer::reveiveSyncSetting(const int nDescriptor, QByteArray content)
{
    Q_UNUSED(nDescriptor)
    Q_UNUSED(content)
}

void ShoeServer::slotListeningStatus(bool isListening)
{
    if(isListening)
    {
        ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[启动]"));
        ui->pushButton_switch->setText("停止");
    }
    else
    {
        ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[终止]"));
        ui->pushButton_switch->setText("启动");
    }
}

void ShoeServer::slotConnected(const int nDescriptor)
{
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[连接" + QString::number(nDescriptor) + "]"));
}

void ShoeServer::slotDisconnected(const int nDescriptor)
{
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[断开" + QString::number(nDescriptor) + "]"));

    // 移除
    m_descriptorIMEI.remove(nDescriptor);
}

void ShoeServer::slotPacketsend(const Packet &packet, const int nDescriptor)
{
    QString text = "[发送" + QString::number(nDescriptor) + "] <" + MsgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString(text));
}

void ShoeServer::slotPacketReceived(const Packet &packet, const int nDescriptor)
{
    QString text = "[接收" + QString::number(nDescriptor) + "] <" + MsgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString(text));

    MsgType    msgType = packet.msgType;
    QByteArray msgContent = packet.msgContent;

    switch (msgType) {
    case PacketType_Login:
        receiveLogin(nDescriptor, msgContent);
        break;
    case PacketType_GPS:
        receiveGPS(nDescriptor, msgContent);
        break;

    case PacketType_Position:
        receivePosition(nDescriptor, msgContent);
        break;

    default:
        break;
    }
}

void ShoeServer::slotErrorOccurred(const QString &errorString, const int nDescriptor)
{
    QString text = "[错误" + QString::number(nDescriptor) + "]" + errorString;
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString(text));
}

void ShoeServer::on_pushButton_switch_clicked()
{
    if(ui->pushButton_switch->text()=="启动")
        commServer->initialize(ui->spinBox_port->value());
    else
        commServer->finalize();
}

void ShoeServer::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

bool ShoeServer::sendPacket(int nDescriptor, MsgType msgType, QByteArray msgContent)
{
    Packet packet;
    packet.msgType    = msgType;
    packet.msgContent = msgContent;

    return commServer->sendPacket(packet, nDescriptor);
}
