#ifndef DELAY_H
#define DELAY_H

#include <QEventLoop>
#include <QTimer>

class Delay
{
public:
    static void run(int mSec)
    {
        QEventLoop loop;
        QTimer::singleShot(mSec, &loop, SLOT(quit()));
        loop.exec();
    }
};

#endif // DELAY_H
