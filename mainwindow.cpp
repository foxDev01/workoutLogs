
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

//    loadTableDataFromINI("serushon.ini", ui->tableWidget);
//    saveTableDataToINI("serushon.ini", ui->tableWidget);

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

// добавление упражнениий на день




void MainWindow::on_pushButton_10_pressed()
{

}






void MainWindow::on_lineEdit_returnPressed()
{
    ui->lineEdit_2->setFocus();
}


void MainWindow::on_pushButton_6_pressed()
{
    int selectedRow =  ui->tableWidget->currentRow(); // получаем выбранную строку
    if (selectedRow != -1) { // если строка выбрана
        //ui->tableWidget->removeRow(selectedRow); // удаляем выбранную строку
         ui->lineEdit->setText(ui->tableWidget->item(selectedRow, 1)->text());
         ui->lineEdit_2->setText(ui->tableWidget->item(selectedRow, 2)->text());
         ui->lineEdit_3->setText(ui->tableWidget->item(selectedRow, 3)->text());
         ui->lineEdit_4->setText(ui->tableWidget->item(selectedRow, 4)->text());
         ui->lineEdit_5->setText(ui->tableWidget->item(selectedRow, 5)->text());
         ui->lineEdit_6->setText(ui->tableWidget->item(selectedRow, 6)->text());
         ui->lineEdit_7->setText(ui->tableWidget->item(selectedRow, 7)->text());
         ui->lineEdit_8->setText(ui->tableWidget->item(selectedRow, 8)->text());
         ui->lineEdit_9->setText(ui->tableWidget->item(selectedRow, 9)->text());
         ui->lineEdit_12->setText(ui->tableWidget->item(selectedRow, 10)->text());
         ui->lineEdit_13->setText(ui->tableWidget->item(selectedRow, 11)->text());




    }
}


void MainWindow::on_pushButton_7_clicked()
{
    // Подключаем базу данных
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("log.db");
    db.open();

    // Осуществляем запрос
    QSqlQuery query;
    query.exec("SELECT _id, Name, iteration1 FROM exercises");

    // Выводим значения из запроса
    while (query.next()) {
         QString _id = query.value(0).toString();
         QString Name = query.value(1).toString();
         QString iteration1 = query.value(2).toString();
         ui->textEdit->insertPlainText(_id+" "+Name+" "+iteration1+"\n");
    }
}

