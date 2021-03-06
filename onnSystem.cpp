#include "onnSystem.h"

extern onnChecker      *blockChecker;
extern onnContract     *blockContract;
extern onnDatabase     *blockDatabase;
extern onnHttpd        *blockHttpd;
extern onnUdpd         *blockUdpd;
extern onnWebsocketd   *blockWebsocketd;

extern QTimer *timerCustom;

onnSystem::onnSystem():onnObject("onnSystem"){

}

void onnSystem::initBoss(){

}

void onnSystem::onInitFinish(){
    emit doStart();
}

void onnSystem::onChangeTimer(QByteArray,QByteArray pSecond){
    if(pSecond.toInt()<=0){
        timerCustom->stop();
        return;
    }
    timeoutStep = pSecond.toInt();
    timerCustom->stop();
    timerCustom->start(timeoutStep);
}

void onnSystem::onStartFinish(){
    if(!getReadableContract().isEmpty()){
        BUG << "fatal error: getReadableContract() != null";
        exit(-1);
    }else{
        BUG << "ok";
    }
    CONN(blockHttpd,SIGNAL(doBlockNew(QByteArray)),blockChecker,SLOT(onBlockNew(QByteArray)));
    CONN(blockHttpd,SIGNAL(doChangeTimer(QByteArray,QByteArray)),this,SLOT(onChangeTimer(QByteArray,QByteArray)));
    CONN(blockHttpd,SIGNAL(doChangeTimer(QByteArray,QByteArray)),blockContract,SLOT(onChangeTimer(QByteArray,QByteArray)));

    if(!getArgument("-ws").isEmpty()){
        CONN(blockContract,SIGNAL(doBroadcastAppNew(QByteArray)),blockWebsocketd,SLOT(onBroadcastAppNew(QByteArray)));
    }
    CONN(blockContract,SIGNAL(doSetBossList(QByteArrayList)),getNetSync(),SLOT(onGetBossAddr(QByteArrayList)));
    CONN(blockContract,SIGNAL(doCustomBroadcast(QString,QString,QString)),getNetSync(),SLOT(onOnnBroadcast(QString,QString,QString)));
    CONN(blockContract,SIGNAL(doCustomRequire(QString,QByteArray,QString,QString)),getNetSync(),SLOT(onOnnRequire(QString,QByteArray,QString,QString)));

    CONN(blockUdpd,SIGNAL(doSetBossList(QByteArrayList)),getNetSync(),SLOT(onGetBossAddr(QByteArrayList)));
    CONN(blockUdpd,SIGNAL(doCustomBroadcast(QString,QString,QString)),getNetSync(),SLOT(onOnnBroadcast(QString,QString,QString)));
    CONN(blockUdpd,SIGNAL(doCustomRequire(QString,QByteArray,QString,QString)),getNetSync(),SLOT(onOnnRequire(QString,QByteArray,QString,QString)));
    CONN(blockUdpd,SIGNAL(doBlockNew(QByteArray)),blockChecker,SLOT(onBlockNew(QByteArray)));

    CONN(getNetSync(),SIGNAL(doUpdatePeerList(QStringList,QStringList,QStringList)),blockUdpd,SLOT(onUdpdPeer(QStringList,QStringList,QStringList)));
    CONN(getNetSync(),SIGNAL(doOnnRequire(QString,QString,QString,QString)),blockUdpd,SLOT(onCustomRequire(QString,QString,QString,QString)));
    /*concurrent*/
    CONN(blockUdpd,SIGNAL(doDeployNew(QByteArray)),blockContract,SLOT(onDeployNew(QByteArray)));
    CONN(blockUdpd,SIGNAL(doMethodNew(QByteArray)),blockContract,SLOT(onMethodNew(QByteArray)));
    CONN(blockUdpd,SIGNAL(doPeerNew(QByteArray)),blockContract,SLOT(onPeerNew(QByteArray)));
    CONN(blockUdpd,SIGNAL(doDestroyNew(QByteArray)),blockContract,SLOT(onDestroyNew(QByteArray)));

    CONN(blockHttpd,SIGNAL(doDeployNew(QByteArray)),blockContract,SLOT(onDeployNew(QByteArray)));
    CONN(blockHttpd,SIGNAL(doMethodNew(QByteArray)),blockContract,SLOT(onMethodNew(QByteArray)));
    CONN(blockHttpd,SIGNAL(doPeerNew(QByteArray)),blockContract,SLOT(onPeerNew(QByteArray)));
    CONN(blockHttpd,SIGNAL(doDestroyNew(QByteArray)),blockContract,SLOT(onDestroyNew(QByteArray)));

    if(!getArgument("-t").isEmpty() && !getArgument("-s").isEmpty()){
        timerCustom->start(timeoutStep);
    }
}
