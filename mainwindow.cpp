
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.cpp"
#include <QSettings>
#include <QDesktopServices>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_11->setText( QDate::currentDate().toString("dd_MM_yyyy"));
    ui->lineEdit_10->setText(QDir::rootPath());

   // ui->tableWidget->clear();
    // Далее, если нужно загрузить данные из файла:
    loadTableDataFromINI("data.ini", ui->tableWidget); // Загружаем данные из файла


}

MainWindow::~MainWindow()
{
    delete ui;
}


// добавляем значения в таблицу
void MainWindow::on_pushButton_2_pressed()
{
    QString exercise = ui->lineEdit->text();
    QString approach1 = ui->lineEdit_2->text()+ "/" + ui->lineEdit_3->text();
    QString approach2 = ui->lineEdit_4->text()+ "/" + ui->lineEdit_5->text();
    QString approach3 = ui->lineEdit_6->text()+ "/" + ui->lineEdit_7->text();
    QString approach4 = ui->lineEdit_8->text()+ "/" + ui->lineEdit_9->text();
    QString approach5 = ui->lineEdit_12->text()+ "/" + ui->lineEdit_13->text();



    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);


    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(exercise));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(approach1));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(approach2));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(approach3));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(approach4));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(approach5));

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();



}

void MainWindow::saveDataBeforeClosing() {
    saveTableDataToINI("data.ini", ui->tableWidget);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // В этой функции вы можете добавить код для сохранения данных или других действий перед закрытием окна
    // Например:
    saveTableDataToINI("data.ini", ui->tableWidget);

    // Вызов базовой реализации closeEvent, чтобы обеспечить корректное закрытие окна
    QMainWindow::closeEvent(event);
}




void MainWindow::on_pushButton_3_pressed()
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Выберите папку для сохранения");

    if (!folderPath.isEmpty()) {
        // Папка была выбрана
        ui->lineEdit_10->setText(folderPath);
        // Здесь можно сохранить отчеты в выбранную папку
    } else {
        // Папка не была выбрана или произошла ошибка
        QMessageBox::warning(this, "Выберите папку","Папка не выбрана");

    }
}

//удаляем строку из таблицы
void MainWindow::on_pushButton_4_pressed()
{

    int selectedRow =  ui->tableWidget->currentRow(); // получаем выбранную строку
    if (selectedRow != -1) { // если строка выбрана
         ui->tableWidget->removeRow(selectedRow); // удаляем выбранную строку
    }

}

// save to csv
void MainWindow::on_pushButton_pressed()
{
    QString path = ui->lineEdit_10->text();
    QString day = ui->lineEdit_11->text();
    saveTableDataToCSV(path + "/" + day + ".csv", ui->tableWidget);
//    saveTableDataToJson(path + "/" + day + ".json", ui->tableWidget);
}

// Открыть CSV-файл для чтения
void MainWindow::on_pushButton_5_pressed()
{
    // Открыть диалоговое окно выбора файла CSV
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл CSV");

    if (!fileName.isEmpty())
    {
         // Открыть CSV-файл для чтения
         QFile file(fileName);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
            QMessageBox::warning(this,"Ошибка открытия файла"," ");

            return;
         }

         // Очистить QTableWidget
         ui->tableWidget_2->clear();
         ui->tableWidget_2->setRowCount(0);
         ui->tableWidget_2->setColumnCount(0);

         // Прочитать данные из CSV-файла
         QTextStream in(&file);
         while (!in.atEnd())
         {
            QString line = in.readLine();
            QStringList fields = line.split(",");

            // Добавить новую строку в QTableWidget
            int row = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(row);

            // Установить количество столбцов в соответствии с данными из CSV-файла
            if (ui->tableWidget_2->columnCount() < fields.size())
                ui->tableWidget_2->setColumnCount(fields.size());

            // Заполнить ячейки значениями из CSV-файла
            for (int column = 0; column < fields.size(); ++column)
            {
                QTableWidgetItem *item = new QTableWidgetItem(fields[column]);
                ui->tableWidget_2->setItem(row, column, item);
            }
         }

         // Закрыть CSV-файл
         file.close();
    }
//    loadTableDataFromJSON(QFileDialog::getOpenFileName(this, "Выберите файл CSV"), ui->tableWidget_2);
}




// на открытие проводника
void MainWindow::on_pushButton_10_pressed()
{

}



void MainWindow::on_lineEdit_returnPressed()
{
    ui->lineEdit_2->setFocus();
}

// для изменнеия строки




