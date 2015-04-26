#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include <QtWidgets/QMainWindow>
#include <qtimer.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qevent.h>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "ui_mainwindow.h"

#include "TCP.h"
#include "LineFollower.h"

#define DEBUG true
#define APP_NAME "The Reaper - Autonomous"
#define TIME 20
#define LABVIEW true // give true if app is launched from labview

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void nextFrame();

private:
	void closeEvent(QCloseEvent *event);

	Ui::MainWindowClass ui;
	QTimer *fpsTimer;
	
	/* Used to display image in QLabel
	---------------------------------- */
	QImage img;
	QPixmap pixel;
	// ----------------------------------
	
	cv::Mat frame;
	cv::VideoCapture stream;

	TCP *labview;
	LineFollower imageLineFollower;
};

#endif // MAINWINDOW_H
