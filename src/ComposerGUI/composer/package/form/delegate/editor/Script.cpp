#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "Script.h"
#include "composer/package/form/model/FixedTreeDuplicateAware.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace delegate
{
namespace editor
{

Script::Script(QWidget *parent)
    : QDialog(parent),
      m_event(new QComboBox),
      m_handler(new QLineEdit),
      m_eventMapper(new QDataWidgetMapper(this)),
      m_handlerMapper(new QDataWidgetMapper(this))
{
    m_handlerMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_eventMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_event->addItems({
        "-",
        "pre-install-cmd",
        "post-install-cmd",
        "pre-update-cmd",
        "post-update-cmd",
        "post-status-cmd",
        "pre-archive-cmd",
        "post-archive-cmd",
        "pre-autoload-dump",
        "post-autoload-dump",
        "post-root-package-install",
        "post-create-project-cmd",
        "pre-dependencies-solving",
        "post-dependencies-solving",
        "pre-package-install",
        "post-package-install",
        "pre-package-update",
        "post-package-update",
        "pre-package-uninstall",
        "post-package-uninstall",
        "init",
        "command",
        "pre-file-download",
        "pre-command-run",
     });

    QFormLayout *common = new QFormLayout;
    common->addRow("Event:", m_event);
    common->addRow("Handler:", m_handler);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    auto validator = [this, okButton]
    {
        bool isValid = (m_event->currentText() != '-') && !(m_handler->text().trimmed().isEmpty());
        okButton->setEnabled(isValid);
    };

    connect(m_event, &QComboBox::currentTextChanged, validator);
    connect(m_handler, &QLineEdit::textChanged, validator);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, m_eventMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, m_handlerMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, [this]
    {
        auto model = static_cast<model::FixedTreeDuplicateAware *>(const_cast<QAbstractItemModel *>(m_eventMapper->model()));
        model->mergeDuplicates();
    });

    common->addWidget(buttonBox);

    setLayout(common);
}

void Script::setModelIndex(const QModelIndex &index)
{
    m_eventMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_eventMapper->addMapping(m_event, 0);
    m_eventMapper->setCurrentModelIndex(index.parent());

    if (!index.parent().data().toString().isEmpty()) {
        m_event->setEnabled(false);
    }

    m_handlerMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_handlerMapper->addMapping(m_handler, 0);
    m_handlerMapper->setRootIndex(index.parent());
    m_handlerMapper->setCurrentModelIndex(index);
}

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
