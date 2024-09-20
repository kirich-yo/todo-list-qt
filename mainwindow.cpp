#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QtXml/QDomDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveItem(MoveDirection direction)
{
    int itemCount = ui->toDoListWidget->count();
    if (itemCount < 2 || ui->toDoListWidget->currentRow() == (direction == MoveDirection::Up ? 0 : itemCount - 1))
        return;

    int currentRow = ui->toDoListWidget->currentRow();
    QListWidgetItem * currentTask = ui->toDoListWidget->item(currentRow), * otherTask = ui->toDoListWidget->item(currentRow + static_cast<int>(direction));
    Qt::CheckState currentState = currentTask->checkState(), otherState = otherTask->checkState();
    QString currentText = currentTask->text(), otherText = otherTask->text();

    currentTask->setCheckState(otherState);
    currentTask->setText(otherText);

    otherTask->setCheckState(currentState);
    otherTask->setText(currentText);

    ui->toDoListWidget->setCurrentRow(currentRow + static_cast<int>(direction));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("This is a small pet project that allows you to keep a to-do list.");
    msgBox.exec();
}


void MainWindow::on_addItemButton_clicked()
{
    for (int row = 0; row < ui->toDoListWidget->count(); row++)
    {
        ui->toDoListWidget->closePersistentEditor(ui->toDoListWidget->item(row));
    }

    QListWidgetItem * taskItem = new QListWidgetItem("New Task");
    taskItem->setFlags(taskItem->flags() | Qt::ItemIsUserCheckable);
    taskItem->setCheckState(Qt::Unchecked);

    ui->toDoListWidget->addItem(taskItem);
    ui->toDoListWidget->openPersistentEditor(taskItem);
}


void MainWindow::on_removeItemButton_clicked()
{
    QListWidgetItem * taskItem = ui->toDoListWidget->takeItem(ui->toDoListWidget->currentRow());
    delete taskItem;
}


void MainWindow::on_toDoListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->toDoListWidget->openPersistentEditor(item);
}


void MainWindow::on_toDoListWidget_itemChanged(QListWidgetItem *item)
{
    ui->toDoListWidget->closePersistentEditor(item);

    bool isChecked = item->checkState();
    QFont taskFont = item->font();
    taskFont.setStrikeOut(isChecked);
    taskFont.setItalic(isChecked);
    item->setFont(taskFont);
}


void MainWindow::on_toDoListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->toDoListWidget->closePersistentEditor(previous);
}


void MainWindow::on_editItemButton_clicked()
{
    if (!ui->toDoListWidget->count())
        return;

    bool ok;
    QListWidgetItem * currentTask = ui->toDoListWidget->currentItem();
    QString taskText = QInputDialog::getText(this, "Edit task", "Task text:", QLineEdit::Normal,
                                             currentTask->text(), &ok);

    if (ok && !taskText.isEmpty())
        currentTask->setText(taskText);
}


void MainWindow::on_moveUpItemButton_clicked()
{
    moveItem(MoveDirection::Up);
}


void MainWindow::on_moveDownItemButton_clicked()
{
    moveItem(MoveDirection::Down);
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save a to-do list...", "", "XML Files (*.xml)");
    if (fileName.isEmpty())
        return;

    QDomDocument todoDoc;
    QFile todoFile(fileName);

    if (!todoFile.open(QIODevice::ReadWrite | QIODevice::Text))
        todoFile.close();

    QDomElement root = todoDoc.createElement("todo-list");
    todoDoc.appendChild(root);

    for (int i = 0; i < ui->toDoListWidget->count(); i++)
    {
        QListWidgetItem * taskItem = ui->toDoListWidget->item(i);
        QDomElement taskNode = todoDoc.createElement("task");
        QDomText taskText = todoDoc.createTextNode(taskItem->text());

        taskNode.setAttribute("isChecked", taskItem->checkState() ? "true" : "false");
        taskNode.appendChild(taskText);
        root.appendChild(taskNode);
    }

    QString xml = todoDoc.toString();
    QTextStream out(&todoFile);
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n" << xml.toLocal8Bit();
    todoFile.close();
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a to-do list...", "", "XML Files (*.xml)");
    if (fileName.isEmpty())
        return;

    QDomDocument todoDoc("todo-list");
    QFile todoFile(fileName);
    if (!todoFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!todoDoc.setContent(&todoFile))
    {
        todoFile.close();
        return;
    }

    for (int row = 0; row < ui->toDoListWidget->count(); row++)
    {
        QListWidgetItem * taskItem = ui->toDoListWidget->takeItem(row);
        delete taskItem;
    }

    QDomElement root = todoDoc.documentElement();
    for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            Qt::CheckState isChecked = (e.attribute("isChecked") == "true") ? Qt::Checked : Qt::Unchecked;
            QListWidgetItem * taskItem = new QListWidgetItem(e.text());
            taskItem->setFlags(taskItem->flags() | Qt::ItemIsUserCheckable);
            taskItem->setCheckState(isChecked);

            QFont taskFont = taskItem->font();
            taskFont.setStrikeOut(isChecked);
            taskFont.setItalic(isChecked);
            taskItem->setFont(taskFont);

            ui->toDoListWidget->addItem(taskItem);
        }
    }

    todoFile.close();
}
