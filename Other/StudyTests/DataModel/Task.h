#pragma once

#include "Component.h"
#include <QString>

class Task : public Component
{
public:
    const QString& question() const;
    void setQuestion(QString question);

    const QString& answer() const;
    void setAnswer(QString answer);

    int numPoints() const;
    void setNumPoints(unsigned numPoints);

    bool equals(const Component* comp) const;

private:
    QString mQuestion;
    QString mAnswer;
    unsigned mNumPoints;
};
