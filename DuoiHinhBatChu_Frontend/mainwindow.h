
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// For database connecting
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showRegisterWidget();
    void showLoginWidget();
    void showRoomWidget(QString sessionId);
    QString sessionId;

signals:
    void dispatchSessionId(QString sessionId);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
};

#endif // MAINWINDOW_H
