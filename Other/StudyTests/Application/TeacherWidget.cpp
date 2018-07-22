#include "TeacherWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextTableFormat>
#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>

TeacherWidget::TeacherWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window);

    setFixedWidth(810);
    setFixedHeight(700);

    mBackButton = new QPushButton("Назад");
    mBackButton->setFixedSize(75, 25);
    mBackButton->setDisabled(true);

    QTextEdit* textEdit = new QTextEdit;
    textEdit->setPlainText("");
    textEdit->setFixedSize(709, 25);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(mBackButton);
    hLayout->addWidget(textEdit);


    mTableView = new QTableView();
    mTableModel = new TableModel(0);
    mTableView->setModel(mTableModel);
    mTableView->verticalHeader()->hide();

    mTableView->horizontalHeader()->resizeSection(0, 250);
    mTableView->horizontalHeader()->resizeSection(1, 150);
    mTableView->horizontalHeader()->resizeSection(2, 150);

    mTableView->setStyleSheet("QTableView { gridline-color: white; }" );

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(hLayout);
    layout->addWidget(mTableView);
    setLayout(layout);

    connect(mBackButton, SIGNAL(clicked()), this, SLOT(backClicked()));

    mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contexMenuClicked(const QPoint)));
}

TeacherWidget::~TeacherWidget()
{
    delete mBackButton;
}

void TeacherWidget::backClicked()
{
    //emit back();
    close();
}

void TeacherWidget::contexMenuClicked(const QPoint pos)
{
    QMenu* menu = new QMenu(this);
    QAction* open = new QAction("Відкрити", this);
    QAction* remove = new QAction("Видалити", this);
    connect(open, SIGNAL(triggered()), this, SLOT(openClicked()));
    connect(remove, SIGNAL(triggered()), this, SLOT(removeClicked()));
    menu->addAction(open);
    menu->addAction(remove);
    menu->popup(mTableView->viewport()->mapToGlobal(pos));
}


void TeacherWidget::openClicked()
{
    //emit back();
    close();
}

void TeacherWidget::removeClicked()
{
    int row = mTableView->selectionModel()->currentIndex().row();
    if(row >= 0)
    {
        if (QMessageBox::warning(this, "Видалити", "Ви точно  хочете видалити?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
        else
        {
            if(!mTableModel->removeRow(row))
            {
                QMessageBox::warning(this,"Повідомлення","Не вдалося видалити значення\n");
            }
            mTableView->setCurrentIndex(mTableModel->index(-1, -1));
        }
   }
}

