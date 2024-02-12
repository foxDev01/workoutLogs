#include "mainwindow.h"
#include <QTableWidget>
#include "QSettings"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"




void loadTableDataFromSettings(const QString& groupName, QTableWidget* tableWidget)
{
    QSettings settings;
    settings.beginGroup(groupName);

    // Загружаем данные в таблицу
    int rowCount = settings.value("rowCount", 0).toInt();
    int columnCount = settings.value("columnCount", 0).toInt();
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(columnCount);

    for (int row = 0; row < rowCount; ++row)
    {
        for (int column = 0; column < columnCount; ++column)
        {
            QString key = QString("item_%1_%2").arg(row).arg(column);
            QString value = settings.value(key).toString();
            QTableWidgetItem* item = new QTableWidgetItem(value);
            tableWidget->setItem(row, column, item);
        }
    }

    settings.endGroup();
}

void saveTableDataToSettings(const QString& groupName, QTableWidget* tableWidget)
{
    QSettings settings;
    settings.beginGroup(groupName);

    // Сохраняем данные из таблицы
    settings.setValue("rowCount", tableWidget->rowCount());
    settings.setValue("columnCount", tableWidget->columnCount());

    for (int row = 0; row < tableWidget->rowCount(); ++row)
    {
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item)
            {
                settings.setValue(QString("item_%1_%2").arg(row).arg(column), item->text());
            }
        }
    }

    settings.endGroup();
}






// функция для сохранения в csv
void saveTableDataToCSV(const QString& filename, QTableWidget* tableWidget)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream stream(&file);

        // Записываем заголовки столбцов
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            stream << tableWidget->horizontalHeaderItem(column)->text();
            if (column < tableWidget->columnCount() - 1)
                stream << ",";
        }
        stream << "\n";

        // Записываем данные из ячеек таблицы
        for (int row = 0; row < tableWidget->rowCount(); ++row)
        {
            for (int column = 0; column < tableWidget->columnCount(); ++column)
            {
                QTableWidgetItem* item = tableWidget->item(row, column);
                if (item)
                {
                    stream << item->text();
                }
                if (column < tableWidget->columnCount() - 1)
                    stream << ",";
            }
            stream << "\n";
        }

        file.close();
        QMessageBox::information(0, "Сохранён", filename);
    }
    else
    {
        qDebug() << "Ошибка сохранения файла " << filename;
    }
}


// функция для сохранения в json
void saveTableDataToJson(const QString& filename, QTableWidget* tableWidget)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QJsonObject rootObject;

        // Добавляем заголовки столбцов в массив
        QJsonArray headersArray;
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            headersArray.append(tableWidget->horizontalHeaderItem(column)->text());
        }
        rootObject["headers"] = headersArray;

        // Добавляем данные ячеек в массив
        QJsonArray dataArray;
        for (int row = 0; row < tableWidget->rowCount(); ++row)
        {
            QJsonArray rowArray;
            for (int column = 0; column < tableWidget->columnCount(); ++column)
            {
                QTableWidgetItem* item = tableWidget->item(row, column);
                if (item)
                {
                    rowArray.append(item->text());
                }
            }
            dataArray.append(rowArray);
        }
        rootObject["data"] = dataArray;

        // Сохраняем JSON файл
        QJsonDocument document(rootObject);
        file.write(document.toJson());

        file.close();
    }
}


void saveTableDataToINI(const QString& filename, QTableWidget* tableWidget)
{
    QSettings settings(filename, QSettings::IniFormat);

    // Сохраняем данные из ячеек таблицы
    for (int row = 0; row < tableWidget->rowCount(); ++row)
    {
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item)
            {
                QString key = QString("Item%1_%2").arg(row).arg(column);
                QString value = item->text();
                settings.setValue(key, value);
            }
        }
    }
}



void loadTableDataFromINI(const QString& filename, QTableWidget* tableWidget)
{
    QSettings settings(filename, QSettings::IniFormat);

    // Очистка таблицы перед загрузкой данных
    tableWidget->clear();

    // Загрузка данных из INI файла в таблицу
    for (int row = 0; row < tableWidget->rowCount(); ++row)
    {
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            QString key = QString("Item%1_%2").arg(row).arg(column);
            if (settings.contains(key))
            {
                QString value = settings.value(key).toString();
                QTableWidgetItem* item = new QTableWidgetItem(value);
                tableWidget->setItem(row, column, item);
            }
        }
    }
}


// функция для открвтия  json
void loadTableDataFromJSON(const QString& filename, QTableWidget* tableWidget)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(stream.readAll().toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        // Получаем данные из поля "headers"
        QStringList headers;
        QJsonValue headersValue = jsonObj["headers"];
        if (headersValue.isArray())
        {
            QJsonArray headersArray = headersValue.toArray();
            for (const QJsonValue& headerValue : headersArray)
            {
                if (headerValue.isString())
                {
                    headers << headerValue.toString();
                }
            }
        }

        // Получаем данные из поля "data"
        QJsonValue dataValue = jsonObj["data"];
        if (dataValue.isArray())
        {
            QJsonArray dataArray = dataValue.toArray();
            tableWidget->setRowCount(dataArray.size()); // Устанавливаем количество строк в таблице

            int row = 0;
            for (const QJsonValue& rowValue : dataArray)
            {
                if (rowValue.isArray())
                {
                    QJsonArray rowArray = rowValue.toArray();
                    tableWidget->setColumnCount(rowArray.size()); // Устанавливаем количество столбцов в таблице

                    int column = 0;
                    for (const QJsonValue& cellValue : rowArray)
                    {
                        if (cellValue.isString())
                        {
                            QTableWidgetItem* item = new QTableWidgetItem(cellValue.toString());
                            tableWidget->setItem(row, column, item);
                        }
                        ++column;
                    }
                }
                ++row;
            }
        }

        file.close();
    }
}

