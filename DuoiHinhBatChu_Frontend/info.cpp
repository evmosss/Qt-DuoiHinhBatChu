#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
}

Info::~Info()
{
    delete ui;
}

void Info::storeSessionId(QString sessionId)
{
    qInfo() << "Session" << sessionId;
    this->sessionId = sessionId;
}
