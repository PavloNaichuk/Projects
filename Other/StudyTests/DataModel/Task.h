#pragma once

#include "Component.h"
#include "Visitor.h"

#include <QString>

class Task : public Component
{
public:
    Task();

    bool equals(const Component* comp) const override;
    void visit(Visitor* visitor) const override;

    const QString& question() const;
    void setQuestion(QString question);

    const QString& answer() const;
    void setAnswer(QString answer);

    int numPoints() const;
    void setNumPoints(int numPoints);

private:
    QString mQuestion;
    QString mAnswer;
    int mNumPoints;
};
