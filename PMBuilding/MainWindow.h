#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow() {}

public slots:
	void onGenerateBuilding();

private:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;
};

#endif // MAINWINDOW_H
