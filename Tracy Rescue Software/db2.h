#ifndef DB2_H
#define DB2_H

#include <QNetworkAccessManager>

class makeEntry : public QNetworkAccessManager{
public:
    makeEntry();
    QString name;
    QString phone;
    QString apartment;
    QString street;
    QString disablity;
    QString message;

    void setParams(QString n,QString p,QString s,QString a, QString d,QString m);
};


#endif // DB2_H
