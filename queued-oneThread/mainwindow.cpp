#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(this, SIGNAL(sig()), this, SLOT(slot()), Qt::QueuedConnection);

	emit sig();

	for (int i = 0; i < 10; i++)
	{
		qDebug() << i;
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot()
{
	qDebug() << u8"Ö´ÐÐ²Ûº¯Êý";
}

