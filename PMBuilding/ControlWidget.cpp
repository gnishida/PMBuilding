#include "ControlWidget.h"
#include <QFileDialog>
#include "MainWindow.h"
#include "GLWidget3D.h"
#include "PMBuildingTower.h"
#include "PMBuildingHouse.h"
#include "PMBuildingFactory.h"
#include "PMBuildingSchool.h"
#include "PMBuildingRB.h"

ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);

	ui.spinBoxType->setValue(0);
	ui.spinBoxSubType->setValue(0);
	ui.lineEditColorRed->setText("192");
	ui.lineEditColorGreen->setText("192");
	ui.lineEditColorBlue->setText("192");
	ui.spinBoxRoofTextureId->setValue(0);
	ui.spinBoxNumStories->setValue(10);

	connect(ui.pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generate()));

	hide();	
}

void ControlWidget::generate() {
	mainWin->glWidget->vboRenderManager.removeStaticGeometry("3d_building");
	mainWin->glWidget->vboRenderManager.removeStaticGeometry("3d_building_fac");

	Building building;
	building.bldType = ui.spinBoxType->value();
	building.subType = ui.spinBoxSubType->value();
	building.color = QColor(ui.lineEditColorRed->text().toInt(), ui.lineEditColorGreen->text().toInt(), ui.lineEditColorBlue->text().toInt());
	building.roofTextureId = ui.spinBoxRoofTextureId->value();
	building.numStories = ui.spinBoxNumStories->value();

	if (building.bldType == 0) {
		building.buildingFootprint.push_back(QVector3D(-8, -8, 10));
		building.buildingFootprint.push_back(QVector3D(8, -8, 10));
		building.buildingFootprint.push_back(QVector3D(8, 8, 10));
		building.buildingFootprint.push_back(QVector3D(-8, 8, 10));
		PMBuildingHouse::generate(mainWin->glWidget->vboRenderManager, "3d_building", building);
	} else if (building.bldType == 1) {
		building.buildingFootprint.push_back(QVector3D(-10, -10, 0));
		building.buildingFootprint.push_back(QVector3D(10, -10, 0));
		building.buildingFootprint.push_back(QVector3D(10, 10, 0));
		building.buildingFootprint.push_back(QVector3D(-10, 10, 0));
		PMBuildingTower::generate(mainWin->glWidget->vboRenderManager, "3d_building", building);
	} else if (building.bldType == 2) {
		/*
		building.buildingFootprint.push_back(QVector3D(-20, -20, 0));
		building.buildingFootprint.push_back(QVector3D(20, -20, 0));
		building.buildingFootprint.push_back(QVector3D(20, 20, 0));
		building.buildingFootprint.push_back(QVector3D(-20, 20, 0));
		*/
		building.buildingFootprint.push_back(QVector3D(-1406.302734, 860.522888, 70));
		building.buildingFootprint.push_back(QVector3D(-1387.503784, 864.010620, 70));
		building.buildingFootprint.push_back(QVector3D(-1393.263062, 894.774536, 70));
		building.buildingFootprint.push_back(QVector3D(-1412.010864, 891.296692, 70));
		PMBuildingFactory::generate(mainWin->glWidget->vboRenderManager, "3d_building", building);
	} else if (building.bldType == 3) {
		building.buildingFootprint.push_back(QVector3D(-20, -20, 70));
		building.buildingFootprint.push_back(QVector3D(20, -20, 70));
		building.buildingFootprint.push_back(QVector3D(20, 20, 70));
		building.buildingFootprint.push_back(QVector3D(-20, 20, 70));
		PMBuildingSchool::generate(mainWin->glWidget->vboRenderManager, "3d_building", building);
	} else if (building.bldType == 4) {
		building.buildingFootprint.push_back(QVector3D(-20, -20, 0));
		building.buildingFootprint.push_back(QVector3D(20, -20, 0));
		building.buildingFootprint.push_back(QVector3D(20, 20, 0));
		building.buildingFootprint.push_back(QVector3D(-20, 20, 0));
		PMBuildingRB::generate(mainWin->glWidget->vboRenderManager, "3d_building", building);
	}

	mainWin->glWidget->shadow.makeShadowMap(mainWin->glWidget);
	mainWin->glWidget->updateGL();
}
