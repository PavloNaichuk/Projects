#pragma once

#include <QWidget>

class QPushButton;

class LevelCompletedWidget : public QWidget
{
    Q_OBJECT

public:
   LevelCompletedWidget(QWidget* parent = 0);
    ~LevelCompletedWidget();

signals:
    void nextLevel();
    void replayLevel();
    void exitGame();

public slots:
    void nextLevelClicked();
    void replayLevelClicked();
    void exitGameClicked();

private:
    QPushButton* mNextLevelButton;
    QPushButton* mReplayLevelButton;
    QPushButton* mExitGameButton;
};
