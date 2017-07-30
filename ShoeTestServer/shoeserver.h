#ifndef SHOESERVER_H
#define SHOESERVER_H

#include <QWidget>
#include <QString>
#include <QMap>
#include "shoetcpserver.h"


namespace Ui {
class ShoeServer;
}

class ShoeServer : public QWidget
{
    Q_OBJECT

public:
    explicit ShoeServer(QWidget *parent = 0);
    ~ShoeServer();

private slots:
    void slotListeningStatus(bool isListening);
    void slotConnected(const int nDescriptor);
    void slotDisconnected(const int nDescriptor);

    void slotPacketsend(const Packet& packet, const int nDescriptor);
    void slotPacketReceived(const Packet& packet, const int nDescriptor);
    void slotErrorOccurred(const QString& errorString, const int nDescriptor);

    void on_pushButton_switch_clicked();
    void on_pushButton_clear_clicked();

private:
    Ui::ShoeServer *ui;
    ShoeTcpServer *commServer;

    QMap<int, QString> m_descriptorIMEI;

    // 发送
    bool sendPacket(int nDescriptor, MsgType msgType, QByteArray msgContent);

    // 接收
    void receiveLogin(const int nDescriptor, QByteArray content);
    void receiveGPS(const int nDescriptor, QByteArray content);
    void receivePosition(const int nDescriptor, QByteArray content);
    void receiveUpdateTime(const int nDescriptor, QByteArray content);
    void reveiveSyncSetting(const int nDescriptor, QByteArray content);

};


#endif // SHOESERVER_H
