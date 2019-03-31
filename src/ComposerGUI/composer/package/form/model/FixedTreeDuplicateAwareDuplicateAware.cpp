#include "FixedTreeDuplicateAware.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace model
{

FixedTreeDuplicateAware::FixedTreeDuplicateAware(QObject *parent) : QStandardItemModel(parent)
{

}

void FixedTreeDuplicateAware::mergeDuplicates()
{
    int counter = rowCount();
    for (int i = 0; i < counter; i++)
    {
        const QString &iText = item(i)->text();
        for (int j = i + 1; j < rowCount(); j++)
        {
            const QString &jText = item(j)->text();
            if (iText == jText) {
                QStandardItem *current = item(j);
                QStandardItem *child = current->takeChild(0);
                item(i)->setChild(item(i)->rowCount(), 0, child);

                auto rowItems = takeRow(j);
                for (auto rowItem : rowItems)
                {
                    delete rowItem;
                }

                --counter;
            }
        }
    }
}

} // model
} // form
} // package
} // composer
} // composer_gui
