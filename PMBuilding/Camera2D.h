#pragma once

#include "glew.h"
#include "qvector4d.h"
#include "qmatrix4x4.h"
#include "Camera.h"
#include <QFile>

class Camera2D : public Camera {
public:
	float xrot;
	float yrot;
	float zrot;

	float dx;
	float dy;
	float dz;

	float lookAtX;
	float lookAtY;
	float lookAtZ;

	Camera2D() {
		xrot = 0.0f;//-75.0;
		yrot = 0.0;
		zrot = 0.0f;//-45.0;
		dx = 0.0;
		dy = 0.0;
		dz = 100.0;
		lookAtX = 0.0f;
		lookAtY = 0.0f;
		lookAtZ = 0.0f;
		fovy = 60.0f;

		type = TYPE_2D;
	}

	QVector4D getCamPos() {
		QVector4D eye(0.0f, 0.0f, 0.0f, 1.0f);
		return ((mvMatrix.transposed()).inverted())*eye;
	}

	void updatePerspective(int width,int height){

		float aspect=(float)width/(float)height;
		float zfar=30000.0f;//90000.0f;
		float znear=5.0f;

		float f = 1.0f / tan (fovy * (0.00872664625f));//PI/360

		double m[16]=
		{	 f/aspect,	0,								0,									0,
					0,	f,								0,						 			0,
			        0,	0,		(zfar+znear)/(znear-zfar),		(2.0f*zfar*znear)/(znear-zfar),
			        0,	0,		    				   -1,									0

		};
		pMatrix=QMatrix4x4(m);
	}

	void updateCamMatrix() {
		// modelview matrix
		mvMatrix.setToIdentity();
		mvMatrix.translate(-dx, -dy, -dz);
		mvMatrix.rotate(xrot, 1.0, 0.0, 0.0);		
		mvMatrix.rotate(yrot, 0.0, 1.0, 0.0);
		mvMatrix.rotate(zrot, 0.0, 0.0, 1.0);
		mvMatrix.translate(-lookAtX, -lookAtY, -lookAtZ);
		// normal matrix
		normalMatrix=mvMatrix.normalMatrix();
		// mvp
		mvpMatrix=pMatrix*mvMatrix;
	}

	static void qNormalizeAngle(float &angle) {
		while (angle < 0)
			angle += 360.0;
		while (angle >= 360.0)
			angle -= 360.0;
	}

	void setRotation(float x, float y, float z) {
		setXRotation(x);
		setYRotation(y);
		setZRotation(z);		
	}

	void setXRotation(float angle) {
		qNormalizeAngle(angle);
		xrot = angle;			
	}

	void setYRotation(float angle) {
		qNormalizeAngle(angle);
		yrot = angle;			
	}

	void setZRotation(float angle) {
		qNormalizeAngle(angle);
		zrot = angle;			
	}

	void changeXRotation(float angle) {
		setXRotation(xrot+angle);
	}

	void changeYRotation(float angle) {
		setYRotation(yrot+angle);
	}

	void changeZRotation(float angle) {
		setZRotation(zrot+angle);
	}

	void setTranslation(float x, float y, float z) {
		dx = x;
		dy = y;
		dz = z;
	}

	void changeXYZTranslation(float x, float y, float z) {
		dx += x;
		dy += y;
		dz += z;
	}

	void setLookAt(float x, float y, float z) {
		lookAtX = x;
		lookAtY = y;
		lookAtZ = z;
	}

	void resetCamera() {
		setLookAt(0.0f, 0.0f, 0.0f);
		xrot = 0.0f;
		yrot = 0.0;
		zrot = 0.0f;
		setTranslation(0.0f, 0.0f, 100);
	}//

	void saveCameraPose(const QString &filepath) {
		//QFile camFile("data/camPose"+QString::number(numCam)+".cam");
		QFile camFile(filepath);

		if (!camFile.open(QIODevice::WriteOnly | QIODevice::Text)){
			printf("ERROR: Cannot open the file cam.txt for writing\n");
			return;
		}
		QTextStream stream( &camFile );	

		stream << xrot << " " << yrot << " " << zrot << " " <<
			this->dx << " " << dy << " " << " " << dz << " " << lookAtX << " " << lookAtY << " " << lookAtZ;		

		camFile.close();
	}//

	void loadCameraPose(const QString &filepath) {
		//QFile camFile("data/camPose"+QString::number(numCam)+".cam");
		QFile camFile(filepath);

		if (!camFile.open(QIODevice::ReadOnly | QIODevice::Text)) // Open the file
		{
			printf("Can't open file.\n");
		}

		else {
			QTextStream stream( &camFile); // Set the stream to read from myFile
			stream >> xrot;
			stream >> yrot;
			stream >> zrot;
			stream >> dx;
			stream >> dy;
			stream >> dz;
			stream >> lookAtX;
			stream >> lookAtY;
			stream >> lookAtZ;
		}
	}//
};

