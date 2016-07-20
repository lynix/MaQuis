/* Copyright 2016 Alexander Koch <lynix47@gmail.com>
 *
 * This file is part of 'MaQuis'. 'MaQuis' is distributed under the
 * terms of the MIT License, see file LICENSE.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>

extern "C" {
#include <turbojpeg.h>
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QStringList &flist, QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        QStringList &files;
        int currentIndex;
        qreal zoom;
        QSignalMapper *rotationMapper;

        void updateStatus(int width, int height, qreal zoom);

    private slots:
        void loadImage();
        void nextImage();
        void prevImage();
        void rotate(int op);
};

#endif // MAINWINDOW_H
