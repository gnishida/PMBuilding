#include "MainWindow.h"
#include "VBOGeoBuilding.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// register the menu's action handlers
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionGenerateBuilding, SIGNAL(triggered()), this, SLOT(onGenerateBuilding()));

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

void MainWindow::onGenerateBuilding() {
	glWidget->vboRenderManager.removeStaticGeometry("3d_building");
	glWidget->vboRenderManager.removeStaticGeometry("3d_building_fac");
			
	Building building;
	building.bldType = 1;
	building.buildingFootprint.push_back(QVector3D(-10, -10, 0));
	building.buildingFootprint.push_back(QVector3D(10, -10, 0));
	building.buildingFootprint.push_back(QVector3D(10, 10, 0));
	building.buildingFootprint.push_back(QVector3D(-10, 10, 0));
	building.numStories = 10;

	VBOGeoBuilding::generateBuilding(glWidget->vboRenderManager, building);

	glWidget->shadow.makeShadowMap(glWidget);
	glWidget->updateGL();
}


