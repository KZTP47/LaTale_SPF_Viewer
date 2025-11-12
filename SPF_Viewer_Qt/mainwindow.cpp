#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene)
    , spf(nullptr)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->splitter->setSizes(QList<int>() << 100 << 300);
}

MainWindow::~MainWindow()
{
    delete spf;
    delete scene;
    delete ui;
}

void MainWindow::on_action_triggered()
{
    QFileDialog diag(this);
    diag.setFileMode(QFileDialog::ExistingFile);
    diag.setNameFilter(tr("SPF文件 (*.SPF)"));

    if (diag.exec()) {
        if (spf)
            delete spf;

        auto files = diag.selectedFiles();
        spf = new QFile(files.front());

        if (!spf->open(QIODevice::ReadOnly))
            QMessageBox::critical(this, tr("打开失败"), tr("打开失败：%1").arg(spf->fileName()));
    }

    on_action_2_triggered();
}

void MainWindow::on_action_2_triggered()
{
    if (!spf)
        return;

    struct entry { char name[128]; int offset, len, index; };
    spf->seek(spf->size() - 2*sizeof(entry));

    entry ent;
    spf->read((char*)&ent, sizeof(ent));
    int first_entry_offset = ent.offset + ent.len;

    spf->seek(first_entry_offset);
    contents.clear();
    ui->listWidget->clear();

    while (true) {
        spf->read((char*)&ent, sizeof(ent));

        if (ent.offset == 0 && ent.len == 0)
            break;

        contents.append(QPair(ent.offset, ent.len));
        ui->listWidget->addItem(QString(ent.name));
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        return;

    int index = ui->listWidget->row(current);
    QPair pair = contents.at(index);

    QPixmap pixmap;
    spf->seek(pair.first);
    if (pixmap.loadFromData(spf->read(pair.second))) {
        pixmap.setDevicePixelRatio(devicePixelRatio()); // fuck dpi scaling off
        scene->clear();
        scene->setSceneRect(pixmap.rect());
        scene->addPixmap(pixmap);
    }
}

void MainWindow::on_action_3_triggered()
{
    QApplication::exit();
}

