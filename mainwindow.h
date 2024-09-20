#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

enum class MoveDirection
{
    Up = -1,
    Down = 1
};

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

private:
    void moveItem(MoveDirection direction);

private slots:
    void on_actionExit_triggered();

    void on_actionAboutQt_triggered();

    void on_actionAbout_triggered();

    void on_addItemButton_clicked();

    void on_removeItemButton_clicked();

    void on_toDoListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_toDoListWidget_itemChanged(QListWidgetItem *item);

    void on_toDoListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_editItemButton_clicked();

    void on_moveUpItemButton_clicked();

    void on_moveDownItemButton_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
