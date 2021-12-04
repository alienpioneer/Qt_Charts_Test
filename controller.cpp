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

    if (data > _yAxisMax)
    {
        _yAxisMax = data + data*0.05;
        emit updateYMax();
    }

    if( data > 0 )
    {
        _currentValue = data;
        emit updateValue();
    }
}

int UiController::value()
{
    //qDebug() << "Sending to qml " << data;
    return _currentValue;
}

int UiController:: yAxisMax()
{
    return _yAxisMax;
}

