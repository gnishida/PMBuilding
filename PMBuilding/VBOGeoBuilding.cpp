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

void calculateColumnContour(std::vector<QVector3D>& currentContour,std::vector<QVector3D>& columnContour){
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

void addWindow(VBORenderManager& rendManager, int type,QVector3D randN,bool frameBoder, QVector3D initPoint,QVector3D dirR,QVector3D dirUp,float width,float height) {
	frameBoder=((int)randN.y())%2;
	QColor color;
	int randCol=((int)randN.z())%5;
	switch(randCol){
	case 0:
		color=QColor(0.3f,0.3f,0.3f);
		break;
	case 1:
		color=QColor(0.345, 0.171, 0.075);//brown
		break;
	case 2:
		color=QColor(0.412, 0.412, 0.412);//grey
		break;
	case 3:
		color=QColor(0.02, 0.02, 0.13);//blue
		break;
	case 4:
		color=QColor(0.961, 0.961, 0.863);//beige
		break;
	}
	// NO WINDOW, JUST DEPTH
	if(type==0){
		std::vector<Vertex> vertWind;

		float depth=2.0f;
		// IN: TOP
		QVector3D perI=QVector3D::crossProduct(dirUp,dirR);//note direction: to inside
		std::vector<QVector3D> tex;
		QVector3D vert[8];
		vert[0]=initPoint;
		vert[1]=initPoint+perI*depth;
		vert[2]=initPoint+perI*depth+dirUp*height;
		vert[3]=initPoint+dirUp*height;

		vert[4]=initPoint+perI*depth+dirR*width;
		vert[5]=initPoint+dirR*width;
		vert[6]=initPoint+dirUp*height+dirR*width;
		vert[7]=initPoint+perI*depth+dirUp*height+dirR*width;
		int texN=-1;
		QColor color(0.5f,0.5f,0.5f);
		// LEFT
		//addTexQuad(texN,vert[0],vert[1],vert[2],vert[3],tex,color,QVector3D(),true);
		QVector3D norm;
		norm=QVector3D::crossProduct(vert[1]-vert[0],vert[3]-vert[0]);
		vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));

		// RIGHT
		//addTexQuad(texN,vert[4],vert[5],vert[6],vert[7],tex,color,QVector3D(),true);
		norm=QVector3D::crossProduct(vert[5]-vert[4],vert[7]-vert[4]);
		vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
		//// BACK
		//addTexQuad(texN,vert[1],vert[4],vert[7],vert[2],tex,QVector3D(0.0f,0.0f,0.5f),QVector3D(),true);
		// TOP
		//addTexQuad(texN,vert[2],vert[7],vert[6],vert[3],tex,color,QVector3D(),true);
		norm=QVector3D::crossProduct(vert[7]-vert[2],vert[3]-vert[2]);
		vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));
		// BOT
		//addTexQuad(texN,vert[0],vert[5],vert[4],vert[1],tex,color,QVector3D(),true);
		norm=QVector3D::crossProduct(vert[5]-vert[0],vert[1]-vert[0]);
		vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
		vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
		rendManager.addStaticGeometry("3d_building",vertWind,"",GL_QUADS,1|mode_Lighting);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
		//////////////////////////////////////////////////////
		// BACK
		vertWind.clear();
		norm=QVector3D::crossProduct(vert[4]-vert[1],vert[2]-vert[1]);
		vertWind.push_back(Vertex(vert[1],color,norm,QVector3D(0,0,0)));
		vertWind.push_back(Vertex(vert[4],color,norm,QVector3D(1,0,0)));
		vertWind.push_back(Vertex(vert[7],color,norm,QVector3D(1,1,0)));
		vertWind.push_back(Vertex(vert[2],color,norm,QVector3D(0,1,0)));

		rendManager.addStaticGeometry("3d_building",vertWind,windowTex[((int)randN.x())%windowTex.size()],GL_QUADS,2|mode_Lighting);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
	}
}

void addColumnGeometry(VBORenderManager& rendManager,
	std::vector<QVector3D>& columnContour,int randomFacade,QVector3D randN,float uS,float vS,float height,int numFloors,bool buildingWithWindows,QVector3D windowRandomSize){

		std::vector<Vertex> vert;

		float verticalHoleSize=windowRandomSize.x();//0.1f+(1.2f*qrand())/RAND_MAX;
		float horHoleSize=windowRandomSize.y();
		float accPerimeter=0;
		QVector3D norm;
		for(int sN=0;sN<columnContour.size();sN++){
			int ind1=sN;
			int ind2=(sN+1)%columnContour.size();
			std::vector<QVector3D> em;
			bool window=(columnContour[ind1]-columnContour[ind2]).length()>3.0f;
			float sideLenght=(columnContour[ind1]-columnContour[ind2]).length();
			if(window==false){//just column
				float heightB=height;
				float heightT=numFloors*storyHeight+height;
				/*em.clear();//calculate coordenates (respect the perimeter)
				em.push_back();
				em.push_back(QVector3D();
				em.push_back();
				em.push_back();
				addTexQuad(facades[randomFacade],
				columnContour[ind1]+QVector3D(0,0,heightB),
				columnContour[ind2]+QVector3D(0,0,heightB),
				columnContour[ind2]+QVector3D(0,0,heightT),
				columnContour[ind1]+QVector3D(0,0,heightT),
				em,
				QVector3D(),
				QVector3D(),
				true);*/
				QVector3D norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightB),QColor(),norm,QVector3D(accPerimeter*uS,heightB*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightB),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightB*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightT),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightT*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightT),QColor(),norm,QVector3D((accPerimeter)*uS,heightT*vS,0.0f)));


			}else{

				for(int numF=0;numF<numFloors;numF++){
					float h0=numF*storyHeight+height;
					float h3=(numF+1)*storyHeight+height;
					float h1=h0+verticalHoleSize;
					float h2=h3-verticalHoleSize;
					// BOT
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h0*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h0*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h1*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h0),
					columnContour[ind2]+QVector3D(0,0,h0),
					columnContour[ind2]+QVector3D(0,0,h1),
					columnContour[ind1]+QVector3D(0,0,h1),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h0),QColor(),norm,QVector3D(accPerimeter*uS,h0*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h0),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h0*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter)*uS,h1*vS,0.0f)));
					// TOP
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h3*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h3*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h3),
					columnContour[ind1]+QVector3D(0,0,h3),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));//h3-h2
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QColor(),norm,QVector3D(accPerimeter*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h3),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h3*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h3),QColor(),norm,QVector3D((accPerimeter)*uS,h3*vS,0.0f)));
					// LEFT
					QVector3D dirW=(columnContour[ind2]-columnContour[ind1]);
					float windWidth=dirW.length();
					dirW/=windWidth;
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+horHoleSize)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+horHoleSize)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h2*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h1),
					columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,
					columnContour[ind1]+QVector3D(0,0,h2)+dirW*horHoleSize,
					columnContour[ind1]+QVector3D(0,0,h2),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(dirW*horHoleSize,QVector3D(0,0,1.0f)).normalized();//h2-h1
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QColor(),norm,QVector3D(accPerimeter*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2)+dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter)*uS,h2*vS,0.0f)));
					// RIGHT
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h2*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind2]+QVector3D(0,0,h1)-dirW*horHoleSize,
					columnContour[ind2]+QVector3D(0,0,h1),
					columnContour[ind2]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h2)-dirW*horHoleSize,
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(dirW*horHoleSize,QVector3D(0,0,1.0f)).normalized();//h2-h1
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1)-dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QColor(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2)-dirW*horHoleSize,QColor(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h2*vS,0.0f)));

					////////// INSIDE
					addWindow(rendManager,0,randN,false,columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,dirW,QVector3D(0,0,1.0f),windWidth-2*horHoleSize,h2-h1);
					//printf("wind wid %f %f\n",windWidth,h2-h1);
				}
			}
			accPerimeter+=sideLenght;
		}
		//rendManager.addStaticGeometry("buildingsTop",vert,"../data/textures/LC/facade/SeamlessBrick05_1.25_1.3_8.JPG",GL_QUADS,2|LC::mode_AdaptTerrain|LC::mode_Lighting);
		rendManager.addStaticGeometry("3d_building",vert,facadeTex[randomFacade],GL_QUADS,2|mode_Lighting);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
}//

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
		printf("initBuildingsTex f %d w %d r %d\n",facadeTex.size(),windowTex.size(),roofTex.size());
	bldgInitialized=true;
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
	int randomFacade=qrand()%facadeTex.size();
	float uS=facadeScale[randomFacade].x();
	float vS=facadeScale[randomFacade].y();
	QVector3D windowRandSize((float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX);
	QVector3D randN(qrand(),qrand(),qrand());
	addColumnGeometry(rendManager,columnContour,randomFacade,randN,uS,vS, firstFloorHeight + boxSize, building.numStories-1, false, windowRandSize);

	// 屋根を追加する
	rendManager.addPrism("3d_building", roofContour, buildingHeight, buildingHeight + boxSize, building.color, false);
	rendManager.addPolygon("3d_building", roofContour, buildingHeight, building.color, true);
	rendManager.addPolygon("3d_building", roofContour, buildingHeight + boxSize, roofTex[building.roofTextureId], QVector3D(1, 1, 1));
}

/**
 * 一軒家の3Dモデルを生成する。
 */
void VBOGeoBuilding::generateHouse(VBORenderManager& rendManager,Building& building) {
	Polygon3D& footprint=building.footprint;
	int numStories=building.numStories;

	///////////////////////////
	// SIDES
	float height = storyHeight;
	std::vector<Vertex> sideVert;
	int nextN;
	QVector3D normal;
	float rndColod=((0.7f*(float)qrand())/RAND_MAX)+0.3f;
	QColor color(rndColod,rndColod,rndColod);

	std::vector<QVector3D> pts(4);
	pts[0] = (footprint[1] - footprint[0]).normalized() * 2 + footprint[0];
	pts[1] = footprint[1];
	pts[2] = footprint[2];
	pts[3] = (footprint[2] - footprint[3]).normalized() * 2 + footprint[3];

	for(int curN=0;curN<pts.size();curN++){
		nextN = (curN+1) % pts.size();
		normal = QVector3D::crossProduct(pts[nextN]-pts[curN], QVector3D(0,0,1)).normalized();
		sideVert.push_back(Vertex(QVector3D(pts[curN].x(), pts[curN].y(), pts[curN].z()), color, normal, QVector3D()));
		sideVert.push_back(Vertex(QVector3D(pts[nextN].x(), pts[nextN].y(), pts[nextN].z()), color, normal, QVector3D()));
		sideVert.push_back(Vertex(QVector3D(pts[nextN].x(), pts[nextN].y(), pts[nextN].z()+height), color, normal, QVector3D()));
		sideVert.push_back(Vertex(QVector3D(pts[curN].x(), pts[curN].y(), pts[curN].z()+height), color, normal, QVector3D()));			
	}
	rendManager.addStaticGeometry("3d_building",sideVert,"",GL_QUADS,1|mode_Lighting);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

	// ROOF
	/*if(footprint.contour.size()==3||footprint.contour.size()==4){
		normal=QVector3D(0,0,1.0f);
		std::vector<Vertex> topVert;
		topVert.push_back(Vertex(QVector3D(footprint[0].x(),footprint[0].y(),footprint[0].z()+height),color,normal,QVector3D()));
		topVert.push_back(Vertex(QVector3D(footprint[1].x(),footprint[1].y(),footprint[1].z()+height),color,normal,QVector3D()));
		topVert.push_back(Vertex(QVector3D(footprint[2].x(),footprint[2].y(),footprint[2].z()+height),color,normal,QVector3D()));
		if(footprint.contour.size()==4){

			topVert.push_back(Vertex(QVector3D(footprint[3].x(),footprint[3].y(),footprint[3].z()+height),color,normal,QVector3D()));
		}else{
			topVert.push_back(Vertex(QVector3D(footprint[2].x(),footprint[2].y(),footprint[2].z()+height),color,normal,QVector3D()));
		}
		rendManager.addStaticGeometry("3d_building",topVert,"",GL_QUADS,1|mode_Lighting);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
	}*/
}
