#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include "Author.h"

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

Author::Author(QWidget *parent) : QDialog(parent)
{
    m_name       = new QLineEdit;
    m_role       = new QLineEdit;
    m_email      = new QLineEdit;
    m_homepage   = new QLineEdit;
    m_mapper     = new QDataWidgetMapper(this);

    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    QFormLayout *editorLayout = new QFormLayout;
    editorLayout->addRow("Name:", m_name);
    editorLayout->addRow("Role:", m_role);
    editorLayout->addRow("Email:", m_email);
    editorLayout->addRow("Homepage:", m_homepage);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    auto validator = [this, okButton]
    {
        bool isAllEmpty = m_name->text().trimmed().isEmpty()
                            && m_role->text().trimmed().isEmpty()
                            && m_email->text().trimmed().isEmpty()
                            && m_homepage->text().trimmed().isEmpty();

        okButton->setEnabled(!isAllEmpty);
    };

    connect(m_name, &QLineEdit::textChanged, validator);
    connect(m_role, &QLineEdit::textChanged, validator);
    connect(m_email, &QLineEdit::textChanged, validator);
    connect(m_homepage, &QLineEdit::textChanged, validator);

    editorLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, m_mapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(editorLayout);
}

void Author::setModelIndex(const QModelIndex &index)
{
    m_mapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_mapper->addMapping(m_name, 0);
    m_mapper->addMapping(m_role, 1);
    m_mapper->addMapping(m_email, 2);
    m_mapper->addMapping(m_homepage, 3);
    m_mapper->setCurrentModelIndex(index);
}

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
