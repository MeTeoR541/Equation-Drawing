#include "Viewer.h"
Viewer::Viewer(QWidget* parent) :QWidget(parent){
	this->setMinimumWidth(1000);
	this->setMinimumHeight(800);
	this->setMaximumWidth(1000);
	this->setMaximumHeight(800);
	center_x = 0;
	center_y = 0;
	axis_x = 0;
	axis_y = 0;
	range = 1;
	axis_hasX = true;
	axis_hasY = true;
	now_function_amount = 0;
	now_drawText = false;
}
void Viewer::drawInputText() {
	QLineEdit* temp = new QLineEdit(this);
	temp->setGeometry(700, 10, 150, 25);
	temp->show();
	text.push_back(temp);
}
void Viewer::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	QPen pen;
	QBrush brush(Qt::white);
	painter.setBrush(brush);
	pen.setColor(QColor(Qt::black));
	painter.setPen(pen);
	painter.drawRect(10, 10, 600, 600);
	
	if (!now_drawText) {
		drawInputText();
		now_drawText = true;
	}
		
	//test
	//painter.drawText(700, 10, QString::number(mouse_x, 10, 4));
	//painter.drawText(700, 20, QString::number(mouse_y, 10, 4));
	drawCoordinate(painter);
	drawFunction(painter);
}
void Viewer::wheelEvent(QWheelEvent* event) {
	QPoint delta = event->angleDelta() / 8;
	QPointF temp = event->position();
	if (delta.y() > 0) {
		changeCenter(temp.x(), temp.y(), range / 2);
		update();
	}
	else {
		changeCenter(temp.x(), temp.y(), range * 2);
		update();
	}
}
void Viewer::changeCenter(double now_x, double now_y, double newrange) {
	if (now_x > 10 && now_x < 610 && now_y>10 && now_y < 610) {
		center_x = ((now_x - 310) / 50 * range + center_x) - ((now_x - 310) / 50 * newrange);
		center_y = ((310 - now_y) / 50 * range + center_y) - ((310 - now_y) / 50 * newrange);
		range = newrange;
	}
}
void Viewer::mouseMoveEvent(QMouseEvent* event) {
	center_x = center_x - (event->x() - mouse_x) / 50 * range;
	center_y = center_y - (mouse_y - event->y()) / 50 * range;
	mouse_x = event->x();
	mouse_y = event->y();
	update();
}
void Viewer::mousePressEvent(QMouseEvent* event) {
	mouse_x = event->x();
	mouse_y = event->y();
}
void Viewer::drawCoordinate(QPainter& painter) {
	double test = center_x;
	QPen pen;
	pen.setColor(QColor(0, 0, 0, 100));
	painter.setPen(pen);
	axis_hasX = false;
	axis_hasY = false;
	if (center_x > 0) {
		while (test >= range) {
			test = test - range;
		}
		for (int i = 0; i < 7; i++) {
			if ((310 - (test / range) * 50) - 50 * i > 10) {
				if (center_x - test - (double)i * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_y = 310 - (test / range) * 50 - 50 * i;
					axis_hasY = true;
				}
				painter.drawLine(310 - (test / range) * 50 - 50 * i, 10, 310 - (test / range) * 50 - 50 * i, 610);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
				axis_headX = 310 - (test / range) * 50 - 50 * i;
				range_headX = center_x - test - (double)i * range;
			}
			if ((310 - (test / range) * 50) + 50 * (i + 1) < 610) {
				if (center_x - test + (double)(i + 1) * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_y = 310 - (test / range) * 50 + 50 * (i + 1);
					axis_hasY = true;
				}
				painter.drawLine(310 - (test / range) * 50 + 50 * (i + 1), 10, 310 - (test / range) * 50 + 50 * (i + 1), 610);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
			}
		}
	}
	else {
		while (test < 0) {
			test = test + range;
		}
		for (int i = 0; i < 7; i++) {
			if ((310 - (test / range) * 50) - 50 * i > 10) {
				if (center_x - test - (double)i * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_y = 310 - (test / range) * 50 - 50 * i;
					axis_hasY = true;
				}
				painter.drawLine(310 - (test / range) * 50 - 50 * i, 10, 310 - (test / range) * 50 - 50 * i, 610);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
				axis_headX = 310 - (test / range) * 50 - 50 * i;
				range_headX = center_x - test - (double)i * range;
			}
			if ((310 - (test / range) * 50) + 50 * (i + 1) < 610) {
				if (center_x - test + (double)(i + 1) * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_y = 310 - (test / range) * 50 + 50 * (i + 1);
					axis_hasY = true;
				}
				painter.drawLine(310 - (test / range) * 50 + 50 * (i + 1), 10, 310 - (test / range) * 50 + 50 * (i + 1), 610);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
			}
		}
	}
	test = center_y;
	if (center_y > 0) {
		while (test >= range) {
			test = test - range;
		}
		for (int i = 0; i < 7; i++) {
			if ((310 + (test / range) * 50) - 50 * i > 10) {
				if (center_y - test + (double)i * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_x = 310 + (test / range) * 50 - 50 * i;
					axis_hasX = true;
				}
				painter.drawLine(10, 310 + (test / range) * 50 - 50 * i, 610, 310 + (test / range) * 50 - 50 * i);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
			}
			if ((310 + (test / range) * 50) + 50 * (i + 1) < 610) {
				if (center_y - test - (double)(i + 1) * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_x = 310 + (test / range) * 50 + 50 * (i + 1);
					axis_hasX = true;
				}
				painter.drawLine(10, 310 + (test / range) * 50 + 50 * (i + 1), 610, 310 + (test / range) * 50 + 50 * (i + 1));
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
				axis_headY = 310 + (test / range) * 50 + 50 * (i + 1);
				range_headY = center_y - test - (double)(i + 1) * range;
			}
		}
	}
	else {
		while (test < 0) {
			test = test + range;
		}
		for (int i = 0; i < 7; i++) {
			if ((310 + (test / range) * 50) - 50 * i > 10) {
				if (center_y - test + (double)i * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					axis_x = 310 + (test / range) * 50 - 50 * i;
					axis_hasX = true;
				}
				painter.drawLine(10, 310 + (test / range) * 50 - 50 * i, 610, 310 + (test / range) * 50 - 50 * i);
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
			}
			if ((310 + (test / range) * 50) + 50 * (i + 1) < 610) {
				if (center_y - test - (double)(i + 1) * range == 0) {
					pen.setColor(QColor(Qt::black));
					painter.setPen(pen);
					double axis_x = 310 + (test / range) * 50 + 50 * (i + 1);
					axis_hasX = true;
				}
				painter.drawLine(10, 310 + (test / range) * 50 + 50 * (i + 1), 610, 310 + (test / range) * 50 + 50 * (i + 1));
				pen.setColor(QColor(0, 0, 0, 100));
				painter.setPen(pen);
				axis_headY = 310 + (test / range) * 50 + 50 * (i + 1);
				range_headY = center_y - test - (double)(i + 1) * range;
			}
		}
	}
	drawAxis(painter);
}
void Viewer::drawAxis(QPainter& painter) {
	if (axis_hasX) {
		for (int i = 0; i < 12; i++) {
			if (axis_headX + i * 50 > 10 && axis_headX + i * 50 < 610)
				painter.drawText(axis_headX + i * 50, axis_x, QString::number(range_headX + i * range, 10, 4));
		}
	}
	if (axis_hasY) {
		for (int i = 0; i < 12; i++) {
			if (axis_headY - i * 50 > 10 && axis_headY - i * 50 < 610)
				painter.drawText(axis_y, axis_headY - i * 50, QString::number(range_headY + i * range, 10, 4));
		}
	}
}
void Viewer::drawFunction(QPainter& painter) {
	QPen pen;
	pen.setColor(QColor(0, 255, 0));
	painter.setPen(pen);
	//painter.drawRect(305, 305, 10, 10);
	double previous_x, previous_y;
	bool first = true;
	for (int i = 0; i < 600; i++) {
		double x, y;
		y = sin((double)i / (50 / range) + center_x - 300 / (50 / range));
		if (!first && ((center_y - y) * (50 / range) + 310) < 610 && ((center_y - y) * (50 / range) + 310) > 10)
			painter.drawLine(i + 10, (center_y - y) * (50 / range) + 310, previous_x + 10, (center_y - previous_y) * (50 / range) + 310);
		first = false;
		if (i == 200) {
			int test;
			test = 0;
		}

		previous_x = i;
		previous_y = y;
	}
}