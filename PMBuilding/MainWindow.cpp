#include "MainWindow.h"
#include "VBOPm.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// register the menu's action handlers
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionGenerateBuilding, SIGNAL(triggered()), this, SLOT(onGenerateBuilding()));

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onGenerateBuilding() {
	VBOPm::generateBuildings(glWidget->vboRenderManager);
	glWidget->shadow.makeShadowMap(glWidget);
	glWidget->updateGL();
}


