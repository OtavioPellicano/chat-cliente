#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "cliente.h"
#include <QStringList>
#include <string>
#include <map>

/**
 * codigo de retorno
 * $c$  //conectado
 * $d$  //fui desconectado
 *
 * formato mensagem
 * #origem#destino#:
 *
 */
using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    Cliente *cliente() const;
    void setCliente(Cliente *cliente);

    QString nickname() const;
    void setNickname(const QString &nickname);

    QString origem() const;
    void setOrigem(const QByteArray &msg);

    QString destino() const;
    void setDestino(const QByteArray &msg);

    QString mensagem() const;
    void setMensagem(const QByteArray &msg);

    bool validarEstruturaMensagem(const QByteArray &msg);

    QString encapsularMsg(const QString &qstrOrigem, const QString &qstrDestino = "", const QString &qstrMsg = "");


public slots:
    void readyRead(const QByteArray &msg);

private slots:


    void on_actionFechar_triggered();

    void on_actionConectar_a_sala_triggered();

    void on_actionSair_da_sala_triggered();

    void on_pushButton_enviar_clicked();

private:
    Ui::MainWindow *ui;

    Cliente *mCliente;
    QString mNickname;

    QString mOrigem;
    QString mDestino;
    QString mMensagem;

    std::map<QString, QStringList> mMapNickLog;

};

#endif // MAINWINDOW_H
