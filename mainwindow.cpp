#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <filecopy.h>
#include <QDirIterator>
#include <QSettings>
#include "setting.h"

//窗体初始化函数
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //读取配置文件
    QSettings *configIniWrite = new QSettings("./cfg.ini", QSettings::IniFormat);
    if(configIniWrite->value("/Setting/DefaultDir").toString() == "1")
    {
    }else{
        ui->GamePath->setText(configIniWrite->value("/Index/GamePath").toString());
        ui->OrderPath->setText(configIniWrite->value("/Index/OrderPath").toString());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//游戏核心文件夹的Open按钮单机事件
void MainWindow::on_Open_GamePath_clicked()
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

    //设置筛选，指遍历文件夹，NoDotAndDotDot 指不遍历自身及上层文件夹
    Dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList info_list =Dir.entryInfoList();
    //清空combobox
    ui->comboBox->clear();

    for(int i= 0 ; i< info_list.count(); i++)
    {
        QFileInfo info = info_list.at(i);
        if(info.isDir())
        {
            //找出下层目录的绝对路径
            QString sub_dir  = info.absoluteFilePath();
            //从路径中截取出文件夹名称（即世界名称）
            int ind=sub_dir.lastIndexOf("/");
            QString pathName=sub_dir.mid(ind+1);
            //将世界名称显示在combobox中
            ui->comboBox->addItem(pathName);
        }
    }
}

//备份存放文件夹的Open按钮单机事件
void MainWindow::on_Open_OrderPath_clicked()
{
    //推出打开文件夹对话框并将文件夹路径显示在文本框中
    ui->OrderPath->setText(QFileDialog::getExistingDirectory ( 0, "选择备份存放文件夹","",QFileDialog::ShowDirsOnly));
}

//Start! 按钮的单机事件
void MainWindow::on_start_clicked()
{
    FileCopy FileCopyer;

    //获取完整的备份路径
    QString ToDir = ui->OrderPath->text() + "/" + ui->comboBox->currentText();
    QDir Dir(ToDir);

    //检测备份文件夹是否存在，若不存在则创建，存在则清空
    if(!Dir.exists())
    {
        Dir.mkdir(ToDir);
    }
    else
    {
        QDirIterator DirsIterator(ToDir, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while(DirsIterator.hasNext())
        {
            // 删除文件操作如果返回否，那它就是目录
            if (!Dir.remove(DirsIterator.next()))
            {
                // 删除目录本身以及它下属所有的文件及目录
                QDir(DirsIterator.filePath()).removeRecursively();
            }
        }
    }

    //拷贝存档
    if(ui->SavesCheck->isChecked())
    {
        //创建saves文件夹
        QDir SavesDir(ToDir + "/saves");
        SavesDir.mkdir(ToDir + "/saves");
        //拷贝存档
        QString FromDir = ui->GamePath->text() + "/saves/" + ui->comboBox->currentText();
        FileCopyer.CopyDir(FromDir,ToDir + "/saves/" + ui->comboBox->currentText(),true);
    }

    //拷贝Xaero‘s map 地图数据
    if(ui->XmapCheck->isChecked())
    {
        //创建文件夹
        QDir XaeroWaypointsDir(ToDir + "/XaeroWaypoints");
        XaeroWaypointsDir.mkdir(ToDir + "/XaeroWaypoints");
        QDir XaeroWorldMapDir(ToDir + "/XaeroWorldMap");
        XaeroWorldMapDir.mkdir(ToDir + "/XaeroWorldMap");
        //拷贝文件夹
        QString XmapDir1 = ui->GamePath->text() + "/XaeroWaypoints/" + ui->comboBox->currentText();
        FileCopyer.CopyDir(XmapDir1,ToDir + "/XaeroWaypoints/" + ui->comboBox->currentText(),true);
        QString XmapDir2 = ui->GamePath->text() + "/XaeroWorldMap/" + ui->comboBox->currentText();
        FileCopyer.CopyDir(XmapDir2,ToDir + "/XaeroWorldMap/" + ui->comboBox->currentText(),true);
    }

    //拷贝mods
    if(ui->ModsCheck->isChecked())
    {
        //创建文件夹
        QDir ModsDir(ToDir + "/mods");
        ModsDir.mkdir(ToDir + "/mods");
        //拷贝文件夹
        QString ModsDir1 = ui->GamePath->text() + "/mods";
        FileCopyer.CopyDir(ModsDir1,ToDir + "/mods",true);
    }

    //拷贝options.txt
    if(ui->OptionsCheck->isChecked())
    {
        FileCopyer.CopyFile(ui->GamePath->text() + "/options.txt",ToDir,true);
    }

    //拷贝config
    if(ui->ConfigCheck->isChecked())
    {
        //创建文件夹
        QDir ConfigDir(ToDir + "/config");
        ConfigDir.mkdir(ToDir + "/config");
        //拷贝文件夹
        QString CfgDir = ui->GamePath->text() + "/config";
        FileCopyer.CopyDir(CfgDir,ToDir + "/config",true);
    }

    //将路径写入配置文件做下一次默认路径
    QSettings *configIniWrite = new QSettings("./cfg.ini", QSettings::IniFormat);
    if(configIniWrite->value("/Setting/DefaultDir").toString() == "2")
    {
    }else{
        configIniWrite->setValue("/Index/GamePath",ui->GamePath->text());
        configIniWrite->setValue("/Index/OrderPath",ui->OrderPath->text());
    }

    //弹出提示
    QMessageBox::information(this, tr("提示"), tr("备份已完成！"),QMessageBox::Ok, QMessageBox::Ok);
}

//菜单-文件-偏好设置 的单击事件
void MainWindow::on_action_triggered()
{
    setting * set=new setting();
    set->show();
}

//菜单-帮助-关于MultiSaves 的单击事件
void MainWindow::on_action_MultiSaves_triggered()
{
    QMessageBox::about(this, "关于MultiSaves", "版本：1.0 \n 作者：FirstArchve (Misaka11025)\n 编译日期:2021.7.23\n 作者网站：https://space.bilibili.com/520551427\n 协议：MIT ");
}

