#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	qDebug() << u8"���߳�ID" << QThread::currentThreadId();

	connect(&m_thread1, SIGNAL(sigThread1()), this, SLOT(slot()), Qt::DirectConnection);

	m_thread1.start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot()
{
	qDebug() << u8"ִ�вۺ���---�߳�ID" << QThread::currentThreadId();
}

