#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget1* parent) :
	QMainWindow1(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	remote12

	qDebug() << u8"Ö÷ĎßłĚID" << QThread::currentThreadId();

	connect(&m_thread1, SIGNAL(sigThread1()), this, SLOT(slot()), Qt::BlockingQueuedConnection);

	m_thread1.start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot()
{
	qDebug() << u8"Ö´ĐĐ˛ŰşŻĘý---ĎßłĚID" << QThread::currentThreadId();
}

