#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"
#include "ControlWidget.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;
	ControlWidget* controlWidget;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow() {}

public slots:
};

#endif // MAINWINDOW_H
