﻿#include "PMBuildingHouse.h"

std::vector<QString> PMBuildingHouse::textures;
bool PMBuildingHouse::initialized = false;

void PMBuildingHouse::initialize() {
	if (initialized) return;

	textures.push_back("../data/textures/LC/house/wall.jpg");
	textures.push_back("../data/textures/LC/house/door.jpg");
	textures.push_back("../data/textures/LC/house/garage.jpg");
	textures.push_back("../data/textures/LC/house/roof.jpg");
	textures.push_back("../data/textures/LC/house/window.jpg");
	textures.push_back("../data/textures/LC/house/window2.jpg");

	initialized = true;
}

/**
 * 一軒家の3Dモデルを生成する。
 */
void PMBuildingHouse::generate(VBORenderManager& rendManager, Building& building) {
	initialize();

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
	
		rendManager.addPrism("3d_building", polygon1, 0, floorHeight, textures[0]);
	}

	// １階扉とガレージ
	{
		QVector3D normal = (building.footprint.contour[1] - building.footprint.contour[0]).normalized();
		std::vector<Vertex> pts(4);
		pts[0] = Vertex(building.footprint.contour[1] + vec2 * 0.5f - vec1 * 0.1f, QColor(), normal, QVector3D(0, 0, 0));
		pts[1] = Vertex(building.footprint.contour[1] + vec2 * 6.0f - vec1 * 0.1f, QColor(), normal, QVector3D(5.5, 0, 0));
		pts[2] = Vertex(building.footprint.contour[1] + vec2 * 6.0f - vec1 * 0.1f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(5.5, floorHeight, 0));
		pts[3] = Vertex(building.footprint.contour[1] + vec2 * 0.5f - vec1 * 0.1f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(0, floorHeight, 0));
		rendManager.addStaticGeometry("3d_building", pts, textures[2], GL_QUADS, 2|mode_Lighting);

		pts[0] = Vertex(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.7f, QColor(), normal, QVector3D(0, 0, 0));
		pts[1] = Vertex(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.7f, QColor(), normal, QVector3D(1, 0, 0));
		pts[2] = Vertex(building.footprint.contour[1] + vec2 * 8.3f - vec1 * 1.7f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(1, 1, 0));
		pts[3] = Vertex(building.footprint.contour[1] + vec2 * 7.0f - vec1 * 1.7f + QVector3D(0, 0, floorHeight), QColor(), normal, QVector3D(0, 1, 0));
		rendManager.addStaticGeometry("3d_building", pts, textures[1], GL_QUADS, 2|mode_Lighting);
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
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt6, building.color, vec2, QVector3D());
		pts[1] = Vertex(pt2, building.color, vec2, QVector3D());
		pts[2] = Vertex(pt3, building.color, vec2, QVector3D());
		pts[3] = Vertex(pt7, building.color, vec2, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt4, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[1] = Vertex(pt8, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[2] = Vertex(pt7, building.color, QVector3D(0, 0, 1), QVector3D());
		pts[3] = Vertex(pt3, building.color, QVector3D(0, 0, 1), QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt5, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt6, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt18, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt17, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt20, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt19, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt7, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt8, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt17, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt9, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt12, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt20, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt10, building.color, vec1, QVector3D());
		pts[1] = Vertex(pt18, building.color, vec1, QVector3D());
		pts[2] = Vertex(pt19, building.color, vec1, QVector3D());
		pts[3] = Vertex(pt11, building.color, vec1, QVector3D());
		rendManager.addStaticGeometry("3d_building", pts, textures[0], GL_QUADS, 1|mode_Lighting);

		pts[0] = Vertex(pt13, building.color, vec1, QVector3D(0, 0, 0));
		pts[1] = Vertex(pt14, building.color, vec1, QVector3D(1, 0, 0));
		pts[2] = Vertex(pt15, building.color, vec1, QVector3D(1, 1, 0));
		pts[3] = Vertex(pt16, building.color, vec1, QVector3D(0, 1, 0));
		rendManager.addStaticGeometry("3d_building", pts, textures[5], GL_QUADS, 2|mode_Lighting);
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
	
		rendManager.addPrism("3d_building", polygon, floorHeight, 3.5f, textures[0]);
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
		rendManager.addQuad("3d_building", polygon, textures[3]);
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

					rendManager.addStaticGeometry("3d_building", verts, textures[4], GL_QUADS, 2|mode_Lighting);
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

			rendManager.addStaticGeometry("3d_building", verts, textures[4], GL_QUADS, 2|mode_Lighting);
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

					rendManager.addStaticGeometry("3d_building", verts, textures[4], GL_QUADS, 2|mode_Lighting);
				}
			}
		}

		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
	
		rendManager.addPrism("3d_building", polygon, 3.5f, 5.0f, textures[0]);
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
	
		rendManager.addPrism("3d_building", polygon, 5.0f, 6.0f, textures[0]);
	}

	// ２階の三角部
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[1]);
		polygon.push_back(building.footprint.contour[1] + vec2 * 6.5f);
		rendManager.addTriangle("3d_building", polygon, 6.0f, 8.0f, textures[0]);
	}

	// ２階の三角部
	{
		Loop3D polygon;
		polygon.push_back(building.footprint.contour[2] - vec1 * 1.6f);
		polygon.push_back(building.footprint.contour[3]);
		rendManager.addTriangle("3d_building", polygon, 6.0f, 8.0f, textures[0]);
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
		rendManager.addQuad("3d_building", polygon, textures[3]);

		polygon.clear();
		polygon.push_back(pt8);
		polygon.push_back(pt11);
		polygon.push_back(pt4);
		polygon.push_back(pt9);
		rendManager.addQuad("3d_building", polygon, textures[3]);

		polygon.clear();
		polygon.push_back(pt12);
		polygon.push_back(pt7);
		polygon.push_back(pt9);
		polygon.push_back(pt4);
		rendManager.addQuad("3d_building", polygon, textures[3]);

		polygon.clear();
		polygon.push_back(pt5);
		polygon.push_back(pt13);
		polygon.push_back(pt4);
		polygon.push_back(pt3);
		rendManager.addQuad("3d_building", polygon, textures[3]);

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
		rendManager.addTriangle("3d_building", polygon, texCoord, textures[3]);

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
		rendManager.addTriangle("3d_building", polygon, texCoord, textures[3]);
	}
}
