#include "LevelCompletedWidget.h"
#include <QHBoxLayout>
#include <QPushButton>

LevelCompletedWidget::LevelCompletedWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Level Completed");

    mNextLevelButton = new QPushButton("&Next Level");
    mReplayLevelButton = new QPushButton("&Replay Current Level");
    mExitGameButton = new QPushButton("&Exit Game");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mNextLevelButton);
    layout->addWidget(mReplayLevelButton);
    layout->addWidget(mExitGameButton);
    setLayout(layout);

    connect(mNextLevelButton, SIGNAL(clicked()), this, SLOT(nextLevelClicked()));
    connect(mReplayLevelButton, SIGNAL(clicked()), this, SLOT(replayLevelClicked()));
    connect(mExitGameButton, SIGNAL(clicked()), this, SLOT(exitGameClicked()));
}

LevelCompletedWidget::~LevelCompletedWidget()
{
    delete mNextLevelButton;
    delete mReplayLevelButton;
    delete mExitGameButton;
}

void LevelCompletedWidget::nextLevelClicked()
{
    emit nextLevel();
    close();
}


void LevelCompletedWidget::replayLevelClicked()
{
    emit replayLevel();
    close();
}

void LevelCompletedWidget::exitGameClicked()
{
    emit exitGame();
    close();
}
