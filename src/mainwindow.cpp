/* Copyright 2016 Alexander Koch <lynix47@gmail.com>
 *
 * This file is part of 'MaQuis'. 'MaQuis' is distributed under the
 * terms of the MIT License, see file LICENSE.
 */

#include <iostream>
#include <QDebug>
#include <QFile>
#include <QShortcut>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList &flist, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    files(flist),
    currentIndex(0),
    zoom(1.0),
    rotationMapper(NULL)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));

    connect(ui->btnFileReload, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(ui->btnFileNext, SIGNAL(clicked()), this, SLOT(nextImage()));
    connect(ui->btnFilePrev, SIGNAL(clicked()), this, SLOT(prevImage()));

    rotationMapper = new QSignalMapper(this);
    connect(ui->btnImgRotLeft, SIGNAL(clicked()), rotationMapper, SLOT(map()));
    connect(ui->btnImgRotFlip, SIGNAL(clicked()), rotationMapper, SLOT(map()));
    connect(ui->btnImgRotRight, SIGNAL(clicked()), rotationMapper, SLOT(map()));
    rotationMapper->setMapping(ui->btnImgRotLeft, TJXOP_ROT270);
    rotationMapper->setMapping(ui->btnImgRotFlip, TJXOP_ROT180);
    rotationMapper->setMapping(ui->btnImgRotRight, TJXOP_ROT90);
    connect(rotationMapper, SIGNAL(mapped(int)), this, SLOT(rotate(int)));

    new QShortcut(QKeySequence(Qt::Key_J), this, SLOT(nextImage()));
    new QShortcut(QKeySequence(Qt::Key_K), this, SLOT(prevImage()));
    new QShortcut(QKeySequence(Qt::Key_R), this, SLOT(loadImage()));

    if (files.length() > 1)
        ui->btnFileNext->setEnabled(true);

    loadImage();
}

MainWindow::~MainWindow()
{
    delete rotationMapper;
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::loadImage()
{
    const QString &file = files.at(currentIndex);
    QPixmap item(file);

    // replace image
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(item);

    // update zoom factor to fit scene
    qreal factorX = (ui->graphicsView->rect().width()-(qreal)2) / (qreal)item.width();
    qreal factorY = (ui->graphicsView->rect().height()-(qreal)2) / (qreal)item.height();
    qreal zoom = factorX < factorY ? factorX : factorY;
    if (zoom != this->zoom) {
        ui->graphicsView->scale((qreal)1/this->zoom*zoom, (qreal)1/this->zoom*zoom);
        this->zoom = zoom;
    }

    updateStatus(item.width(), item.height(), zoom);
}

void MainWindow::nextImage()
{
    if (currentIndex == files.length()-1)
        return;

    currentIndex++;

    if (currentIndex + 1 == files.length())
        ui->btnFileNext->setEnabled(false);
    if (currentIndex == 1)
        ui->btnFilePrev->setEnabled(true);

    loadImage();
}

void MainWindow::prevImage()
{
    if (currentIndex == 0)
        return;

    currentIndex--;

    if (currentIndex == 0)
        ui->btnFilePrev->setEnabled(false);
    if (currentIndex == files.length()-2)
        ui->btnFileNext->setEnabled(true);

    loadImage();
}

void MainWindow::updateStatus(int width, int height, qreal zoom)
{
    QString msg("(%1/%2) %3 (%4x%5) [%6%]");
    msg = msg.arg(currentIndex + 1).arg(files.length());
    msg = msg.arg(files.at(currentIndex));
    msg = msg.arg(width).arg(height);
    msg = msg.arg((int)(zoom*100));

    ui->statusBar->showMessage(msg);
}

void MainWindow::rotate(int op)
{
    const QString &file = files.at(currentIndex);

    // read source file
    QFile qfile(file);
    if (!qfile.open(QIODevice::ReadOnly)) {
        std::cerr << "failed to open '" << file.toStdString() << "' for reading"
                  << std::endl;
        return;
    }
    QByteArray bytes = qfile.readAll();
    qfile.close();
    if (bytes.isEmpty()) {
        std::cerr << "empty buffer after reading file" << std::endl;
        return;
    }

    // initialize libjpeg-turbo transforation
    tjhandle tjh = tjInitTransform();
    if (tjh == NULL) {
        std::cerr << "failed to initialize libturbojpeg: " << tjGetErrorStr()
                  << std::endl;
        return;
    }

    // perform transformation
    unsigned char *destBuf = NULL;
    unsigned long destSize = 0;
    tjtransform transform;
    transform.r = {0, 0, 0, 0};
    transform.op = op;
    transform.options = TJXOPT_TRIM;
    transform.data = NULL;
    transform.customFilter = NULL;
    if (tjTransform(tjh,
                    reinterpret_cast<const unsigned char *>(bytes.constData()),
                    bytes.length(),
                    1,
                    &destBuf,
                    &destSize,
                    &transform,
                    0) < 0) {
        std::cerr << "libjpeg-turbo transformation failed: " << tjGetErrorStr()
                  << std::endl;
        return;
    }

    // save transformation result
    if (!qfile.open(QIODevice::WriteOnly)) {
        std::cerr << "failed to open '" << file.toStdString() << "' for writing"
                  << std::endl;
        return;
    }
    if (qfile.write(reinterpret_cast<const char *>(destBuf), destSize) !=
            (qint64)destSize) {
        std::cerr << "failed to write '" << file.toStdString() << "'"
                  << std::endl;
        qfile.close();
        return;
    }
    qfile.close();

    // clean up libturbo-jpeg
    tjFree(destBuf);
    tjDestroy(tjh);

    // reload image
    loadImage();
}
