#ifndef NETREQUEST_H
#define NETREQUEST_H

#include "Define.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

using namespace std;

class NetRequest : public QObject {
    Q_OBJECT
public:
    NetRequest(){

    }
    ~NetRequest(){}

signals:
    void doRsponse(QString);
public slots:
    void onStart(){
        mNetGet = new QNetworkAccessManager();
        mNetPost = new QNetworkAccessManager();
        connect(mNetGet, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onFinish(QNetworkReply*)),Qt::QueuedConnection);
    }
    void onGet(QString pUrl,QString pData){
        mNetGet->get(QNetworkRequest(pUrl+"/"+pData));
    }
    void onPost(QString pUrl,QString pData){
        QNetworkRequest qnr(pUrl);
        qnr.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        mNetPost->post(qnr,pData.toLatin1());
    }
    void onFinish(QNetworkReply *pReply){
        cout << QTime::currentTime().toString("ss zzz").toStdString() << endl;
        QByteArray data = pReply->readAll();
        emit doRsponse(data);
        pReply->deleteLater();
    }

private:
    QNetworkAccessManager *mNetGet;
    QNetworkAccessManager *mNetPost;
};

#endif // NETREQUEST_H
