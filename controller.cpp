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
        _currentValue = data;
        emit updateValue();
    }
}

void UiController::onUpdateSamplePeriod()
{

}

int UiController::value()
{
    //qDebug() << "Sending to qml " << data;
    return _currentValue;
}

int UiController::samplePeriod()
{
    return _samplingPeriod;
}

void UiController::setSamplingPeriod(int samplePeriod)
{
    _samplingPeriod = samplePeriod;
    qDebug() << "sampling period " <<_samplingPeriod;
//    QByteArray data = QByteArray::number(_samplingPeriod);
//    _socket->writeDatagram(data.data(), data.length(), QHostAddress("10.0.0.2"), PORT);
}

