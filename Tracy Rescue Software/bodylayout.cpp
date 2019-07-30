#include "bodylayout.h"
#include "serial.h"


bodyLayout::bodyLayout()
{
    QTimer *load_messages = new QTimer();
    Serial *usb = new Serial();

    QWebEnginePage *page = new QWebEnginePage();
    page->profile()->setHttpUserAgent("Mozilla/5.0 (Linux; U; Android 4.0.2; en-us; Galaxy Nexus Build/ICL53F) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
    page->load(QUrl("https://www.google.com/maps/"));

    QWebEngineView *view = new QWebEngineView();
    view->resize(600,1080);
    view->setPage(page);


    this->setColumnStretch(3,0);
    this->setColumnStretch(4,2);
    /*----------------------------------------------------------------------------------------*/
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(0, 0,0, 0));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QPixmap logo_pixmap("C:/Users/ajink/OneDrive/Pictures/Tracy_png_Red.png");
    QLabel *logo_label = new QLabel();
    logo_label->setPixmap(logo_pixmap);

    QLabel *window_info = new QLabel("Emergancy Response Terminal");
    QFont font("Arial",30,QFont::Bold);
    window_info->setFont(font);

    this->addWidget(logo_label,0,0,1,1);
    this->addWidget(window_info,0,1,1,1);
    this->addWidget(view,0,4,14,1);
    this->addWidget(line,1,0,1,2);

    /*----------------------------------------------------------------------------------------*/
    QLineEdit *send_command = new QLineEdit();
    send_command->setPlaceholderText("Send Command");

    QPushButton *send_command_btn = new QPushButton("Send Command");

    this->addWidget(send_command,2,0,1,1);
    this->addWidget(send_command_btn,2,1,1,1);

    /*----------------------------------------------------------------------------------------*/
    QFrame *line2 = new QFrame();
    line2->setObjectName(QString::fromUtf8("line"));
    line2->setGeometry(QRect(0,0,0,0));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    QLabel *row_title1 = new QLabel("Broadcast Message: (Max 10000 characters!)");
    this->addWidget(row_title1,4,0,1,1);
    this->addWidget(line2,4,1,1,1);

    QTextEdit *broadcast = new QTextEdit();
    broadcast->setPlaceholderText("Enter the text to be broadcasted!");
    this->addWidget(broadcast,5,0,1,2);


    QPushButton *send_broadcast = new QPushButton("Send Broadcast Message");
    this->addWidget(send_broadcast,6,0,1,2);


    QObject::connect(send_broadcast,&QPushButton::clicked,[=](){
        usb->writeMessages(broadcast->toPlainText());
    });


    /*----------------------------------------------------------------------------------------*/
    QFrame *line3 = new QFrame();
    line3->setObjectName(QString::fromUtf8("line"));
    line3->setGeometry(QRect(0,0,0,0));
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);

    QLabel *single_recepient = new QLabel("Single Recepient: (Development in progress!!)");
    this->addWidget(single_recepient,7,0,1,1);
    this->addWidget(line3,7,1,1,1);

    QLineEdit *recepient_no = new QLineEdit();
    recepient_no->setPlaceholderText("Enter Recepient's registered mobile or telephone no.");
    recepient_no->setDisabled(true);

    QTextEdit *enter_msg_recepient = new QTextEdit();
    enter_msg_recepient->setPlaceholderText("Enter your message here.");
    enter_msg_recepient->setDisabled(true);

    QPushButton *send_message = new QPushButton("Send Message");
    send_message->setDisabled(true);

    this->addWidget(recepient_no,8,0,1,2);
    this->addWidget(enter_msg_recepient,9,0,1,2);
    this->addWidget(send_message,10,0,1,2);

    /*----------------------------------------------------------------------------------------*/

    QFrame *line4 = new QFrame();
    line4->setObjectName(QString::fromUtf8("line"));
    line4->setGeometry(QRect(0,0,0,0));
    line4->setFrameShape(QFrame::HLine);
    line4->setFrameShadow(QFrame::Sunken);

    QLabel *receiving = new QLabel("Receive Messages:");
    this->addWidget(receiving,11,0,1,1);
    this->addWidget(line4,11,1,1,1);

    QTextEdit *received_msgs = new QTextEdit();
    received_msgs->setPlaceholderText("You will see messages from civilians here.");
    this->addWidget(received_msgs,12,0,1,2);

    QObject::connect(load_messages,&QTimer::timeout,[=](){
        QString msg = usb->receivedMessages();
        QString street = usb->street_name;

        qDebug() << msg;
        if(msg != ""){
            received_msgs->append(msg);
            page->load(QUrl("https://www.google.com/maps/place/"+ street));
        }
    });

    load_messages->start(1000);


    /*----------------------------------------------------------------------------------------*/
    QFrame *line5 = new QFrame();
    line5->setObjectName(QString::fromUtf8("line"));
    line5->setGeometry(QRect(0,0,0,0));
    line5->setFrameShape(QFrame::HLine);
    line5->setFrameShadow(QFrame::Sunken);

    QLabel *about = new QLabel("Developed by Ajinkya Datalkar and Manoela Morais.");
    this->addWidget(line5,13,0,1,2);
    this->addWidget(about,14,0,1,2);
}
