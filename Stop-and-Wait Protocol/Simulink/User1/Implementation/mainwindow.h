// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QScrollArea>
#include "udphandler.h"
#include "wrapper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const QString &localIP = QString(), int localPort = 0,
                        const QString &targetIP = QString(), int targetPort = 0);
    ~MainWindow();

signals:
    void addToQueue(QString message);

public slots:
    void showReceivedMessage(QString message);
    void messaageSent();

private slots:
    void sendMessage();     // Slot for sending messages

private:
    Ui::MainWindow *ui;          // Pointer to the UI
    QScrollArea *scrollArea;     // Scroll area for the chat display
    QVBoxLayout *chatLayout;     // Layout for dynamically added chat messages
    UdpHandler handler;
    Wrapper wrapper;

    QString getBotResponse(const QString &message); // Function to generate bot responses
    void addMessage(const QString &message, bool isUser); // Add message to the scrollable chat UI
};

#endif // MAINWINDOW_H
