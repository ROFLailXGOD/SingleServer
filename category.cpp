#include "category.h"

Category::Category()
{
    amount = 0;
}

Category::GetCategory()
{
    return category;
}

Category::GetAmount()
{
    return amount;
}

Category::SetCategory(QString name)
{
    category = name;
}
