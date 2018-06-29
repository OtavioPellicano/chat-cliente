#include "cliente.h"

Cliente::Cliente(QObject *parent) : QObject(parent)
{

}

bool Cliente::startCliente(const QString &host, const qint16 &porta)
{
    setSocket(new QTcpSocket(this));

    connect(socket(), SIGNAL(connected()), this, SLOT(connected()));
    connect(socket(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket(), SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::QueuedConnection);

    socket()->connectToHost(host, porta);

    if(!socket()->waitForConnected())
    {
        qDebug() << "Impossivel conectar ao servidor";
        return false;
    }

    setDescriptor(socket()->socketDescriptor());

    return true;
}

bool Cliente::enviarMensagem(const QString &qstrMsg)
{
    QByteArray byteArrayTemp = qstrMsg.toLocal8Bit();
    byteArrayTemp.append("\r\n\r\n\r\n");       //garantindo o envio
    socket()->write(byteArrayTemp);
    if(!socket()->waitForBytesWritten())
        return false;
    return true;
}

QTcpSocket *Cliente::socket() const
{
    return mSocket;
}

void Cliente::setSocket(QTcpSocket *socket)
{
    mSocket = socket;
}

qintptr Cliente::descriptor() const
{
    return mDescriptor;
}

void Cliente::setDescriptor(const qintptr &descriptor)
{
    mDescriptor = descriptor;
}


void Cliente::connected()
{
    qDebug() << descriptor() << ": connected";
}

void Cliente::disconnected()
{
    qDebug() << descriptor() << ": desconnected";
}

void Cliente::bytesWritten(qint64 bytes)
{
    qDebug() << descriptor() << "bytes written: " << bytes;
}

void Cliente::readyRead()
{
    QByteArray bTemp = socket()->readAll();
    qDebug() << descriptor() << "read:" << bTemp;

    emit readyRead(bTemp);
}





