#include "GameMenuWidget.h"
#include <QHBoxLayout>
#include <QPushButton>

GameMenuWidget::GameMenuWidget(QDialog* parent)
    : QDialog(parent)
{
    setWindowTitle("Game Menu");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);

    mResumeButton = new QPushButton("&Resume");
    mShowHelpButton = new QPushButton("&Show Help");
    mExitGameButton = new QPushButton("&Exit Game");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mResumeButton);
    layout->addWidget(mShowHelpButton);
    layout->addWidget(mExitGameButton);
    setLayout(layout);

    resize(250, 100);
    connect(mResumeButton, SIGNAL(clicked()), this, SLOT(resumeClicked()));
    connect(mShowHelpButton, SIGNAL(clicked()), this, SLOT(showHelpClicked()));
    connect(mExitGameButton, SIGNAL(clicked()), this, SLOT(exitGameClicked()));
}

GameMenuWidget::~GameMenuWidget()
{
    delete mResumeButton;
    delete mShowHelpButton;
    delete mExitGameButton;
}

void GameMenuWidget::resumeClicked()
{
    emit resume();
    close();
}

void GameMenuWidget::exitGameClicked()
{
    emit exitGame();
    close();
}

void GameMenuWidget::showHelpClicked()
{
    emit showHelp();
    show();
}
