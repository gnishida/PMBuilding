/************************************************************************************************
*		Procedural City Generation: Buildings geometry
*		@author igarciad
************************************************************************************************/

#include "VBORenderManager.h"
#include "VBOGeoBuilding.h"
#include <QMatrix4x4>

#include "qdir.h"
const float storyHeight=4.2f;

using namespace boost::polygon::operators;
bool VBOGeoBuilding::bldgInitialized=false;

static std::vector<QString> facadeTex;
static std::vector<QVector3D> facadeScale;
static std::vector<QString> windowTex;
static std::vector<QString> roofTex;
static std::vector<QString> houseTex;

void VBOGeoBuilding::initBuildingsTex(){
	QString pathName="../data/textures/LC/";
	QStringList nameFilter;
	nameFilter << "*.png" << "*.jpg" << "*.gif";
	// 1. facade
	QDir directory(pathName+"facade/");
	QStringList list = directory.entryList( nameFilter, QDir::Files );
	for(int lE=0;lE<list.size();lE++){
		facadeTex.push_back(pathName+"/facade/"+list[lE]);
		QStringList scaleS=list[lE].split("_");
		//printf("*********** scaleS %d\n",scaleS.size());
		if(scaleS.size()!=4)
			facadeScale.push_back(QVector3D(1.0f,1.0f,0));
		else{
			facadeScale.push_back(QVector3D(scaleS[1].toFloat(),scaleS[2].toFloat(),0));
			//printf("Scale %s -->%f %f\n",list[lE].toAscii().constData(),scaleS[1].toFloat(),scaleS[2].toFloat());
		}
	}

	// 2. windows
	QDir directoryW(pathName+"wind/");
	list = directoryW.entryList( nameFilter, QDir::Files );
	for(int lE=0;lE<list.size();lE++){
		windowTex.push_back(pathName+"wind/"+list[lE]);
	}

	// 3. roof
	QDir directoryR(pathName+"roof/");
	list = directoryR.entryList( nameFilter, QDir::Files );
	for(int lE=0;lE<list.size();lE++){
		roofTex.push_back(pathName+"roof/"+list[lE]);
	}

	// 4. house
	houseTex.push_back("../data/textures/LC/house/wall.jpg");
	houseTex.push_back("../data/textures/LC/house/door.jpg");
	houseTex.push_back("../data/textures/LC/house/garage.jpg");
	houseTex.push_back("../data/textures/LC/house/roof.jpg");
	houseTex.push_back("../data/textures/LC/house/window.jpg");
	houseTex.push_back("../data/textures/LC/house/window2.jpg");

	bldgInitialized=true;
}

void calculateColumnContour(std::vector<QVector3D>& currentContour, std::vector<QVector3D>& columnContour) {
	QVector3D pos1,pos2;
	for(int sN=0;sN<currentContour.size();sN++){
		int ind1=sN;
		int ind2=(sN+1)%currentContour.size();
		pos1=currentContour[ind1];
		pos2=currentContour[ind2];
		QVector3D dirV=(pos2-pos1);
		float leng=(dirV).length();
		dirV/=leng;
		if(leng>7.0f){
			QVector3D perDir=QVector3D::crossProduct(dirV,QVector3D(0,0,1.0f));

			float remindingL=leng-1.0f-1.5f;
			int numWindows=remindingL/(3.0f+1.5f);
			float windowWidth=(leng-1.0f-1.5f*(1+numWindows))/numWindows;

			columnContour.push_back(pos1);
			//first col
			columnContour.push_back(pos1+0.5f*dirV);// first col
			columnContour.push_back(pos1+0.5f*dirV+0.8f*perDir);
			columnContour.push_back(pos1+(0.5f+1.5f)*dirV+0.8f*perDir);
			columnContour.push_back(pos1+(0.5f+1.5f)*dirV);
			QVector3D cPos=pos1+(0.5f+1.5f)*dirV;
			for(int nW=0;nW<numWindows;nW++){
				//window
				columnContour.push_back(cPos+(windowWidth)*dirV);
				//column
				columnContour.push_back(cPos+(windowWidth)*dirV+0.8f*perDir);
				columnContour.push_back(cPos+(windowWidth+1.5f)*dirV+0.8f*perDir);
				columnContour.push_back(cPos+(windowWidth+1.5f)*dirV);
				cPos+=dirV*(windowWidth+1.5f);
			}

		}else{
			columnContour.push_back(pos1);
		}
	}
}//

void addWindow(VBORenderManager& rendManager, int windowTexId, QVector3D initPoint,QVector3D dirR,QVector3D dirUp,float width,float height) {
	std::vector<Vertex> vertWind;

	float depth = 2.0f;
	// IN: TOP
	QVector3D perI = QVector3D::crossProduct(dirUp,dirR);//note direction: to inside
	QVector3D vert[8];
	vert[0]=initPoint;
	vert[1]=initPoint+perI*depth;
	vert[2]=initPoint+perI*depth+dirUp*height;
	vert[3]=initPoint+dirUp*height;

	vert[4]=initPoint+perI*depth+dirR*width;
	vert[5]=initPoint+dirR*width;
	vert[6]=initPoint+dirUp*height+dirR*width;
	vert[7]=initPoint+perI*depth+dirUp*height+dirR*width;

	QColor color = QColor(0.5f,0.5f,0.5f);
	// LEFT
	QVector3D norm;
	norm=QVector3D::crossProduct(vert[1]-vert[0],vert[3]-vert[0]);
	vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));
	// RIGHT
	norm=QVector3D::crossProduct(vert[5]-vert[4],vert[7]-vert[4]);
	vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
	// TOP
	norm=QVector3D::crossProduct(vert[7]-vert[2],vert[3]-vert[2]);
	vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));
	// BOT
	norm=QVector3D::crossProduct(vert[5]-vert[0],vert[1]-vert[0]);
	vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
	vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
	rendManager.addStaticGeometry("3d_building", vertWind, "", GL_QUADS, 1|mode_Lighting);
	// BACK
	vertWind.clear();
	norm=QVector3D::crossProduct(vert[4]-vert[1],vert[2]-vert[1]);
	vertWind.push_back(Vertex(vert[1],color,norm,QVector3D(0,0,0)));
	vertWind.push_back(Vertex(vert[4],color,norm,QVector3D(1,0,0)));
	vertWind.push_back(Vertex(vert[7],color,norm,QVector3D(1,1,0)));
	vertWind.push_back(Vertex(vert[2],color,norm,QVector3D(0,1,0)));

	rendManager.addStaticGeometry("3d_building", vertWind, windowTex[windowTexId], GL_QUADS, 2|mode_Lighting);
}

void addColumnGeometry(VBORenderManager& rendManager, std::vector<QVector3D>& columnContour, int randomFacade, int windowTexId, float uS, float vS, float height, int numFloors) {
	std::vector<Vertex> vert;

	float verticalHoleSize = 0.5;
	float horHoleSize = 0.5;
	float accPerimeter = 0;
	QVector3D norm;
	for (int sN = 0; sN < columnContour.size(); sN++) {
		int ind1 = sN;
		int ind2 = (sN+1) % columnContour.size();
		std::vector<QVector3D> em;
		float sideLenght=(columnContour[ind1]-columnContour[ind2]).length();
		if (sideLenght <= 3.0f) {
			float heightB=height;
			float heightT=numFloors*storyHeight+height;

			QVector3D norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
			vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightB),QColor(),norm,QVector3D(accPerimeter*uS,heightB*vS,0.0f)));
			vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightB),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightB*vS,0.0f)));
			vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightT),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightT*vS,0.0f)));
			vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightT),QColor(),norm,QVector3D((accPerimeter)*uS,heightT*vS,0.0f)));
		} else {
			for (int numF = 0; numF < numFloors; numF++) {
				float h0 = numF*storyHeight+height;
				float h3 = (numF+1)*storyHeight+height;
				float h1 = h0+verticalHoleSize;
				float h2 = h3-verticalHoleSize;
				norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h0),QColor(),norm,QVector3D(accPerimeter*uS,h0*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h0),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h0*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter)*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QColor(),norm,QVector3D(accPerimeter*uS,h2*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h3),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h3*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h3),QColor(),norm,QVector3D((accPerimeter)*uS,h3*vS,0.0f)));
				// LEFT
				QVector3D dirW = columnContour[ind2] - columnContour[ind1];
				dirW /= sideLenght;
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QColor(),norm,QVector3D(accPerimeter*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2)+dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h2*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter)*uS,h2*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1)-dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2)-dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h2*vS,0.0f)));

				////////// INSIDE
				addWindow(rendManager, windowTexId, columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize, dirW, QVector3D(0,0,1.0f), sideLenght-2*horHoleSize, h2-h1);
			}
		}
		accPerimeter+=sideLenght;
	}
	rendManager.addStaticGeometry("3d_building",vert,facadeTex[randomFacade],GL_QUADS,2|mode_Lighting);
}

void VBOGeoBuilding::generateBuilding(VBORenderManager& rendManager, Building& building) {
	if (building.bldType == 0) {
		generateHouse(rendManager, building);
		return;
	}

	// テクスチャ読み込み
	if (!bldgInitialized) initBuildingsTex();

	float boxSize = 1.0f;
	float firstFloorHeight = 4.8f;
	float buildingHeight = (building.numStories - 1) * storyHeight + firstFloorHeight + boxSize;//just one box size (1st-2nd)

	// roofOffContを作成 (footprintを少し大きくする)
	Loop3D roofContour;
	building.footprint.computeInset(-boxSize, roofContour, false); 

	// １階部分を構築
	rendManager.addPrism("3d_building", building.footprint.contour, 0, firstFloorHeight, building.color, false);
	rendManager.addPrism("3d_building", roofContour, firstFloorHeight, firstFloorHeight + boxSize, building.color, true);

	// ファサードのcontourを計算する
	std::vector<QVector3D> columnContour;
	calculateColumnContour(building.footprint.contour, columnContour);

	// ファサードを追加する
	int randomFacade = qrand()%facadeTex.size();
	float uS = facadeScale[randomFacade].x();
	float vS = facadeScale[randomFacade].y();
	int windowTexId = ((int)qrand()) % windowTex.size();
	addColumnGeometry(rendManager, columnContour, randomFacade, windowTexId, uS, vS, firstFloorHeight + boxSize, building.numStories-1);

	// 屋根を追加する
	rendManager.addPrism("3d_building", roofContour, buildingHeight, buildingHeight + boxSize, building.color, false);
	rendManager.addPolygon("3d_building", roofContour, buildingHeight, building.color, true);
	rendManager.addPolygon("3d_building", roofContour, buildingHeight + boxSize, roofTex[building.roofTextureId], QVector3D(1, 1, 1));
}

/**
 * 一軒家の3Dモデルを生成する。
 */
void VBOGeoBuilding::generateHouse(VBORenderManager& rendManager, Building& building) {
	float floorHeight = 2.5f;
	float margin = 0.2f;

	QVector3D vec1 = (building.footprint.contour[1] - building.footprint.contour[0]).normalized();
	QVector3D vec2 = (building.footprint.contour[2] - building.footprint.contour[1]).normalized();

	// １階の扉の上まで
	{
		Loop3D polygon1;
		polygon1.push_back(building.footprint.contour[0]);
		polygon1.push_back(building.footprint.contour[1]);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 0.5f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 0.5f - vec1 * margin);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 6.0f - vec1 * margin);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 6.0f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.6f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.8f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.8f);
		polygon1.push_back(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.6f);
		polygon1.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon1.push_back(building.footprint.contour[3]);
	
		rendManager.addPrism("3d_building", polygon1, 0, floorHeight, houseTex[0]);
	}

	// １階扉とガレージ
	{
		QVector3D normal = (building.footprint.contour[1] - building.footprint.contour[0]).normalized();
		std::vector<Vertex> pts(4);
		pts[0] = Vertex(building.footprint.contour[1] + vec2 * 0.5f - vec1 * 0.1f, QColor(), normal, QVector3D(0, 0, 0));
		pts[1] = Vertex(building.footprint.contour[1] + vec2 * 6.0f - vec1 * 0.1f, QColor(), normal, QVector3D(5.5, 0, 0));
		pts[2] = Vertex(building.footprint.contour[1] + vec2 * 6.0f - vec1 * 0.1f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(5.5, floorHeight, 0));
		pts[3] = Vertex(building.footprint.contour[1] + vec2 * 0.5f - vec1 * 0.1f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(0, floorHeight, 0));
		rendManager.addStaticGeometry("3d_building", pts, houseTex[2], GL_QUADS, 2|mode_Lighting);

		pts[0] = Vertex(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.7f, QColor(), normal, QVector3D(0, 0, 0));
		pts[1] = Vertex(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.7f, QColor(), normal, QVector3D(1, 0, 0));
		pts[2] = Vertex(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.7f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(1, 1, 0));
		pts[3] = Vertex(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.7f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(0, 1, 0));
		rendManager.addStaticGeometry("3d_building", pts, houseTex[1], GL_QUADS, 2|mode_Lighting);
	}

	// １階出窓
	{
		QVector3D pt1 = (building.footprint[1] + vec2 * 6.5 + building.footprint[2]) * 0.5 - vec1 * 1.6 - vec2 * 1.5;
		pt1.setZ(1);
		QVector3D pt2 = pt1 + vec2 * 3.0;
		QVector3D pt3 = pt2 + QVector3D(0, 0, 1.5);
		QVector3D pt4 = pt1 + QVector3D(0, 0, 1.5);
		QVector3D pt5 = pt1 + vec1 * 0.4;
		QVector3D pt6 = pt2 + vec1 * 0.4;
		QVector3D pt7 = pt3 + vec1 * 0.4;
		QVector3D pt8 = pt4 + vec1 * 0.4;
		QVector3D pt9 = pt5 + vec2 * 0.1 + QVector3D(0, 0, 0.1);
		QVector3D pt10 = pt6 - vec2 * 0.1 + QVector3D(0, 0, 0.1);
		QVector3D pt11 = pt7 - vec2 * 0.1 - QVector3D(0, 0, 0.1);
		QVector3D pt12 = pt8 + vec2 * 0.1 - QVector3D(0, 0, 0.1);
		QVector3D pt13 = pt1 + vec2 * 0.1 + QVector3D(0, 0, 0.1) + vec1 * 0.1;
		QVector3D pt14 = pt2 - vec2 * 0.1 + QVector3D(0, 0, 0.1) + vec1 * 0.1;
		QVector3D pt15 = pt3 - vec2 * 0.1 - QVector3D(0, 0, 0.1) + vec1 * 0.1;
		QVector3D pt16 = pt4 + vec2 * 0.1 - QVector3D(0, 0, 0.1) + vec1 * 0.1;
		QVector3D pt17 = pt9 - vec2 * 0.1;
		QVector3D pt18 = pt10 + vec2 * 0.1;
		QVector3D pt19 = pt11 + vec2 * 0.1;
		QVector3D pt20 = pt12 - vec2 * 0.1;

		std::vector<Vertex> pts(4);
		pts[0] = Vertex(pt1, building.color, -vec2, QVector3D());
		pts[1] = Vertex(pt5, building.color, -vec2, QVector3D());
		pts[2] = Vertex(pt8, building.color, -vec2, QVector3D());
		pts[3] = Vertex(pt4, building.color, -vec2, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt6, building.color, vec2, QVector3D());
		pts[1] = Vertex(pt2, building.color, vec2, QVector3D());
		pts[2] = Vertex(pt3, building.color, vec2, QVector3D());
		pts[3] = Vertex(pt7, building.color, vec2, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt4, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[1] = Vertex(pt8, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[2] = Vertex(pt7, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[3] = Vertex(pt3, building.color, QVector3D(0, 0, 1), QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt5, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt6, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt18, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt17, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt20, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt19, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt7, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt8, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt17, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt9, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt12, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt20, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt10, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt18, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt19, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt11, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, houseTex[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt13, building.color, vec1, QVector3D(0, 0, 0));
		pts[1] = Vertex(pt14, building.color, vec1, QVector3D(1, 0, 0));
		pts[2] = Vertex(pt15, building.color, vec1, QVector3D(1, 1, 0));
		pts[3] = Vertex(pt16, building.color, vec1, QVector3D(0, 1, 0));
		rendManager.addStaticGeometry("3d_building", pts, houseTex[5], GL_QUADS, 2|mode_Lighting);
	}

	// １階の扉の上から２階の窓の下まで
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[0]);
		polygon.push_back(building.footprint.contour[1]);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
	
		rendManager.addPrism("3d_building", polygon, floorHeight, 3.5f, houseTex[0]);
	}

	// 日よけ
	{
		Loop3D polygon;
		polygon.resize(4);
		polygon[0] = building.footprint.contour[2];
		polygon[0].setZ(2.8);
		polygon[1] = building.footprint.contour[2] - vec1 * 1.6f;
		polygon[1].setZ(3.2);
		polygon[2] = building.footprint.contour[1] - vec1 * 1.6f + vec2 * 6.5f;
		polygon[2].setZ(3.2);
		polygon[3] = building.footprint.contour[1] + vec2 * 6.5f;
		polygon[3].setZ(2.8);
		rendManager.addQuad("3d_building", polygon, houseTex[3]);
	}

	// ２階の窓を含む壁
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[0]);

		{
			float length = (building.footprint.contour[1] - building.footprint.contour[0]).length();

			for (int i = 0; i < (int)((length - 2) / 3); ++i) {
				polygon.push_back(building.footprint.contour[0] + vec1 * (3.0f + i * 3.0));
				polygon.push_back(building.footprint.contour[0] + vec1 * (3.0f + i * 3.0) + vec2 * margin);
				polygon.push_back(building.footprint.contour[0] + vec1 * (3.0f + i * 3.0 + 1.5) + vec2 * margin);
				polygon.push_back(building.footprint.contour[0] + vec1 * (3.0f + i * 3.0 + 1.5));

				{ // 窓
					Loop3D pts;
					pts.resize(4);
					pts[0] = building.footprint.contour[0] + vec1 * (3.0f + i * 3.0) + vec2 * 0.1;
					pts[0].setZ(3.5);
					pts[1] = building.footprint.contour[0] + vec1 * (3.0f + i * 3.0 + 1.5) + vec2 * 0.1;
					pts[1].setZ(3.5);
					pts[2] = pts[1] + QVector3D(0, 0, 1.5);
					pts[3] = pts[0] + QVector3D(0, 0, 1.5);

					std::vector<Vertex> verts;
					verts.push_back(Vertex(pts[0], QColor(), vec1, QVector3D(0, 0, 0)));
					verts.push_back(Vertex(pts[1], QColor(), vec1, QVector3D(1, 0, 0)));
					verts.push_back(Vertex(pts[2], QColor(), vec1, QVector3D(1, 1, 0)));
					verts.push_back(Vertex(pts[3], QColor(), vec1, QVector3D(0, 1, 0)));

					rendManager.addStaticGeometry("3d_building", verts, houseTex[4], GL_QUADS, 2|mode_Lighting);
				}
			}
		}

		polygon.push_back(building.footprint.contour[1]);
		polygon.push_back(building.footprint.contour[1] + vec2 * 2.5f);
		polygon.push_back(building.footprint.contour[1] + vec2 * 2.5f - vec1 * margin);
		polygon.push_back(building.footprint.contour[1] + vec2 * 4.0f - vec1 * margin);
		polygon.push_back(building.footprint.contour[1] + vec2 * 4.0f);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f);
	
		{ // 窓
			Loop3D pts;
			pts.resize(4);
			pts[0] = building.footprint.contour[1] + vec2 * 2.5f - vec1 * 0.1f;
			pts[0].setZ(3.5);
			pts[1] = building.footprint.contour[1] + vec2 * 4.0f - vec1 * 0.1f;
			pts[1].setZ(3.5);
			pts[2] = pts[1] + QVector3D(0, 0, 1.5);
			pts[3] = pts[0] + QVector3D(0, 0, 1.5);

			std::vector<Vertex> verts;
			verts.push_back(Vertex(pts[0], QColor(), vec1, QVector3D(0, 0, 0)));
			verts.push_back(Vertex(pts[1], QColor(), vec1, QVector3D(1, 0, 0)));
			verts.push_back(Vertex(pts[2], QColor(), vec1, QVector3D(1, 1, 0)));
			verts.push_back(Vertex(pts[3], QColor(), vec1, QVector3D(0, 1, 0)));

			rendManager.addStaticGeometry("3d_building", verts, houseTex[4], GL_QUADS, 2|mode_Lighting);
		}

		{
			float length = (building.footprint.contour[2] - building.footprint.contour[1]).length() - 6.5;

			for (int i = 0; i < (int)((length - 3) / 3); ++i) {
				polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0));
				polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0) - vec1 * margin);
				polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0 + 1.5) - vec1 * margin);
				polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0 + 1.5));

				{ // 窓
					Loop3D pts;
					pts.resize(4);
					pts[0] = building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0) - vec1 * 0.1;
					pts[0].setZ(3.5);
					pts[1] = building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f + vec2 * (3.0f + i * 3.0 + 1.5) - vec1 * 0.1;
					pts[1].setZ(3.5);
					pts[2] = pts[1] + QVector3D(0, 0, 1.5);
					pts[3] = pts[0] + QVector3D(0, 0, 1.5);

					std::vector<Vertex> verts;
					verts.push_back(Vertex(pts[0], QColor(), vec1, QVector3D(0, 0, 0)));
					verts.push_back(Vertex(pts[1], QColor(), vec1, QVector3D(1, 0, 0)));
					verts.push_back(Vertex(pts[2], QColor(), vec1, QVector3D(1, 1, 0)));
					verts.push_back(Vertex(pts[3], QColor(), vec1, QVector3D(0, 1, 0)));

					rendManager.addStaticGeometry("3d_building", verts, houseTex[4], GL_QUADS, 2|mode_Lighting);
				}
			}
		}

		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
	
		rendManager.addPrism("3d_building", polygon, 3.5f, 5.0f, houseTex[0]);
	}

	// ２階の窓の上から
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[0]);
		polygon.push_back(building.footprint.contour[1]);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
	
		rendManager.addPrism("3d_building", polygon, 5.0f, 6.0f, houseTex[0]);
	}

	// ２階の三角部
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[1]);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		rendManager.addTriangle("3d_building", polygon, 6.0f, 8.0f, houseTex[0]);
	}

	// ２階の三角部
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
		rendManager.addTriangle("3d_building", polygon, 6.0f, 8.0f, houseTex[0]);
	}

	// 屋根
	{
		Loop3D polygon;
		QVector3D pt1 = QVector3D(building.footprint.contour[0].x(), building.footprint.contour[0].y(), 6.0);
		QVector3D pt2 = QVector3D(building.footprint.contour[1].x(), building.footprint.contour[1].y(), 6.0);
		QVector3D pt3 = (building.footprint.contour[1] + building.footprint.contour[1] + vec2 * 6.5f) * 0.5f;
		pt3.setZ(8.0);
		QVector3D pt4 = (building.footprint.contour[0] + building.footprint.contour[1] - vec1 * 1.6f) * 0.5f;
		pt4 += QVector3D(0, 6.5 * 0.5, 0);
		pt4.setZ(8.0);
		QVector3D pt5 = pt2 +  vec2 * 6.5;
		QVector3D pt6 = pt5 - vec1 * 1.6f;
		QVector3D pt7 = building.footprint.contour[2] - vec1 * 1.6f;
		pt7.setZ(6.0);
		QVector3D pt8 = QVector3D(building.footprint.contour[3].x(), building.footprint.contour[3].y(), 6.0);
		QVector3D pt9 = (pt7 + pt8) * 0.5f;
		pt9.setZ(8.0);
		QVector3D pt10 = (building.footprint.contour[0] + building.footprint.contour[1] - vec1 * 1.6f) * 0.5f;
		pt10.setZ(6.0);
		QVector3D pt11 = pt1 + vec2 * 3.25;
		QVector3D pt12 = pt6 - vec2 * 3.25;
		QVector3D pt13 = pt10 + vec2 * 6.5;

		polygon.push_back(pt10);
		polygon.push_back(pt2);
		polygon.push_back(pt3);
		polygon.push_back(pt4);
		rendManager.addQuad("3d_building", polygon, houseTex[3]);

		polygon.clear();
		polygon.push_back(pt8);
		polygon.push_back(pt11);
		polygon.push_back(pt4);
		polygon.push_back(pt9);
		rendManager.addQuad("3d_building", polygon, houseTex[3]);

		polygon.clear();
		polygon.push_back(pt12);
		polygon.push_back(pt7);
		polygon.push_back(pt9);
		polygon.push_back(pt4);
		rendManager.addQuad("3d_building", polygon, houseTex[3]);

		polygon.clear();
		polygon.push_back(pt5);
		polygon.push_back(pt13);
		polygon.push_back(pt4);
		polygon.push_back(pt3);
		rendManager.addQuad("3d_building", polygon, houseTex[3]);

		polygon.clear();
		Loop3D texCoord;
		polygon.push_back(pt1);
		texCoord.push_back(QVector3D(0, 0, 0));
		polygon.push_back(pt10);
		float base = (pt10 - pt1).length();
		float height = (pt4 - pt10).length();
		texCoord.push_back(QVector3D(base, 0, 0));
		polygon.push_back(pt4);
		texCoord.push_back(QVector3D(base, height, 0));
		rendManager.addTriangle("3d_building", polygon, texCoord, houseTex[3]);

		polygon.clear();
		texCoord.clear();
		polygon.push_back(pt11);
		texCoord.push_back(QVector3D(0, 0, 0));
		polygon.push_back(pt1);
		base = (pt1 - pt11).length();
		height = (pt4 - pt11).length();
		texCoord.push_back(QVector3D(base, 0, 0));
		polygon.push_back(pt4);
		texCoord.push_back(QVector3D(0, height, 0));
		rendManager.addTriangle("3d_building", polygon, texCoord, houseTex[3]);
	}
}
