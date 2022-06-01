#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <QDialog>
#include <vector>
#include <QWidget>
#include <QApplication>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QFileDialog>
#include <cmath>
using namespace std;
class Viewer :public QWidget {
	Q_OBJECT
private:
	double center_x;
	double center_y;
	double range;
	double range_headX;
	double range_headY;
	double axis_x;
	double axis_y;
	double axis_headX;
	double axis_headY;
	bool axis_hasX;
	bool axis_hasY;
public:
	Viewer(QWidget* parent = 0);
	void paintEvent(QPaintEvent*);
	void drawCoordinate(QPainter& painter);
	void drawAxis(QPainter& painter);
};