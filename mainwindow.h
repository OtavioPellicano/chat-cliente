#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "cliente.h"
#include <QStringList>

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

    QStringList mNicknameOnline;

};

#endif // MAINWINDOW_H
