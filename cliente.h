#ifndef CLIENTE_H
#define CLIENTE_H

#include <QObject>
#include <QTcpSocket>
#include <QObject>
#include <QTest>

class Cliente : public QObject
{
    Q_OBJECT
public:
    explicit Cliente(QObject *parent = nullptr);

    virtual ~Cliente();

    bool startCliente(const QString &host = "127.0.0.1", const qint16 &porta = 1312);

    bool enviarMensagem(const QString &qstrMsg);

private:

    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);

    qintptr descriptor() const;
    void setDescriptor(const qintptr &descriptor);

signals:
    void readyRead(const QByteArray &msg);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:

    QTcpSocket *mSocket;
    qintptr mDescriptor;

};

#endif // CLIENTE_H
