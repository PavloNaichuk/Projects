#include "ScreenResolutions.h"
#include <QtWidgets>

ScreenResolutions::ScreenResolutions(QWidget *parent)
    : QGroupBox("Screen resolutions:",parent)
{
    setCheckable(true);
    setChecked(true);
    mFullSize= new QRadioButton("1920x1080");
    mMediumSize = new QRadioButton ("1280x720");
    mSmallSize = new QRadioButton("1024x768");
    mApply = new QPushButton("&Apply");
    mFullSize->setChecked(true);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(mFullSize);
    vLayout->addWidget(mMediumSize);
    vLayout->addWidget(mSmallSize);
    vLayout->addWidget(mApply);
    setLayout(vLayout);
    setWindowTitle("Screen resolutions");
    resize(300,100);


    connect(mApply, SIGNAL(clicked()), SLOT(quitWindow()));
}

void ScreenResolutions::quitWindow()
{
    QCoreApplication::quit();
}


