#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Open_GamePath_clicked();

    void on_Open_OrderPath_clicked();

    void on_start_clicked();

    void on_action_triggered();

    void on_action_MultiSaves_triggered();

private:
    Ui::MainWindow *ui;

    QString GamePath;
};

#endif // MAINWINDOW_H
