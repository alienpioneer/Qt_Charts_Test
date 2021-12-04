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
    Q_PROPERTY( int value        READ value                                 NOTIFY updateValue        )
    Q_PROPERTY( int samplePeriod READ samplePeriod WRITE setSamplingPeriod  NOTIFY updateSamplePeriod )

public:
    UiController();
    ~UiController();

    int value();
    int samplePeriod();

private:
    void readDatagrams();
    void onUpdateSamplePeriod();

signals:
    void updateValue();
    void updateSamplePeriod();

public slots:
    Q_INVOKABLE void setSamplingPeriod(int samplePeriod);

private:
    QUdpSocket* _socket;
    QByteArray  _buffer;
    int         _currentValue;
    int         _samplingPeriod = 2;

};

#endif // CONTROLLER_H
