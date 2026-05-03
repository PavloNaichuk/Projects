#ifndef GAMETIMER_H
#define GAMETIMER_H
#include <QTimer>
#include <QTime>

class GameTimer : public QObject
{
    Q_OBJECT

public:
    GameTimer();

    void start();
    void pause();
    void resume();
    void update();

signals:
    void timeChanged(int);

private:
    bool mStarted;
    bool mPaused;
    int mElapsedMs;
    QTime mTime;
};

#endif // GAMETIMER_H
