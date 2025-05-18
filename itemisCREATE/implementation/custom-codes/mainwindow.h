// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendData(uint8_t character);
    void resetSend();
    void resetReceive();

public slots:
    void messaageSent();
    void sendNext();
    void receivedData(uint8_t data);

private slots:
    void sendMessage();     // Slot for sending messages

private:
    Ui::MainWindow *ui;          // Pointer to the UI
    QScrollArea *scrollArea;     // Scroll area for the chat display
    QVBoxLayout *chatLayout;     // Layout for dynamically added chat messages
    QQueue<uint8_t> queue;
    QString receivedMessage = "";

    void addMessage(const QString &message, bool isUser); // Add message to the scrollable chat UI
    void showReceivedMessage(QString message);
};

#endif // MAINWINDOW_H
