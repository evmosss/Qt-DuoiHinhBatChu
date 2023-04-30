#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QString>

namespace Ui {
class Info;
}

class Info : public QWidget
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();
    QString sessionId;
    void storeSessionId(QString sessionId);

private:
    Ui::Info *ui;
};

#endif // INFO_H
