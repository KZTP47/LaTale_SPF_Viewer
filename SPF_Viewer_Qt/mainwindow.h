#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPair>
class QGraphicsScene;
class QFile;
class QListWidgetItem;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();
    void on_action_2_triggered();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_action_3_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QFile *spf;
    QVector<QPair<int, int>> contents;
};
#endif // MAINWINDOW_H
