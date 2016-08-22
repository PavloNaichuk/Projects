#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QMessageBox>

class HelpWidget : public QMessageBox
{
    Q_OBJECT

public:
    HelpWidget(QMessageBox* parent = 0);
    ~HelpWidget();

};

#endif // HELPWIDGET_H
