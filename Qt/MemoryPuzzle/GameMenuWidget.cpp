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
    mExitGameButton = new QPushButton("&Exit Game");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mResumeButton);
    layout->addWidget(mExitGameButton);
    setLayout(layout);

    resize(250, 100);
    connect(mResumeButton, SIGNAL(clicked()), this, SLOT(resumeClicked()));
    connect(mExitGameButton, SIGNAL(clicked()), this, SLOT(exitGameClicked()));
}

GameMenuWidget::~GameMenuWidget()
{
    delete mResumeButton;
    delete mExitGameButton;
}

void GameMenuWidget::resumeClicked()
{
    emit resume();
    hide();
}

void GameMenuWidget::exitGameClicked()
{
    emit exitGame();
    hide();
}
