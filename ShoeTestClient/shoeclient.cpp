#include <QLineEdit>

#include "shoeclient.h"
#include "ui_shoeclient.h"
#include "shoehelper.h"

#include "messagelogin.h"
#include "messagegps.h"

ShoeClient::ShoeClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoeClient)
{
    ui->setupUi(this);

    commClient = new ShoeTcpClient(this);

    connect(commClient, &ShoeTcpClient::onConnected, this, &ShoeClient::slotConnected);
    connect(commClient, &ShoeTcpClient::onDisconnected, this, &ShoeClient::slotDisconnected);
    connect(commClient, &ShoeTcpClient::onPacketSend, this, &ShoeClient::slotPacketsend);
    connect(commClient, &ShoeTcpClient::onPacketReceived, this, &ShoeClient::slotPacketReceived);
    connect(commClient, &ShoeTcpClient::onErrorOccurred, this, &ShoeClient::slotErrorOccurred);
}

ShoeClient::~ShoeClient()
{
    delete ui;
}

void ShoeClient::slotConnected()
{
    ui->textEdit->append(ShoeHelper::timePrefixedString("[连接]"));
    ui->pushButton_connect->setText("断开");
}

void ShoeClient::slotDisconnected()
{
    ui->textEdit->append(ShoeHelper::timePrefixedString("[断开]"));
    ui->pushButton_connect->setText("连接");
}

void ShoeClient::slotPacketsend(const Packet &packet)
{
    QString text = "[发送] <" + ShoeHelper::msgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeHelper::timePrefixedString(text));
}

void ShoeClient::slotPacketReceived(const Packet &packet)
{
    QString text = "[接收] <" + ShoeHelper::msgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeHelper::timePrefixedString(text));
}

void ShoeClient::slotErrorOccurred(const QString &errorString)
{
    ui->textEdit->append("[错误]" + ShoeHelper::timePrefixedString(errorString));
}

void ShoeClient::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text()=="连接")
        commClient->initialize(QHostAddress(ui->comboBox_ip->currentText()), ui->spinBox_port->value());
    else
        commClient->finalize();
}

void ShoeClient::on_pushButton_status_clicked()
{

}

void ShoeClient::on_pushButton_gps_clicked()
{
    MessageGPS gps;
    sendPacket(PacketType_GPS,   gps.getData());
}

void ShoeClient::on_pushButton_login_clicked()
{
    MessageLogin login;
    sendPacket(PacketType_Login, login.getData());
}

bool ShoeClient::sendPacket(MsgType msgType, QByteArray msgContent, quint16 msgNumber)
{
    Packet packet;
    packet.msgType    = msgType;
    packet.msgContent = msgContent;
    packet.msgNumber  = msgNumber;
    return commClient->sendPacket(packet);
}

void ShoeClient::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}
