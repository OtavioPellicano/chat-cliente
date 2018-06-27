#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionSair_da_sala->setEnabled(false);
    ui->comboBox_conversa_atual->setEnabled(false);
    ui->listView_chat->setEnabled(false);
    ui->listView_usuarios->setEnabled(false);
    ui->pushButton_enviar->setEnabled(false);
    ui->textEdit_mensagem->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::nickname() const
{
    return mNickname;
}

void MainWindow::setNickname(const QString &nickname)
{
    mNickname = nickname;
}

QString MainWindow::origem() const
{
    return mOrigem;
}

void MainWindow::setOrigem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itFirstQuadrado = std::find(str.begin(), str.end(), '#');
    string::iterator itSecondQuadrado = std::find(itFirstQuadrado + 1, str.end(), '#');

    mOrigem = QString::fromStdString(string(itFirstQuadrado + 1, itSecondQuadrado));

}

QString MainWindow::destino() const
{
    return mDestino;
}

void MainWindow::setDestino(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itSecondQuadrado = std::find(str.begin() + 1, str.end(), '#');
    string::iterator itThirdQuadrado = std::find(itSecondQuadrado + 1, str.end(), '#');

    mDestino = QString::fromStdString(string(itSecondQuadrado + 1, itThirdQuadrado));
}

QString MainWindow::mensagem() const
{
    return mMensagem;
}

void MainWindow::setMensagem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    str = string(str.begin(),std::remove(str.begin(), str.end(), '\r'));
    str = string(str.begin(),std::remove(str.begin(), str.end(), '\n'));

    string::iterator itFirst = std::find(str.begin(), str.end(), ':');

    mMensagem = QString::fromStdString(string(itFirst + 1, str.end()));
}

/**
 * @brief Servidor::validarEstruturaMensagem
 * @param msg
 * @return
 * valida a estrutura do protocolo de mensagem, conforme a seguir:
 * #origem#destino#:mensagem
 * o destino pode está vazio, representando assim o input de um nickname
 * se a mensagem vier ###:
 * o cliente foi desconectado do servidor
 */
bool MainWindow::validarEstruturaMensagem(const QByteArray &msg)
{
    //     #origem#destino#:mensagem

    string str = QString(msg).toStdString();

    if(!(std::count(str.begin(), str.end(),'#') >= 3 && std::count(str.begin(), str.end(), ':') >= 1))
    {
        return false;
    }

    string::iterator itFirstQuadrado = std::find(str.begin(), str.end(), '#');
    string::iterator itSecondQuadrado = std::find(itFirstQuadrado + 1, str.end(), '#');
    string::iterator itThirdQuadrado = std::find(itSecondQuadrado + 1, str.end(), '#');
    string::iterator itPonto = std::find(str.begin(), str.end(), ':');

    if(itFirstQuadrado == str.begin())
        if(itThirdQuadrado + 1 == itPonto)                      //o ultimo # deve seguir de :
            return true;

    return false;
}

QString MainWindow::encapsularMsg(const QString &qstrOrigem, const QString &qstrDestino, const QString &qstrMsg)
{
    return QString("#%1#%2#:%3").arg(qstrOrigem).arg(qstrDestino).arg(qstrMsg);
}

void MainWindow::readyRead(const QByteArray &msg)
{


    qDebug() << "mainWindowReadReady: " << msg;

    if(!validarEstruturaMensagem(msg))
    {
        qDebug() << "mensagem fora da estrutura";

        return;
    }

    QString qstr = QString(msg);
    QMessageBox::information(this, "teste", QString(msg));

    ui->actionSair_da_sala->setEnabled(true);
    ui->comboBox_conversa_atual->setEnabled(false);
    ui->listView_chat->setEnabled(false);
    ui->listView_usuarios->setEnabled(false);
    ui->pushButton_enviar->setEnabled(false);
    ui->textEdit_mensagem->setEnabled(false);

}

Cliente *MainWindow::cliente() const
{
    return mCliente;
}

void MainWindow::setCliente(Cliente *cliente)
{
    mCliente = cliente;
}


void MainWindow::on_actionFechar_triggered()
{
    this->close();
}

void MainWindow::on_actionConectar_a_sala_triggered()
{
    bool ok;

    QString strTemp = QInputDialog::getText(this, tr("Nickname"), tr("Insira um nickname para entrar na sala"), QLineEdit::Normal, tr("nickname"), &ok);

    setCliente(new Cliente(this));

    connect(cliente(), SIGNAL(readyRead(QByteArray)), this, SLOT(readyRead(QByteArray)));

    if(ok)
    {

        if(!cliente()->startCliente())
        {
            QMessageBox::critical(this, tr("Erro!"), tr("Erro ao entrar na sala!\nVerifique se o servidor está online.\n-> 127.0.0.1 1312"), QMessageBox::Ok);
            return;
        }



        setNickname(strTemp);
        if(!cliente()->enviarMensagem(encapsularMsg(nickname())))
        {
            qDebug() << "impossivel enviar a mensagem";
            return;
        }


        ui->actionConectar_a_sala->setEnabled(false);
        ui->actionSair_da_sala->setEnabled(true);
        ui->comboBox_conversa_atual->setEnabled(true);
        ui->listView_chat->setEnabled(true);
        ui->listView_usuarios->setEnabled(true);
        ui->pushButton_enviar->setEnabled(true);
        ui->textEdit_mensagem->setEnabled(true);

    }
    else
    {
        delete cliente();
    }


}


void MainWindow::on_actionSair_da_sala_triggered()
{
    ui->actionConectar_a_sala->setEnabled(true);
    ui->actionSair_da_sala->setEnabled(false);

    delete cliente();

    qDebug() << "Desconectado";

}

void MainWindow::on_pushButton_enviar_clicked()
{

}
