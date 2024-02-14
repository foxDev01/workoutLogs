#include "mainwindow.h"
#include <QTableWidget>
#include "QSettings"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

/*
#include <QtAndroid>

void requestPermissions()
{
    QtAndroid::PermissionResultMap result = QtAndroid::requestPermissionsSync(
        QStringList() << "android.permission.READ_EXTERNAL_STORAGE"
                      << "android.permission.WRITE_EXTERNAL_STORAGE");

    if (result["android.permission.READ_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied
        || result["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
        // Пользователь отклонил разрешения
        // Обработайте это соответствующим образом
    }
}
*/
// функция для сохранения в csv
void saveTableDataToCSV(const QString& filename, QTableWidget* tableWidget)
{
    QFile file(filename);

    // Открываем файл для записи, если он существует, его содержимое будет перезаписано
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
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
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Ошибка открытия файла для записи: " << filename;
        return;
    }

    QTextStream stream(&file);

    // Сохраняем названия столбцов
    QStringList headers;
    for (int column = 0; column < tableWidget->columnCount(); ++column)
    {
        headers << tableWidget->horizontalHeaderItem(column)->text();
    }
    stream << "[General]\n";
    stream << "ColumnHeaders=" << headers.join(", ") << "\n\n";

    // Сохраняем данные из ячеек таблицы
    for (int row = 0; row < tableWidget->rowCount(); ++row)
    {
        stream << "[Row" << row << "]\n";
        for (int column = 0; column < tableWidget->columnCount(); ++column)
        {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item)
            {
                stream << "Column" << column << "=" << item->text() << "\n";
            }
        }
        stream << "\n";
    }

    file.close();
}


void loadTableDataFromINI(const QString& filename, QTableWidget* tableWidget)
{
    QSettings settings(filename, QSettings::IniFormat);

    // Проверяем, существует ли файл
    if (!QFile(filename).exists()) {
        qDebug() << "Файл не существует: " << filename;
        return; // Выходим из функции, так как данных для загрузки нет
    }

    // Очищаем таблицу перед загрузкой данных
    tableWidget->clear();

    // Загружаем названия столбцов
    QStringList headers = settings.value("ColumnHeaders").toStringList();
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    qDebug() << "Загрузка данных из файла INI";

    // Получаем список всех групп (строк)
    QStringList rowGroups = settings.childGroups();

    // Устанавливаем количество строк в таблице
    tableWidget->setRowCount(rowGroups.size());

    // Загружаем данные из INI файла в таблицу
    for (int i = 0; i < rowGroups.size(); ++i)
    {
        QString groupKey = rowGroups.at(i);
        settings.beginGroup(groupKey);
        for (int column = 0; column < headers.size(); ++column)
        {
            QString key = QString("Column%1").arg(column);
            QString value = settings.value(key).toString();
            QTableWidgetItem* item = new QTableWidgetItem(value);
            tableWidget->setItem(i, column, item);
            qDebug() << "Загружено значение" << value << "в строку" << i << "столбец" << column;
        }
        settings.endGroup();
    }

    qDebug() << "Загрузка данных завершена";
}



//необходмо разобраться с загрузкой данный в qtable/ мб по другому сохранять. такой же csv

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

