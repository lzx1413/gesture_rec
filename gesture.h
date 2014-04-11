#ifndef GESTURE_H
#define GESTURE_H

#include <QtWidgets/QMainWindow>
#include "ui_gesture.h"
#include <qpushbutton.h>

class Gesture : public QMainWindow
{
	Q_OBJECT

public:
	Gesture(QWidget *parent = 0);
	~Gesture();
	void Gesture_recognition();
	QPushButton startButton;
private:
	Ui::GestureClass ui;
	
};

#endif // GESTURE_H
