#include "Viewer.h"
Viewer::Viewer(QWidget* parent) :QWidget(parent){
	this->setMinimumWidth(1000);
	this->setMinimumHeight(800);
	this->setMaximumWidth(1000);
	this->setMaximumHeight(800);

	QLineEdit* temp = new QLineEdit(this);
	temp->setGeometry(700, 45, 175, 35);
	text.push_back(temp);

	int color = rand() % 256;
	function_color.push_back(color);
	color = rand() % 256;
	function_color_sec.push_back(color);
	function_hide.push_back(false);
	error.push_back(false);
	center_x = 0;
	center_y = 0;
	axis_x = 0;
	axis_y = 0;
	range = 1;
	axis_hasX = true;
	axis_hasY = true;
	now_function_amount = 1;
	now_drawText = false;

	display.load("Button1.png");
	hide.load("Button1-2.png");
	cancel.load("Button2.png");
	error_pix.load("error.png");
}
void Viewer::drawInputText() {
	for (int i = 0; i < now_function_amount; i++) {
		text[i]->setGeometry(700, i * 35 + 45, 175, 35);
		text[i]->show();
	}
	QPushButton* test = new QPushButton(this);
	test->setText("Add Function");
	test->show();
	test->setGeometry(700, 10, 175, 35);
	connect(test, &QPushButton::clicked, [&] {
		QLineEdit* temp = new QLineEdit(this);
		temp->setGeometry(700, now_function_amount * 35 + 10, 175, 35);
		text.push_back(temp);
		int color = rand() % 256;
		function_color.push_back(color);
		color = rand() % 256;
		function_color_sec.push_back(color);
		function_hide.push_back(false);
		error.push_back(false);
		now_function_amount++;
		now_drawText = false;
		update();
		});
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
	painter.drawRect(665, 10, 325, 780);

	if (!now_drawText) {
		drawInputText();
		now_drawText = true;
	}
		
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
	if (event->x() >= 915 && event->x() <= 945) {
		int del = (event->y() - 45) / 35;
		if (del < text.size() && del >= 0) {
			string test = text[del]->text().toStdString();
			stringstream str(test);
			string var;
			getline(str, var, '=');
			if (var == "y") {
				string value;
				getline(str, value, '\n');
				for (int k = 0; k < function.size(); k++) {
					if (function[k] == value)
						function.erase(function.begin() + k, function.begin() + k + 1);
				}
			}
			else if (var != "x" && var != "y" && var != "\0" && var.size() == 1) {
				string value;
				getline(str, value, '\n');
				for (int k = 0; k < variable.size(); k++) {
					if (variable[k].getName() == var && variable[k].getValue() == value)
						variable.erase(variable.begin() + k, variable.begin() + k + 1);
				}
			}
			text[del]->close();
			text.erase(text.begin() + del, text.begin() + del + 1);
			function_color.erase(function_color.begin() + del, function_color.begin() + del + 1);
			function_color_sec.erase(function_color_sec.begin() + del, function_color_sec.begin() + del + 1);
			function_hide.erase(function_hide.begin() + del, function_hide.begin() + del + 1);
			error.erase(error.begin() + del, error.begin() + del + 1);
			now_drawText = false;
			now_function_amount--;
			update();
		}
	}
	else if (event->x() >= 875 && event->x() <= 910) {
		int hide = (event->y() - 45) / 35;
		if (hide < text.size() && hide >= 0) {
			function_hide[hide] = !function_hide[hide];
			update();
		}
	}
}
void Viewer::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
		for (int i = 0; i < now_function_amount; i++) {
			QString temp;
			temp = text[i]->text();
			string test;
			test = temp.toStdString();

			stringstream str(test);
			string var;
			getline(str, var, '=');
			if (var == "y") {
				string value;
				getline(str, value, '\n');
				bool same = false;
				for (int i = 0; i < function.size(); i++) {
					if (function[i] == value) {
						same = true;
						break;
					}
				}
				if (!same) {
					function.push_back(value);
					error[i] = false;
				}
			}
			else if (var != "x" && var != "y" && var != "\0" && var.size() == 1) {
				int test = var[0];
				if (test >= 65 && test <= 90) {
					string value;
					getline(str, value, '\n');
					bool check = false;
					bool same = false;
					for (int i = 0; i < variable.size(); i++) {
						if (variable[i].getName() == var) {
							if (variable[i].getValue() == value)
								same = true;
							check = true;
							break;
						}
					}
					if (!check && variableCheck(value)) {
						variable.push_back(Variable(var, value));
						error[i] = false;
					}
					else if (!same)
						error[i] = true;
				}
				else if (test >= 97 && test <= 122) {
					string value;
					getline(str, value, '\n');
					bool check = false;
					bool same = false;
					for (int i = 0; i < variable.size(); i++) {
						if (variable[i].getName() == var) {
							if (variable[i].getValue() == value)
								same = true;
							check = true;
							break;
						}
					}
					if (!check && variableCheck(value)) {
						variable.push_back(Variable(var, value));
						error[i] = false;
					}	
					else if (!same)
						error[i] = true;
				}
			}
			else if (var != "\0") {
				error[i] = true;
			}
		}
		update();
	}
}
bool Viewer::variableCheck(string value) {
	Function test;
	for (int i = 0; i < variable.size(); i++) {
		test.setVariable(variable[i].getName(), 1);
	}
	test.setVariable("x", 1);
	if (test.replaceVariables(value) == "Error") {
		return false;
	}
	else if (test.calculate(test.replaceVariables(value)) == "Error")
		return false;
	return true;
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
	for (int i = 0; i < now_function_amount; i++) {
		if (!function_hide[i] && !error[i]) {
			pen.setColor(QColor(255, function_color[i], function_color_sec[i]));
			painter.setPen(pen);
			QString temp;
			temp = text[i]->text();
			string test;
			test = temp.toStdString();
			stringstream str(test);
			string var;
			getline(str, var, '=');
			if (var == "y") {
				string value;
				getline(str, value, '\n');
				bool have = false;
				for (auto& k : function) {
					if (k == value) {
						Function fun_test;
						fun_test.setVariable("x", 1);
						for (int j = 0; j < variable.size(); j++) {
							string var_value = fun_test.replaceVariables(variable[j].getValue());
							fun_test.setVariable(variable[j].getName(),1);
						}
						string test_first = fun_test.replaceVariables(value);
						if (test_first != "Error") {
							string test_second = fun_test.calculate(test_first);
							if (test_second != "Error") {
								error[i] = false;
								have = true;
							}
							else
								error[i] = true;
						}
						else
							error[i] = true;
					}
				}
				if (have) {
					double previous_x, previous_y;
					bool first = true;
					Function fun;
					for (int k = 0; k < 600; k++) {
						double x, y;
						Function fun;
						fun.setVariable("x", (double)k / (50 / range) + center_x - 300 / (50 / range));
						for (int j = 0; j < variable.size(); j++) {
							string var_value = fun.replaceVariables(variable[j].getValue());
							fun.setVariable(variable[j].getName(), stod(fun.calculate(var_value)));
						}
						string next = fun.replaceVariables(value);
						try {
							y = stod(fun.calculate(next));
							if (!first && ((center_y - previous_y) * (50 / range) + 310) < 610 && ((center_y - previous_y) * (50 / range) + 310) > 10)
							{
								if ((center_y - y) * (50 / range) + 310 > 610)
									painter.drawLine(k + 10, 610, previous_x + 10, (center_y - previous_y) * (50 / range) + 310);
								else if ((center_y - y) * (50 / range) + 310 < 10)
									painter.drawLine(k + 10, 10, previous_x + 10, (center_y - previous_y) * (50 / range) + 310);
								else
									painter.drawLine(k + 10, (center_y - y) * (50 / range) + 310, previous_x + 10, (center_y - previous_y) * (50 / range) + 310);
							}
							else if (!first && ((center_y - y) * (50 / range) + 310) < 610 && ((center_y - y) * (50 / range) + 310) > 10) {
								if ((center_y - previous_y) * (50 / range) + 310 > 610)
									painter.drawLine(previous_x + 10, 610, k + 10, (center_y - y) * (50 / range) + 310);
								else if ((center_y - previous_y) * (50 / range) + 310 < 10)
									painter.drawLine(previous_x + 10, 10, k + 10, (center_y - y) * (50 / range) + 310);
							}
							first = false;
							previous_x = k;
							previous_y = y;
						}
						catch(int x){
							first = true;
							continue;
						}
					}
				}
			}		
		}
		if(!function_hide[i])
			painter.drawPixmap(875, 45 + i * 35, 35, 35, display);
		else
			painter.drawPixmap(875, 45 + i * 35, 35, 35, hide);
		painter.drawPixmap(915, 45 + i * 35, 35, 35, cancel);
		QBrush brush(QColor(255, function_color[i], function_color_sec[i]));
		pen.setColor(QColor(255, function_color[i], function_color_sec[i]));
		painter.setPen(pen);
		painter.setBrush(brush);
		painter.drawRect(665, 45 + i * 35, 35, 35);
		if (error[i])
			painter.drawPixmap(665, 45 + i * 35, 35, 35, error_pix);
	}
}