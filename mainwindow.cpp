#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUiConectado(false);

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

void MainWindow::setUiConectado(const bool &value)
{
    ui->lineEdit_origem->setReadOnly(true);
    ui->lineEdit_destino->setReadOnly(true);

    if(value)
    {
        ui->actionConectar_a_sala->setEnabled(false);
        ui->actionSair_da_sala->setEnabled(true);
        ui->lineEdit_destino->setEnabled(true);
        ui->listWidget_chat->setEnabled(true);
        ui->listWidget_usuarios->setEnabled(true);
        ui->pushButton_enviar->setEnabled(true);
        ui->lineEdit_mensagem->setEnabled(true);
        ui->lineEdit_origem->setEnabled(true);

        ui->statusBar->showMessage(QString("conectado como %1").arg(nickname()), 3000);
    }
    else
    {
        ui->actionConectar_a_sala->setEnabled(true);
        ui->actionSair_da_sala->setEnabled(false);
        ui->lineEdit_destino->setEnabled(false);
        ui->listWidget_chat->setEnabled(false);
        ui->listWidget_usuarios->setEnabled(false);
        ui->pushButton_enviar->setEnabled(false);
        ui->lineEdit_mensagem->setEnabled(false);
        ui->lineEdit_origem->setEnabled(false);

        ui->lineEdit_origem->clear();
        ui->lineEdit_destino->clear();
        ui->lineEdit_mensagem->clear();
        ui->listWidget_chat->clear();
        ui->listWidget_usuarios->clear();

        ui->statusBar->showMessage(tr("desconectado"), 3000);

    }
}

void MainWindow::readyRead(const QByteArray &msg)
{

    qDebug() << "mainWindowReadReady: " << msg;

    if(!validarEstruturaMensagem(msg))
    {
        qDebug() << "mensagem fora da estrutura";
        return;
    }

    setOrigem(msg);
    setDestino(msg);
    setMensagem(msg);

    //Validando nickname existente
    if(destino().isEmpty() && origem().isEmpty())
    {
        QMessageBox::critical(this, tr("Chat"), tr("Nickname em uso!"), QMessageBox::Ok);
        return;
    }


    //recebendo nickname
    if(destino().isEmpty() && !origem().isEmpty())
    {
        setNickname(origem());
        ui->lineEdit_origem->setText(nickname());
        setUiConectado(true);
        return;
    }

    //Broadcast: #$$$$#???#:user1;user2;user3
    if(origem() == BROADCAST_KEY &&
            (destino() == BROADCAST_CONECTADO || destino() == BROADCAST_DESCONECTADO) &&
            !mensagem().isEmpty())
    {

        if(destino() == BROADCAST_CONECTADO)
        {
            setListaNicknameOnline(mensagem().split(";"));
            QString usuarioOnline = listaNicknameOnline().back();

            for(auto itqstr = mListaNicknameOnline.begin(); itqstr != mListaNicknameOnline.end(); ++itqstr)
            {
                if(*itqstr == nickname())
                {
                    mListaNicknameOnline.erase(itqstr);
                    break;
                }
            }

            ui->listWidget_usuarios->clear();
            ui->listWidget_usuarios->addItems(listaNicknameOnline());

            qDebug() << "nickname: " << nickname();
            qDebug() << "usuario online:" << usuarioOnline;

            if(usuarioOnline != nickname())
            {
                QMessageBox::information(this, tr("Chat"), QString("Novo usuário online:\n%1").arg(usuarioOnline), QMessageBox::Ok);
            }


        }
        else
        {
            setListaNicknameOnline(mensagem().split(";"));
            ui->listWidget_usuarios->clear();

            ui->statusBar->showMessage(QString("%1 desconectado").arg(mListaNicknameOnline.back()), 3000);

            mListaNicknameOnline.pop_back();

            for(auto itqstr = mListaNicknameOnline.begin(); itqstr != mListaNicknameOnline.end(); ++itqstr)
            {
                if(*itqstr == nickname())
                {
                    mListaNicknameOnline.erase(itqstr);
                    break;
                }
            }

            ui->listWidget_usuarios->addItems(listaNicknameOnline());

        }

        return;
    }



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

    }
    else
    {
        delete cliente();
    }


}


void MainWindow::on_actionSair_da_sala_triggered()
{
    setUiConectado(false);

    delete cliente();

    qDebug() << "Desconectado";

}

void MainWindow::on_pushButton_enviar_clicked()
{
    setHomeMensagem(ui->lineEdit_mensagem->text());
    ui->lineEdit_mensagem->clear();
    if(homeMensagem().isEmpty() || homeDestino().isEmpty())
        return;


    mListaConversa.append(QString("%1: %2").arg(homeOrigem()).arg(homeMensagem()));
    ui->listWidget_chat->clear();
    ui->listWidget_chat->addItems(mListaConversa);
    ui->listWidget_chat->scrollToBottom();

    cliente()->enviarMensagem(encapsularMsg(homeOrigem(), homeDestino(), homeMensagem()));
}

QStringList MainWindow::listaNicknameOnline() const
{
    return mListaNicknameOnline;
}

void MainWindow::setListaNicknameOnline(const QStringList &listaNicknameOnline)
{
    mListaNicknameOnline = listaNicknameOnline;
}



void MainWindow::on_listWidget_usuarios_itemDoubleClicked(QListWidgetItem *item)
{

    ui->lineEdit_destino->setText(item->text());
    setHomeOrigem(ui->lineEdit_origem->text());
    setHomeDestino(ui->lineEdit_destino->text());

    ui->statusBar->showMessage(QString("conversando com %1").arg(item->text()));


}

QString MainWindow::homeMensagem() const
{
    return mHomeMensagem;
}

void MainWindow::setHomeMensagem(const QString &homeMensagem)
{
    mHomeMensagem = homeMensagem;
}

QString MainWindow::homeDestino() const
{
    return mHomeDestino;
}

void MainWindow::setHomeDestino(const QString &homeDestino)
{
    mHomeDestino = homeDestino;
}

QString MainWindow::homeOrigem() const
{
    return mHomeOrigem;
}

void MainWindow::setHomeOrigem(const QString &homeOrigem)
{
    mHomeOrigem = homeOrigem;
}
