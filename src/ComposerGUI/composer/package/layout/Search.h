#pragma once

#include <QVBoxLayout>
#include <QLabel>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace layout
{

class Search : public QVBoxLayout
{
public:
    Search();
    void clear(bool searchStarted);

private:
    QLabel *m_searchLabel;
};

} // layout
} // package
} // composer
} // composer_gui
