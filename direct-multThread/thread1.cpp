#include "thread1.h"
#include <QDebug>

Thread1::Thread1(QThread* parent)
	: QThread(parent)
{

}

void Thread1::run()
{
	qDebug() << u8"Thread1Ïß³ÌID" << QThread::currentThreadId();

	emit sigThread1();

	for (int i = 0; i < 10; i++)
	{
		qDebug() << i;
	}
}

