#include "configuracoes.h"
#include "ui_configuracoes.h"

Configuracoes::Configuracoes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracoes)
{
    ui->setupUi(this);

}

Configuracoes::~Configuracoes()
{
    delete ui;
}

void Configuracoes::on_pushButton_ok_clicked()
{
    emit pushButtonOkClicked(ui->lineEdit_host->text(), ui->spinBox->value());
    this->close();
}
