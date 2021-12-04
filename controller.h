#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

#define PORT 34455

class UiController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value    READ getValue NOTIFY updateValue)
    Q_PROPERTY(QVariant yAxisMax READ getYMax  NOTIFY updateYMax)
    Q_PROPERTY(int xAxisMax READ getXMax  NOTIFY updateXMax)
public:
    UiController();
    ~UiController();

    int getValue();
    QVariant getYMax();
    int getXMax();

private:
    void readDatagrams();

signals:
    void updateValue();
    void updateYMax();
    void updateXMax();

private:
    QUdpSocket* _socket;
    QByteArray  _buffer;
    int         _xAxisMax = 300;
    int         _yAxisMax = 5000;
};

#endif // CONTROLLER_H
