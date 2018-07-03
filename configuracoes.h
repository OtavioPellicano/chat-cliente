#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include <QDialog>
#include <QDir>
#include <fstream>
#include <QStringList>
#include <QDebug>

namespace Ui {
class Configuracoes;
}

class Configuracoes : public QDialog
{
    Q_OBJECT

public:
    explicit Configuracoes(QWidget *parent = 0);
    ~Configuracoes();

private:

    QString sep() const;

    QDir dirConf() const;

    QString nomeArq() const;

    QString host() const;
    void setHost(const QString &host);

    QString porta() const;
    void setPorta(const QString &porta);

    void setup();

    void carregarHostPorta();
    void salvarHostPorta();

signals:
    void pushButtonOkClicked(QString host, int porta);

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::Configuracoes *ui;

    QString mHost;
    QString mPorta;

    QDir mDirConf;
    QString mNomeArq;
    QString mSep = ";";

};

#endif // CONFIGURACOES_H
