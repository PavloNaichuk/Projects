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
#include "FileUtilities.h"
#include "Component.h"
#include "CategoryWidget.h"
#include "TestWidget.h"

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

    auto fileData = LoadComponentFromFile("Tests.data");
    mTableView = new QTableView();
    mTableModel = new TableModel(std::move(fileData));
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

    //mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contexMenuClicked(const QPoint)));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contexMenuComponent(const QPoint)));
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
    QMenu menu;

    QAction* openAction = new QAction("Відкрити", &menu);
    QAction* removeAction = new QAction("Видалити", &menu);

    connect(openAction, SIGNAL(triggered()), this, SLOT(openClicked()));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeClicked()));

    menu.addAction(openAction);
    menu.addAction(removeAction);

    menu.exec(mTableView->mapToGlobal(pos));
}

void TeacherWidget::contexMenuComponent(const QPoint pos)
{
    QMenu menu;
    QMenu* createMenu;

    createMenu = menu.addMenu("Додати");

    QAction* categoryAction = new QAction("Категорію", createMenu);
    QAction* testAction = new QAction("Тест", createMenu);
    createMenu->addAction(categoryAction);
    createMenu->addAction(testAction);

    QAction* sortByAction = new QAction("Відсортувати", &menu);

    connect(categoryAction, SIGNAL(triggered()), this, SLOT(addCategoryClicked()));
    connect(testAction, SIGNAL(triggered()), this, SLOT(addTestClicked()));
    connect(sortByAction, SIGNAL(triggered()), this, SLOT(sortByClicked()));

    menu.addAction(sortByAction);
    menu.exec(mapToGlobal(pos));
}

void TeacherWidget::openClicked()
{
    //if( mComponent->type() == CATEGORY)
   // {
        mCategoryWidget = new CategoryWidget();
        mCategoryWidget->show();
   // }

   /* if( mComponent->type() == TEST)
    {
        mTestWidget = new TestWidget();
        mTestWidget->show();
    }*/
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
            mTableModel->removeRows(mTableModel->rowCount(), mTableModel->columnCount());
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

void TeacherWidget::addCategoryClicked()
{
    mTableModel->insertRows(mTableModel->rowCount(), mTableModel->columnCount());
}
void TeacherWidget::addTestClicked()
{
    mTableModel->insertRows(mTableModel->rowCount(), mTableModel->columnCount());
}


void TeacherWidget::sortByClicked()
{
    close();
}

