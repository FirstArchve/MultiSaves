#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = nullptr);
    ~setting();

private slots:

    void on_DirRB3_toggled(bool checked);

    void on_OpenGameDir_clicked();

    void on_OpenOrderDir_clicked();

    void on_apply_clicked();

    void on_close_clicked();

private:
    Ui::setting *ui;
};

#endif // SETTING_H
