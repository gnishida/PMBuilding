#include "ControlWidget.h"
#include <QFileDialog>
#include "MainWindow.h"
#include "GLWidget3D.h"
#include "VBOGeoBuilding.h"

ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);

	ui.lineEditColorRed->setText("192");
	ui.lineEditColorGreen->setText("192");
	ui.lineEditColorBlue->setText("192");
	ui.spinBoxRoofTextureId->setValue(0);

	connect(ui.pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generate()));

	hide();	
}

void ControlWidget::generate() {
	mainWin->glWidget->vboRenderManager.removeStaticGeometry("3d_building");
	mainWin->glWidget->vboRenderManager.removeStaticGeometry("3d_building_fac");

	Building building;
	building.bldType = 1;
	building.color = QColor(ui.lineEditColorRed->text().toInt(), ui.lineEditColorGreen->text().toInt(), ui.lineEditColorBlue->text().toInt());
	building.footprint.push_back(QVector3D(-10, -10, 0));
	building.footprint.push_back(QVector3D(10, -10, 0));
	building.footprint.push_back(QVector3D(10, 10, 0));
	building.footprint.push_back(QVector3D(-10, 10, 0));
	building.numStories = 10;
	building.roofTextureId = ui.spinBoxRoofTextureId->value();

	VBOGeoBuilding::generateBuilding(mainWin->glWidget->vboRenderManager, building);

	mainWin->glWidget->shadow.makeShadowMap(mainWin->glWidget);
	mainWin->glWidget->updateGL();
}
