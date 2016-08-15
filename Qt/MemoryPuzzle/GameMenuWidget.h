#ifndef GAMEMENUWIDGET_H
#define GAMEMENUWIDGET_H

#include <QWidget>

class QPushButton;

class GameMenuWidget : public QWidget
{
    Q_OBJECT

public:
    GameMenuWidget(QWidget* parent = 0);
    ~GameMenuWidget();

signals:
    void resume();
    void exitGame();

public slots:
    void resumeClicked();
    void exitGameClicked();

private:
    QPushButton* mResumeButton;
    QPushButton* mExitGameButton;
};

#endif // GAMEMENUWIDGET_H
