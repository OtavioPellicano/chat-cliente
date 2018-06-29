#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include <QDialog>

namespace Ui {
class Configuracoes;
}

class Configuracoes : public QDialog
{
    Q_OBJECT

public:
    explicit Configuracoes(QWidget *parent = 0);
    ~Configuracoes();

signals:
    void pushButtonOkClicked(QString host, int porta);

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::Configuracoes *ui;
};

#endif // CONFIGURACOES_H
