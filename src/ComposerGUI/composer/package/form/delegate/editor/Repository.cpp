#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "Repository.h"
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

Repository::Repository(QWidget *parent)
    : QDialog(parent),
      m_type(new QComboBox),
      m_url(new QLineEdit),
      m_typeMapper(new QDataWidgetMapper(this)),
      m_urlMapper(new QDataWidgetMapper(this))
{
    m_typeMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_urlMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_type->addItems({
        "-",
        "vcs",
        "git",
        "svn",
        "pear",
        "path",
        "composer",
        "artifact",
        "git-bitbucket",
        "hg-bitbucket",
        "github",
        "gitlab",
        "perforce",
        "fossil",
        "hg"
     });

    QFormLayout *common = new QFormLayout;
    common->addRow("Type:", m_type);
    common->addRow("Url:", m_url);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    auto validator = [=]
    {
        bool isValid = (m_type->currentText() != '-') && !(m_url->text().trimmed().isEmpty());
        okButton->setEnabled(isValid);
    };

    connect(m_type, &QComboBox::currentTextChanged, validator);
    connect(m_url, &QLineEdit::textChanged, validator);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, m_typeMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, m_urlMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, [this]
    {
        auto model = static_cast<model::FixedTreeDuplicateAware *>(const_cast<QAbstractItemModel *>(m_typeMapper->model()));
        model->mergeDuplicates();
    });

    common->addWidget(buttonBox);

    setLayout(common);
}

void Repository::setModelIndex(const QModelIndex &index)
{
    m_typeMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_typeMapper->addMapping(m_type, 0);
    m_typeMapper->setCurrentModelIndex(index.parent());

    if (!index.parent().data().toString().isEmpty()) {
        m_type->setEnabled(false);
    }

    m_urlMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_urlMapper->addMapping(m_url, 0);
    m_urlMapper->setRootIndex(index.parent());
    m_urlMapper->setCurrentModelIndex(index);
}

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
