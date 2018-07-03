#include "configuracoes.h"
#include "ui_configuracoes.h"

Configuracoes::Configuracoes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracoes)
{
    ui->setupUi(this);

    setup();
    carregarHostPorta();

}

Configuracoes::~Configuracoes()
{
    delete ui;
}

void Configuracoes::on_pushButton_ok_clicked()
{
    setHost(ui->lineEdit_host->text());
    setPorta(QString::number(ui->spinBox->value()));
    salvarHostPorta();

    emit pushButtonOkClicked(host(), porta().toInt());
    this->close();
}

QString Configuracoes::sep() const
{
    return mSep;
}

QString Configuracoes::nomeArq() const
{
    return mNomeArq;
}

QDir Configuracoes::dirConf() const
{
    return mDirConf;
}

QString Configuracoes::porta() const
{
    return mPorta;
}

void Configuracoes::setPorta(const QString &porta)
{
    mPorta = porta;
}

void Configuracoes::setup()
{
    mDirConf.mkdir("confg");
    mDirConf.cd("confg");
    mNomeArq =  "confg.csv";
}

void Configuracoes::carregarHostPorta()
{
    using namespace std;

    ifstream arq;
    QStringList strCsv;
    string str;

    arq.open(dirConf().absoluteFilePath(nomeArq()).toStdString());
    if(arq.is_open())
    {
        for(size_t i = 0; getline(arq, str); ++i)
        {
            if(i != 0)
            {
                strCsv = QString::fromStdString(str).split(sep());
                setHost(strCsv[0]);
                setPorta(strCsv[1]);
                break;
            }
        }
        arq.close();
    }
    else
    {
        setHost("127.0.0.1");
        setPorta("1312");
    }

    ui->lineEdit_host->setText(host());
    ui->spinBox->setValue(porta().toInt());

}

void Configuracoes::salvarHostPorta()
{
    using namespace std;
    ofstream arq(dirConf().absoluteFilePath(nomeArq()).toStdString());

    if(arq.is_open())
    {
        arq << QString("%1%2%3").arg("host").arg(sep()).arg("porta").toStdString() << endl;
        arq << QString("%1%2%3").arg(host()).arg(sep()).arg(porta()).toStdString() << endl;
        arq.close();
        qDebug() << "arquivo confg.csv salvo com sucesso";
    }
    else
    {
        qDebug() << "erro ao salvar confg.csv";
    }


}

QString Configuracoes::host() const
{
    return mHost;
}

void Configuracoes::setHost(const QString &host)
{
    mHost = host;
}
