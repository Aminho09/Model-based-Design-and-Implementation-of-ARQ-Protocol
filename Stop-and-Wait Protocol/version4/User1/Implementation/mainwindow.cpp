#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , handler(nullptr, "127.0.0.1", 12346, "127.0.0.1", 12345)
{
    ui->setupUi(this);

    // Set up the scrollable chat area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget(this);
    QVBoxLayout *chatLayout = new QVBoxLayout(scrollWidget);
    chatLayout->setAlignment(Qt::AlignTop); // Ensure messages align at the top
    scrollWidget->setLayout(chatLayout);
    scrollArea->setWidget(scrollWidget);

    // Add the scroll area to the main layout
    ui->verticalLayout->insertWidget(0, scrollArea);

    // Store the scroll area and layout in member variables
    this->scrollArea = scrollArea;
    this->chatLayout = chatLayout;

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // Connect send button to sendMessage function
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    QObject::connect(this, &MainWindow::addToQueue, &wrapper, &Wrapper::storeString);
    QObject::connect(&wrapper, &Wrapper::sendOutputReady, &handler, &UdpHandler::sendPacket);
    QObject::connect(&wrapper, &Wrapper::ackReady, &handler, &UdpHandler::sendAck);
    QObject::connect(&wrapper, &Wrapper::showMessage, this, &MainWindow::showReceivedMessage);
    QObject::connect(&wrapper, &Wrapper::messageSent, this, &MainWindow::messaageSent);
    QObject::connect(&handler, &UdpHandler::ackReceived, &wrapper, &Wrapper::sendAckCall);
    QObject::connect(&handler, &UdpHandler::packetReceived, &wrapper, &Wrapper::receivePacketCall);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addMessage(const QString &message, bool isUser)
{
    QLabel *messageLabel = new QLabel(this);
    messageLabel->setText(message);
    messageLabel->setWordWrap(true);

    QLabel *timeLabel = new QLabel(this);
    QString currentTime = QTime::currentTime().toString("hh:mm:ss.zzz"); // Include milliseconds
    timeLabel->setText(currentTime);
    timeLabel->setStyleSheet("font-size: 10px; color: gray; padding: 3px;");

    QVBoxLayout *messageWithTimeLayout = new QVBoxLayout;
    messageWithTimeLayout->addWidget(messageLabel);
    messageWithTimeLayout->addWidget(timeLabel);

    QHBoxLayout *messageLayout = new QHBoxLayout;

    if (isUser) {
        messageLabel->setStyleSheet("background-color: #0084ff; color: white; border-radius: 12px; padding: 10px;");
        messageLayout->addStretch();
        messageLayout->addLayout(messageWithTimeLayout);
    } else {
        messageLabel->setStyleSheet("background-color: #e4e6eb; color: black; border-radius: 12px; padding: 10px;");
        messageLayout->addLayout(messageWithTimeLayout);
        messageLayout->addStretch();
    }

    QWidget *messageWidget = new QWidget(this);
    messageWidget->setLayout(messageLayout);
    this->chatLayout->addWidget(messageWidget);

    // Scroll to the bottom to show the latest message
    this->scrollArea->verticalScrollBar()->setValue(this->scrollArea->verticalScrollBar()->maximum());
}

void MainWindow::sendMessage()
{
    QString message = ui->messageInput->text().trimmed();

    if (message.isEmpty()) {
        QMessageBox::warning(this, "Empty Message", "Please type a message before sending.");
        return;
    }

    // Add user message to the chat display
    addMessage(QString("<b>You:</b> %1").arg(message), true);
    ui->messageInput->clear();

    emit addToQueue(message);



//     Get bot response and simulate a delay before showing it
//    QString botResponse = getBotResponse(message);
//    QTimer::singleShot(500, [this, botResponse]() {
//        addMessage(QString("<b>Bot:</b> %1").arg(botResponse), false);
//    });

}

void MainWindow::showReceivedMessage(QString message){
    addMessage(QString("<b>Bot:</b> %1").arg(message), false);
}

void MainWindow::messaageSent(){
    ui->statusBar->showMessage("Message sent", 2000);
}

QString MainWindow::getBotResponse(const QString &message)
{
    // Simple predefined responses
    if (message.contains("hello", Qt::CaseInsensitive)) {
        return "Hello! How can I help you today?";
    } else if (message.contains("how are you", Qt::CaseInsensitive)) {
        return "I'm just a bot, but I'm doing great! Thanks for asking.";
    } else if (message.contains("time", Qt::CaseInsensitive)) {
        return QString("The current time is %1.").arg(QTime::currentTime().toString("hh:mm:ss"));
    } else if (message.contains("bye", Qt::CaseInsensitive)) {
        return "Goodbye! Have a great day!";
    } else {
        return "I'm not sure how to respond to that. Can you try asking something else?";
    }
}
