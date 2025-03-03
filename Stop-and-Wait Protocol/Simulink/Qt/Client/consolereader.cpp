#include "consolereader.h"
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject *parent) : QObject(parent){}

void ConsoleReader::readInput()
{
    qDebug() << "Enter a string (or type 'exit' to quit):";
    QString input;
    input = QTextStream(stdin).readLine();

    // Check for exit command
    if (input.trimmed().compare("exit", Qt::CaseInsensitive) == 0) {
        qDebug() << "Exiting application...";
        QCoreApplication::exit(0);  // Exit the application with status code 0
        return;
    }

    QByteArray utf8Bytes = input.toUtf8();
    for (auto byte : utf8Bytes) {
        globalQueue.enqueue(static_cast<uint8_t>(byte));
    }
}
