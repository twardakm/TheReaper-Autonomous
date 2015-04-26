#include "mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	if (LABVIEW)
	{
		labview = new TCP(this);
		if (labview->connectTCP())
			if (!DEBUG)
				exit(1);
		if (labview->sendMessage(APP_NAME))
		{
			labview->closeTCP();
			if (!DEBUG)
				exit(1);
		}

		QString maxSpeedQString = labview->receiveMessage();
		char * maxSpeedChar = new char[2];
		strcpy(maxSpeedChar, maxSpeedQString.toStdString().c_str());
		int maxSpeedInt = (int)maxSpeedChar[0];
		imageLineFollower.setMaxSpeed(maxSpeedInt);

	}

	fpsTimer = new QTimer(this);
	connect(fpsTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
	fpsTimer->start(TIME);

	stream.open(0); // open device
	stream.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	stream.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, APP_NAME,
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	}
	else 
	{
		fpsTimer->stop();
		if (LABVIEW)
			labview->closeTCP();
		event->accept();
	}
}

void MainWindow::nextFrame()
{
	stream.read(frame);
	imageLineFollower.nextFrame(frame); // Process all data

	img = QImage((uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
	pixel = QPixmap::fromImage(img);
	ui.videoQt->setPixmap(pixel);

	char data[2];
	data[0] = (char)imageLineFollower.getSpeed();
	data[1] = (char)(imageLineFollower.getServo());

	if (LABVIEW)
		if (labview->sendMessage(data, true, 2)) //exit if error while sending
			exit(1);
}