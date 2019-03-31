#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QLabel>

#include "Window.h"
#include "globals.h"
#include "console/factory/Factory.h"

#include "factory/MainWindow.h"

namespace composer_gui
{
namespace main
{

Window::Window(TExplorer *explorer,
               TOverview *overview,
               QTabWidget *consoleTabs,
               QWidget *parent)
    : QMainWindow(parent),
      m_explorer(explorer),
      m_overview(overview),
      m_consoleTabs(consoleTabs)
{

    QVBoxLayout *leftColumnLayout =  new QVBoxLayout;
    leftColumnLayout->addWidget(m_explorer);

    QVBoxLayout *rightColumnLayout = new QVBoxLayout;
    rightColumnLayout->addWidget(m_overview, 7);
    rightColumnLayout->addWidget(m_consoleTabs, 4);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(leftColumnLayout, 3);
    centralLayout->addLayout(rightColumnLayout, 7);

    QWidget *central = new QWidget;
    central->setLayout(centralLayout);

    setCentralWidget(central);
}

} // main
} // composer_gui
