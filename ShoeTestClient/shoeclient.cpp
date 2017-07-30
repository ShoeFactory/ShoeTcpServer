#include <QLineEdit>
#include "shoeutilslibrary.h"

#include "shoeclient.h"
#include "ui_shoeclient.h"

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
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[连接]"));
    ui->pushButton_connect->setText("断开");
}

void ShoeClient::slotDisconnected()
{
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString("[断开]"));
    ui->pushButton_connect->setText("连接");
}

void ShoeClient::slotPacketsend(const Packet &packet)
{
    QString text = "[发送] <" + MsgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString(text));
}

void ShoeClient::slotPacketReceived(const Packet &packet)
{
    QString text = "[接收] <" + MsgTypeString(packet.msgType) + "> : " + packet.msgContent.toHex();
    ui->textEdit->append(ShoeUtilsLibrary::TimePrefixedString(text));
}

void ShoeClient::slotErrorOccurred(const QString &errorString)
{
    ui->textEdit->append("[错误]" + ShoeUtilsLibrary::TimePrefixedString(errorString));
}

void ShoeClient::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text()=="连接")
        commClient->initialize(QHostAddress(ui->comboBox_ip->currentText()), ui->spinBox_port->value());
    else
        commClient->finalize();
}


void ShoeClient::on_pushButton_gps_clicked()
{
    sendPacket(PacketType_GPS,QByteArray::fromHex("0A03170F32179C026B3F3E0C22AD651F3460"));
}

void ShoeClient::on_pushButton_login_clicked()
{
    sendPacket(PacketType_Login, QByteArray::fromHex("012345678901234535"));
}

bool ShoeClient::sendPacket(MsgType msgType, QByteArray msgContent)
{
    Packet packet;
    packet.msgType    = msgType;
    packet.msgContent = msgContent;

    return commClient->sendPacket(packet);
}

void ShoeClient::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}
