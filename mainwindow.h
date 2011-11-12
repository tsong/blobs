#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAIN_WINDOW_DEFAULT_WIDTH 500
#define MAIN_WINDOW_DEFAULT_HEIGHT 500

#include <QMainWindow>
#include <QUndoStack>
#include "displaywidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void createMenus();

protected:
    //surface which users interact with and draw the points
    DisplayWidget *m_displayWidget;
};

#endif // MAINWINDOW_H
