#include "cliente.h"

Cliente::Cliente(QObject *parent) : QObject(parent)
{

}

bool Cliente::startCliente()
{
    setSocket(new QTcpSocket(this));

    connect(socket(), SIGNAL(connected()), this, SLOT(connected()));
    connect(socket(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket(), SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket()->connectToHost("127.0.0.1", 1312);

    if(!socket()->waitForConnected())
    {
        qDebug() << "Impossivel conectar ao servidor";
        return false;
    }

    setDescriptor(socket()->socketDescriptor());

    return true;
}

void Cliente::enviarNickname(const QString &nickname)
{
    QByteArray data;
    data.append("#");
    data.append(KEY_NICKNAME);
    data.append("#:");
    data.append(nickname);
    socket()->write(data);
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
    qDebug() << descriptor() << ":" << socket()->readAll();
    QByteArray bTemp = socket()->readAll();

    emit readyRead(socket()->readAll());
}




