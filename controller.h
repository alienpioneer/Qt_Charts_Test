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
    Q_PROPERTY( QPoint value READ value    NOTIFY updateValue)
    Q_PROPERTY( int yAxisMax READ yAxisMax NOTIFY updateYMax )
    Q_PROPERTY( int xAxisMax READ xAxisMax NOTIFY updateXMax )
    Q_PROPERTY( int xAxisMin READ xAxisMin NOTIFY updateXMin )

public:
    UiController();
    ~UiController();

    QPoint value();
    int yAxisMax();
    int xAxisMax();
    int xAxisMin();

private:
    void readDatagrams();

signals:
    void updateValue();
    void updateYMax();
    void updateXMax();
    void updateXMin();

private:
    QUdpSocket*     _socket;
    QByteArray      _buffer;
    QVector<QPoint> _allPoints;
    int             _xAxisMax   = 100;
    int             _xAxisMin   = 0;
    int             _yAxisMax   = 1000;
    const int       _maxPoints  = 50;
    const int       _samplingPeriod = 2;
};

#endif // CONTROLLER_H
