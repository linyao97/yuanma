#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();
signals:
    void adminSignal();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::admin *ui;
};

#endif // ADMIN_H
