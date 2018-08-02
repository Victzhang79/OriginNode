#ifndef ONNSYSTEM_H
#define ONNSYSTEM_H

#include <QObject>
#include "onnObject.h"
#include "onnChecker.h"
#include "onnContract.h"
#include "onnDatabase.h"
#include "onnHttpd.h"
#include "onnUdpd.h"
#include "onnWebsocketd.h"

class onnSystem : public onnObject
{
    Q_OBJECT
public:
    explicit onnSystem();
    void initBoss();
signals:

public slots:
    void onInitFinish();
    void onStartFinish();
};

#endif // ONNSYSTEM_H
