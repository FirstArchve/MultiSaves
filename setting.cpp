#include "setting.h"
#include "ui_setting.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDirIterator>
#include <QSettings>

setting::setting(QWidget *parent) : QDialog(parent),ui(new Ui::setting)
{
    ui->setupUi(this);
    //读取配置文件
    QSettings *configIniWrite = new QSettings(QCoreApplication::applicationDirPath() + "/cfg.ini", QSettings::IniFormat);
    if(configIniWrite->value("/Setting/DefaultDir").toString() == "0")
    {
        ui->DirRB1->setChecked(true);
    }
    else if(configIniWrite->value("/Setting/DefaultDir").toString() == "1")
    {
        ui->DirRB2->setChecked(true);
    }
    else if(configIniWrite->value("/Setting/DefaultDir").toString() == "2")
    {
        ui->DirRB3->setChecked(true);
        ui->GamePath->setText(configIniWrite->value("/Index/GamePath").toString());
        ui->OrderPath->setText(configIniWrite->value("/Index/OrderPath").toString());
    }
}

setting::~setting()
{
    delete ui;
}

//自定义单选框的开关变化事件
void setting::on_DirRB3_toggled(bool checked)
{
    //根据单选框的状态控制按钮是否可用
    if(checked)
    {
        ui->OpenGameDir->setEnabled(true);
        ui->OpenOrderDir->setEnabled(true);
    }
    else
    {
        ui->OpenGameDir->setEnabled(false);
        ui->OpenOrderDir->setEnabled(false);
    }
}

//游戏核心文件夹Open按钮的单击事件
void setting::on_OpenGameDir_clicked()
{
    //推出文件夹打开对话框并检测文件夹是否合法
    QString Path = QFileDialog::getExistingDirectory ( 0, "选择游戏核心文件夹","",QFileDialog::ShowDirsOnly);
    QDir Dir = Path + "/saves";
    if(!Dir.exists())
    {
        QMessageBox::critical(this, tr("错误"), tr("这不是一个正常的游戏核心文件夹!"));
        return ;
    }
    ui->GamePath->setText(Path);
}

//备份文件夹Open按钮的单击事件
void setting::on_OpenOrderDir_clicked()
{
    //推出打开文件夹对话框并将文件夹路径显示在文本框中
    ui->OrderPath->setText(QFileDialog::getExistingDirectory ( 0, "选择备份存放文件夹","",QFileDialog::ShowDirsOnly));
}

//Apply按钮的单击事件
void setting::on_apply_clicked()
{
    //将设置保存至配置文件中
    QSettings *configIniWrite = new QSettings(QCoreApplication::applicationDirPath() + "/cfg.ini", QSettings::IniFormat);
    if(ui->DirRB1->isChecked())
    {
        configIniWrite->setValue("/Setting/DefaultDir","0");
    }
    else if(ui->DirRB2->isChecked())
    {
        configIniWrite->setValue("/Setting/DefaultDir","1");
    }
    else if(ui->DirRB3->isChecked())
    {
        QDir GameDir = ui->GamePath->text() + "/saves";
        QDir OrderDir = ui->GamePath->text();
        //判断路径是否合法
        if(!GameDir.exists())
        {
            QMessageBox::critical(this, tr("错误"), tr("这不是一个正常的游戏核心文件夹!"));
            return ;
        }
        if(!OrderDir.exists())
        {
            QMessageBox::critical(this, tr("错误"), tr("这不是一个合法的文件夹！"));
            return ;
        }
        configIniWrite->setValue("/Setting/DefaultDir","2");
        configIniWrite->setValue("/Index/GamePath",ui->GamePath->text());
        configIniWrite->setValue("/Index/OrderPath",ui->OrderPath->text());
    }

    //关闭窗口
    accept();
}

//close按钮的单击事件
void setting::on_close_clicked()
{
    //关闭窗口
    accept();
}

