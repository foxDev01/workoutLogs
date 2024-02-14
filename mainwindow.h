
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDate>
#include <QDebug>






QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveDataBeforeClosing();

private slots:
    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();

    void on_pushButton_pressed();

    void on_pushButton_5_pressed();

    void on_pushButton_10_pressed();

    void on_lineEdit_returnPressed();
protected:
    void closeEvent(QCloseEvent *event) override;
   // void openEvent(QCopenEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
