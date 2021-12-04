#include "controller.h"

UiController::UiController()
    : _socket( new QUdpSocket(this) )
{
    //_socket->bind(QHostAddress::LocalHost, PORT);
    _socket->bind(QHostAddress("10.0.0.10"), PORT);
    connect(_socket ,&QUdpSocket::readyRead, this, &UiController::readDatagrams );
}

UiController::~UiController()
{}

void UiController::readDatagrams()
{
    _buffer.clear();

    while (_socket->hasPendingDatagrams())
    {
        _buffer = _socket->receiveDatagram().data();
    }

    int data = _buffer.toInt();

    if( data > 0 )
    {
        int dataX = 0;

        if (_allPoints.size() > 0)
            dataX = _allPoints.last().x()+_samplingPeriod;

        _allPoints.append( QPoint(dataX, data) );
        emit updateValue();
    }
}

QPoint UiController::value()
{
    //qDebug() << "Sending to qml " << data;
    return _allPoints.last();
}

int UiController:: yAxisMax()
{
    return _yAxisMax;
}

int UiController:: xAxisMax()
{
    return _xAxisMax;
}

int UiController::xAxisMin()
{
    return _xAxisMin;
}
