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

void MainWindow::readyRead(const QByteArray &msg)
{
    QString qstr = msg;
    QMessageBox::information(this, "teste", QString(msg));

    ui->actionSair_da_sala->setEnabled(false);
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
        cliente()->enviarNickname(nickname());

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
