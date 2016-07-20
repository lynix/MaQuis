/* Copyright 2016 Alexander Koch <lynix47@gmail.com>
 *
 * This file is part of 'MaQuis'. 'MaQuis' is distributed under the
 * terms of the MIT License, see file LICENSE.
 */

#include <iostream>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStringList>

#include "mainwindow.h"

#define PATTERN_JPEG "\\.jpe?g$"


void crawl(int argc, char *argv[], QStringList &list)
{
    QRegularExpression filePattern(PATTERN_JPEG,
                                   QRegularExpression::CaseInsensitiveOption);

    for (int i=1; i<argc; i++) {

        QFileInfo item(argv[i]);
        if (item.isFile()) {
            if (filePattern.match(item.fileName()).hasMatch())
                list.append(item.filePath());
        } else if (item.isDir()) {
            QDirIterator it(argv[i], QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QString next = it.next();
                if (filePattern.match(next).hasMatch())
                    list.append(next);
            }
        } else {
            std::cerr << "skipping non-JPEG item: '"
                      << item.filePath().toStdString() << "'" << std::endl;
        }

    }

}

void print_help()
{
    std::cout << "Usage: MaQuis [[FILE|DIRECTORY]...]" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Error: missing argument(s)" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }

    if (QString::compare(argv[1], "-h") == 0 ||
            QString::compare(argv[1], "--help") == 0)
    {
        print_help();
        exit(EXIT_SUCCESS);
    }

    QStringList files;
    crawl(argc, argv, files);
    files.sort();

    if (files.length() == 0) {
        std::cerr << "error: file list empy" << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef QT_DEBUG
    qDebug() << "found " << files.length() << " files";
    QListIterator<QString> it(files);
    while (it.hasNext())
        qDebug() << "list item: " << it.next();
#endif

    QApplication a(argc, argv);
    MainWindow w(files);
    w.show();

    return a.exec();
}
