#include "thread1.h"
#include <QDebug>

Thread1::Thread1(QThread* parent)
	: QThread(parent)
{

}

void Thread1::run()
{
	qDebug() << u8"Thread1�߳�ID" << QThread::currentThreadId();

	emit sigThread1();

	for (int i = 0; i < 1000; i++) //�˴�Ϊ10000�Σ��ӳ�ʱ�䣬�Ա������Ĺ۲�����
	{
		qDebug() << i;
	}
}

