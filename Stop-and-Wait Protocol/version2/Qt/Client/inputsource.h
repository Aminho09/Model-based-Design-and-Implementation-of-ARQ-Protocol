#ifndef INPUTSOURCE_H
#define INPUTSOURCE_H

#include <QObject>
#include <QDebug>

class InputSource : public QObject
{
    Q_OBJECT
public:
    explicit InputSource(QObject *parent = nullptr);

signals:
    void inputReady(uint8_t ACK);

public slots:
    void generateInput(const QString &sender, quint16 senderPort, uint8_t ACK);
};

#endif // INPUTSOURCE_H
