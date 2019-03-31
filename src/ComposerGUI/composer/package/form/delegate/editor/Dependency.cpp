#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QRegExp>

#include "Dependency.h"

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

Dependency::Dependency(QWidget *parent)
    : QDialog(parent),
      m_package(new QLineEdit),
      m_mapper(new QDataWidgetMapper(this))
{
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_package->setPlaceholderText("vendor/name@version");

    QFormLayout *editorLayout = new QFormLayout;
    editorLayout->addRow("Package: ", m_package);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!m_package->text().isEmpty());

    editorLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, m_mapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_package, &QLineEdit::textChanged, [buttonBox](const QString &text)
    {
        static QRegExp r("^.+/.+@.+");
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(r.exactMatch(text.trimmed()));
    });

    setLayout(editorLayout);
}

void Dependency::setModelIndex(const QModelIndex &index)
{
    m_mapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_mapper->addMapping(m_package, 0);
    m_mapper->setCurrentModelIndex(index);
}

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
