#include "ControlWidget.h"
#include <QFileDialog>
#include "MainWindow.h"
#include "GLWidget3D.h"
#include "PMBuildingTower.h"
#include "PMBuildingHouse.h"
#include "PMBuildingFactory.h"
#include "PMBuildingSchool.h"

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
		building.footprint.push_back(QVector3D(-5, -8, 0));
		building.footprint.push_back(QVector3D(5, -8, 0));
		building.footprint.push_back(QVector3D(5, 8, 0));
		building.footprint.push_back(QVector3D(-5, 8, 0));
		PMBuildingHouse::generate(mainWin->glWidget->vboRenderManager, building);
	} else if (building.bldType == 1) {
		building.footprint.push_back(QVector3D(-10, -10, 0));
		building.footprint.push_back(QVector3D(10, -10, 0));
		building.footprint.push_back(QVector3D(10, 10, 0));
		building.footprint.push_back(QVector3D(-10, 10, 0));
		PMBuildingTower::generate(mainWin->glWidget->vboRenderManager, building);
	} else if (building.bldType == 2) {
		building.footprint.push_back(QVector3D(-20, -20, 0));
		building.footprint.push_back(QVector3D(20, -20, 0));
		building.footprint.push_back(QVector3D(20, 20, 0));
		building.footprint.push_back(QVector3D(-20, 20, 0));
		PMBuildingFactory::generate(mainWin->glWidget->vboRenderManager, building);
	} else if (building.bldType == 3) {
		building.footprint.push_back(QVector3D(-20, -20, 0));
		building.footprint.push_back(QVector3D(20, -20, 0));
		building.footprint.push_back(QVector3D(20, 20, 0));
		building.footprint.push_back(QVector3D(-20, 20, 0));
		PMBuildingSchool::generate(mainWin->glWidget->vboRenderManager, building);
	}

	mainWin->glWidget->shadow.makeShadowMap(mainWin->glWidget);
	mainWin->glWidget->updateGL();
}
