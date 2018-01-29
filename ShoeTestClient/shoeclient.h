#ifndef SHOECLIENT_H
#define SHOECLIENT_H

#include <QWidget>
#include "shoetcpclient.h"

namespace Ui {
class ShoeClient;
}

class ShoeClient : public QWidget
{
    Q_OBJECT

public:
    explicit ShoeClient(QWidget *parent = 0);
    ~ShoeClient();

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotPacketsend(const Packet& packet);
    void slotPacketReceived(const Packet& packet);
    void slotErrorOccurred(const QString& errorString);

    void on_pushButton_clear_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_gps_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_wifilbs_clicked();

private:
    Ui::ShoeClient *ui;
    ShoeTcpClient *commClient;
    bool sendPacket(MsgType msgType, QByteArray msgContent);
};

#endif // SHOECLIENT_H
