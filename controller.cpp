#include "controller.h"

UiController::UiController()
    : _socket( new QUdpSocket(this) )
{
    //_socket->bind(QHostAddress::LocalHost, PORT);
    _socket->bind(QHostAddress("10.0.0.10"), PORT);
    connect(_socket ,&QUdpSocket::readyRead, this, &UiController::readDatagrams );
}

UiController::~UiController()
{

}

int UiController::getValue()
{
    const int data = _buffer.toInt();
    _buffer.clear();
    qDebug() << "Sending to qml " << data;
    return data;
}

QVariant UiController::getYMax()
{
    return QVariant::fromValue(_yAxisMax);
}

int UiController::getXMax()
{
    return _xAxisMax;
}

void UiController::readDatagrams()
{
    _buffer.clear();

    while (_socket->hasPendingDatagrams())
    {
        _buffer = _socket->receiveDatagram().data();
    }
    emit updateValue();
}
