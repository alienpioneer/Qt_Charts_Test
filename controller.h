#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QUdpSocket>
#include <QNetworkDatagram>

#define PORT 34455

class UiController: public QObject
{
    Q_OBJECT
    Q_PROPERTY( int value    READ value    NOTIFY updateValue)
    Q_PROPERTY( int yAxisMax READ yAxisMax NOTIFY updateYMax )

public:
    UiController();
    ~UiController();

    int value();
    int yAxisMax();

private:
    void readDatagrams();

signals:
    void updateValue();
    void updateYMax();

private:
    QUdpSocket* _socket;
    QByteArray  _buffer;
    int         _currentValue;
    int         _yAxisMax   = 500;
    int   _maxPoints  = 100;
    int   _samplingPeriod = 2;
};

#endif // CONTROLLER_H
