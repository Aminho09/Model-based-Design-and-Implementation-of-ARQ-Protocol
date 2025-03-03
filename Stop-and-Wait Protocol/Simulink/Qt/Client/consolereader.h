#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#include <QObject>
#include <QQueue>
#include <QDebug>

extern QQueue<uint8_t> globalQueue;

class ConsoleReader : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleReader(QObject *parent = nullptr);

public slots:
    void readInput();

private:
};

#endif // CONSOLEREADER_H
