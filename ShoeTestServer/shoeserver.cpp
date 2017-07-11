#include "shoeserver.h"
#include "ui_shoeserver.h"
#include "shoehelper.h"

#include "messagelogin.h"

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

void ShoeServer::slotListeningStatus(bool isListening)
{
    if(isListening)
    {
        ui->textEdit->append(ShoeHelper::timePrefixedString("[启动]"));
        ui->pushButton_switch->setText("停止");
    }
    else
    {
        ui->textEdit->append(ShoeHelper::timePrefixedString("[终止]"));
        ui->pushButton_switch->setText("启动");
    }
}

void ShoeServer::slotConnected(const int nDescriptor)
{
    ui->textEdit->append(ShoeHelper::timePrefixedString("[连接" + QString::number(nDescriptor) + "]"));
}

void ShoeServer::slotDisconnected(const int nDescriptor)
{
    ui->textEdit->append(ShoeHelper::timePrefixedString("[断开" + QString::number(nDescriptor) + "]"));
}

void ShoeServer::slotPacketsend(const Packet &packet, const int nDescriptor)
{
    QString text = "[发送" + QString::number(nDescriptor) + "] <" + ShoeHelper::msgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeHelper::timePrefixedString(text));
}

void ShoeServer::slotPacketReceived(const Packet &packet, const int nDescriptor)
{
    QString text = "[接收" + QString::number(nDescriptor) + "] <" + ShoeHelper::msgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeHelper::timePrefixedString(text));

    //如果是登录
    if(packet.msgType == PacketType_Login)
    {
        MessageLogin login;
        login.removeIMEI();
        sendPacket(nDescriptor, packet.msgType, login.getData(), packet.msgNumber);
    }
}

void ShoeServer::slotErrorOccurred(const QString &errorString, const int nDescriptor)
{
    QString text = "[错误" + QString::number(nDescriptor) + "]" + errorString;
    ui->textEdit->append(ShoeHelper::timePrefixedString(text));
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

bool ShoeServer::sendPacket(int nDescriptor, MsgType msgType, QByteArray msgContent, quint16 msgNumber)
{
    Packet packet;
    packet.msgType    = msgType;
    packet.msgContent = msgContent;
    packet.msgNumber  = msgNumber;

    commServer->sendPacket(packet, nDescriptor);
}
