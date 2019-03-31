#pragma once

#include <QTabWidget>
#include <QScrollArea>
#include <QWidget>

#include "composer/package/form/Authors.h"
#include "composer/package/form/Autoload.h"
#include "composer/package/form/Config.h"
#include "composer/package/form/Dependencies.h"
#include "composer/package/form/General.h"
#include "composer/package/form/Repositories.h"
#include "composer/package/form/Scripts.h"
#include "composer/package/form/Support.h"
#include "composer/package/form/visitor/VisitorInterface.h"
#include "composer/package/form/visitor/AcceptorInterface.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class Form : public QTabWidget, public visitor::AcceptorInterface
{
public:
    Form(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;

private:
    package::form::General *m_general;
    package::form::Dependencies *m_dependencies;
    package::form::Autoload *m_autoload;
    package::form::Repositories *m_repositories;
    package::form::Scripts *m_scripts;
    package::form::Config *m_config;
    package::form::Authors *m_authors;
    package::form::Support *m_support;

    QScrollArea *createScrollForWidget(QWidget *widget);
};

} // form
} // package
} // composer
} // composer_gui
