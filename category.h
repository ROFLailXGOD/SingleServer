#pragma once
#include <QMetaType>
#include <QDataStream>

class Category
{
private:
    QString category;
    int amount;
public:
    Category();

    QString GetCategory();
    int GetAmount();

    void SetCategory(QString);
};
Q_DECLARE_METATYPE(Category*)
