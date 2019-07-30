#ifndef BODYLAYOUT_H
#define BODYLAYOUT_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMenuBar>

#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QLine>
#include <QFrame>
#include <QUrl>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

#include <QTimer>


class bodyLayout:public QGridLayout {

public:
    bodyLayout();
};

class line : public QFrame{
public:
    line();
};


#endif // BODYLAYOUT_H
