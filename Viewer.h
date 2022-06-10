#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <QDialog>
#include <vector>
#include <QWidget>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <cmath>
using namespace std;
class Viewer :public QWidget {
	Q_OBJECT
private:
	vector<QLineEdit*>text;
	vector<int>function_color;
	vector<int>function_color_sec;
	int now_function_amount;
	bool now_drawText;
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

	double mouse_x;
	double mouse_y;
public:
	Viewer(QWidget* parent = 0);
	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);

	void drawInputText();
	void drawCoordinate(QPainter& painter);
	void drawAxis(QPainter& painter);
	void changeCenter(double now_x, double now_y, double newrange);
	void drawFunction(QPainter& painter);
};