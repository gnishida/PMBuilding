#pragma once

#include <QSettings>
#include "Polygon3D.h"
#include <QColor>

class Building {
public:
	Polygon3D footprint;
	int bldType;
	int numStories;
	QColor color;

public:
	Building() : bldType(-1), numStories(-1) {}
};
