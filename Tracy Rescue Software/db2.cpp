#include "db2.h"

makeEntry::makeEntry()
{
    if(this->get(QNetworkRequest(QUrl("http://localhost/makeEntry.php/?fname=" + name+ "&phone=" + phone + "&street=" + street + "&apartment=" + apartment + "&disablity=" + disablity + "&message=" + message)))){
        qDebug() << "Entry Successful!";
    }
    else{
        qDebug() << "Unsuccessful!";
    }
}

void makeEntry::setParams(QString n,QString p,QString s,QString a, QString d,QString m)
{
    name=n;
    phone=p;
    street=s;
    apartment=a;
    disablity=d;
    message=m;
}
