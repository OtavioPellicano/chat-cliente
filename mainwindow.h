#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "cliente.h"
#include <QStringList>
#include <QStringBuilder>
#include <string>
#include <map>
#include <QListWidget>
#include <configuracoes.h>


/**
 * codigo de retorno
 * $c$  //conectado
 * $d$  //desconectado
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


    QString host() const;
    void setHost(const QString &host);

    qint16 porta() const;
    void setPorta(const qint16 &porta);

private:
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

    void setUiConectado(const bool &value);

    QStringList listaNicknameOnline() const;
    void setListaNicknameOnline(const QStringList &listaNicknameOnline);


    QString homeOrigem() const;
    void setHomeOrigem(const QString &homeOrigem);

    QString homeDestino() const;
    void setHomeDestino(const QString &homeDestino);

    QString homeMensagem() const;
    void setHomeMensagem(const QString &homeMensagem);

    void atualizarChatList(const QString &dst, const QString &msg);



public slots:
    void readyRead(const QByteArray &msg);


private slots:

    void on_actionFechar_triggered();

    void on_actionConectar_a_sala_triggered();

    void on_actionSair_da_sala_triggered();

    void on_pushButton_enviar_clicked();

    void on_listWidget_usuarios_itemDoubleClicked(QListWidgetItem *item);

    void on_actionConfigurar_Host_triggered();

public slots:
    void configuracoesOnPushButtonClicked(const QString &host, const int &porta);


private:
    Ui::MainWindow *ui;

    const QString BROADCAST_KEY = "$$$";
    const QString BROADCAST_CONECTADO = "$c$";      //broadcast: #$$$$#$c$#:user1;user2;user3
    const QString BROADCAST_DESCONECTADO = "$d$";   //broadcast: #$$$$#$d$#:user1;user2;user3

    Cliente *mCliente;
    QString mNickname;

    QString mOrigem;
    QString mDestino;
    QString mMensagem;

    std::map<QString, QStringList> mMapNickLog;
    QStringList mListaNicknameOnline;
    QStringList mListaConversa;

    QString mHomeOrigem;
    QString mHomeDestino;
    QString mHomeMensagem;

    Configuracoes *mConfigUi;

    QString mHost = "127.0.0.1";
    qint16 mPorta = 1312;

};

#endif // MAINWINDOW_H
