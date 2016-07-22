#include "MainWidget.h"
#include "CardBoard.h"
#include "CardBoardWidget.h"
#include "LevelGenerator.h"
#include "GameLogic.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWidget::MainWidget(GameLogic* gameLogic, QWidget* parent)
    : QWidget(parent)
    , mGameLogic(gameLogic)
    , mTime(new QTime(0,0))
    , mTimer(new QTimer())
    , mCardBoardWidget(new CardBoardWidget(gameLogic))

{
    mLevelLabel = new QLabel("Level: 1");
    mScoreLabel = new QLabel(QString("Score: %1").arg(mGameLogic->getTotalScore()));

    mTimeLabel = new QLabel(QString("Time: ") + mTime->toString("hh:mm:ss"));
    mTimeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    mTime->start();
    mTimer->start(1000);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(mLevelLabel);
    hLayout->addWidget(mScoreLabel);
    hLayout->addWidget(mTimeLabel);

    mLevelLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mScoreLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mTimeLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mCardBoardWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mCardBoardWidget);

    resize(800, 600);
    setLayout(vLayout);

    connect(mGameLogic, SIGNAL(scoreChanged()), this, SLOT(updateScore()));
}

MainWidget::~MainWidget()
{
    delete mTimer;
    delete mTime;
    delete mCardBoardWidget;
    delete mLevelLabel;
    delete mScoreLabel;
    delete mTimeLabel;
}

void MainWidget::setCardBoard(CardBoard* cardBoard)
{
    mCardBoardWidget->setCardBoard(cardBoard);
}

const CardBoard* MainWidget::getCardBoard() const
{
    return mCardBoardWidget->getCardBoard();
}

void MainWidget::updateTime()
{
    int elapsedTimeMs = mTime->elapsed();
    int elapsedTimeSec = elapsedTimeMs / 1000;

    mTimeLabel->setText(QString("Time: ") + QTime(0, 0, elapsedTimeSec).toString("hh:mm:ss"));
}

void MainWidget::updateScore()
{
    mScoreLabel->setText(QString("Score: %1").arg(mGameLogic->getTotalScore()));
}
