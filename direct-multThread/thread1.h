#ifndef THREAD1_H
#define THREAD1_H

#include <QThread>

class Thread1 : public QThread
{
	Q_OBJECT
public:
	explicit Thread1(QThread* parent = 0);

protected:
	virtual void run();

signals:
	void sigThread1();
};

#endif // THREAD1_H

