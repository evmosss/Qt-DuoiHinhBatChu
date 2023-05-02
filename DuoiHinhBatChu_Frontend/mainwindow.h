
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// For database connecting
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QCloseEvent>

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
    void showRoomWidget(QString sessionId, int userId);
    QString sessionId;

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void dispatchSessionId(QString sessionId, int userId);
    void closeWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
};

#endif // MAINWINDOW_H
