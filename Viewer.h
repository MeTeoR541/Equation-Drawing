#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <QDialog>
#include <map>
#include <vector>
#include <QWidget>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <cmath>
#include "functions.h"
#include "Variable.h"
using namespace std;
class Viewer :public QWidget {
	Q_OBJECT
private:
	vector<QLineEdit*>text;
	vector<string>function;
	vector<int>function_color;
	vector<int>function_color_sec;
	vector<bool>function_hide; //false顯示；true不顯示
	vector<bool>error;
	vector<Variable>variable; //x,y以外的變數
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

	Function fun;
	QPixmap display;
	QPixmap hide;
	QPixmap cancel;
public:
	Viewer(QWidget* parent = 0);
	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void keyPressEvent(QKeyEvent* event);

	void drawInputText();
	void drawCoordinate(QPainter& painter);
	void drawAxis(QPainter& painter);
	void changeCenter(double now_x, double now_y, double newrange);
	void drawFunction(QPainter& painter);
	bool variableCheck(string value);
};