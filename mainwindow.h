#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAIN_WINDOW_DEFAULT_WIDTH 800
#define MAIN_WINDOW_DEFAULT_HEIGHT 600

#include <QMainWindow>
#include <QUndoStack>
#include "displaywidget.h"
#include "controlwidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //surface which users interact with and draw the points
    DisplayWidget *m_displayWidget;

    ControlWidget *m_controlWidget;
};

#endif // MAINWINDOW_H
