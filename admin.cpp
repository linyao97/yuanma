#include "admin.h"
#include "ui_admin.h"
QString loginname;
admin::admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin)
{
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_6->setVisible(false);
    QFile file("login.txt");
    QString logname;
    int i=0;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
    QTextStream in (&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        i++;
        if(i==1)
        {
            logname=line;
        }
    }
    logname=logname.right(logname.length()-5);
    ui->lineEdit_2->setText(logname);
}

admin::~admin()
{
    delete ui;
}
void admin::on_pushButton_2_clicked()
{
    this->close();
}

void admin::on_pushButton_clicked()
{
    int i=0;
    QString name,pass,logname,logpass;
    QFile file("login.txt");
    name="name:";
    name +=ui->lineEdit_2->text();
    loginname +=ui->lineEdit_2->text();
    pass="password:";
    pass +=ui->lineEdit->text();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
    QTextStream in (&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        i++;
        if(i==1)
        {
            logname=line;
        }
        if(i==2)
        {
            logpass=line;
        }
    }
    if(logname==name and logpass==pass)
    {
        emit adminSignal();
        ui->lineEdit->clear();
        this->close();
    }
    else
    {
        QMessageBox::about(NULL,"提示","密码或账户错误");
    }

}

void admin::on_pushButton_3_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_5->setVisible(true);
    ui->label->setText("旧用户：");
    ui->label_2->setText("旧密码：");
}

void admin::on_pushButton_5_clicked()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_3->setVisible(true);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_5->setVisible(false);
    ui->label->setText("用户：");
    ui->label_2->setText("密码：");
}

void admin::on_pushButton_4_clicked()
{
    int i=0;
    QString name,pass,logname,logpass;
    QFile file("login.txt");
    name="name:";
    name +=ui->lineEdit_2->text();
    loginname +=ui->lineEdit_2->text();
    pass="password:";
    pass +=ui->lineEdit->text();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
    QTextStream in (&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        i++;
        if(i==1)
        {
            logname=line;
        }
        if(i==2)
        {
            logpass=line;
        }
    }
    if(logname==name and logpass==pass)
    {
        ui->label->setText("新用户：");
        ui->label_2->setText("新密码：");
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_4->setVisible(false);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_6->setVisible(true);
        ui->lineEdit->clear();
    }
    else
    {
        QMessageBox::about(NULL,"提示","密码或账户错误");
    }
}

void admin::on_pushButton_6_clicked()
{
    QFile file("login.txt");
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
       QMessageBox::information(this,tr("打开文件失败！"),file.errorString());
       return;
    }
    QString newname = "name:";
    QString newpass = "password:";
    newname += ui->lineEdit_2->text();
    file.write(newname.toUtf8());
    file.write("\n");

    newpass += ui->lineEdit->text();
    file.write(newpass.toUtf8());
    file.write("\n");

    file.close();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->label->setText("用户：");
    ui->label_2->setText("密码：");
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_6->setVisible(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_3->setVisible(true);
}
