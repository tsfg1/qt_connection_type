#include "thread1.h"
#include <QDebug>

Thread1::Thread1(QThread* parent)
	: QThread(parent)
{

}

void Thread1::run()
{
	qDebug() << u8"Thread1线程ID" << QThread::currentThreadId();

	emit sigThread1();

	for (int i = 0; i < 1000; i++) //此处为10000次，加长时间，以便更清楚的观察现象
	{
		qDebug() << i;
	}
}

