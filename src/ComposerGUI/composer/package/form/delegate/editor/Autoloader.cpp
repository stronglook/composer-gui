#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QRegExp>

#include "Autoloader.h"
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

Autoloader::Autoloader(QWidget *parent) : QDialog(parent)
{
    m_typeMapper = new QDataWidgetMapper(this);
    m_pathMapper = new QDataWidgetMapper(this);

    m_typeMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_pathMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_type = new QComboBox;
    m_path = new QLineEdit;

    m_type->addItems({"-", "psr-0", "psr-4", "files", "classmap", "exclude-from-classmap"});

    QFormLayout *editorLayout = new QFormLayout;
    editorLayout->addRow("Type:", m_type);
    editorLayout->addRow("Path:", m_path);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    editorLayout->addWidget(buttonBox);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    auto validator = [this]
    {
        bool valid = (m_type->currentText() != '-');
        if (!valid) {
            return false;
        }

        const QString &pathText = m_path->text().trimmed();
        if (m_isComplexPath) {
            static QRegExp pathRegex("^.+:.+");
            return pathRegex.exactMatch(pathText);
        }

        return !(pathText.isEmpty());
    };

    connect(m_type, &QComboBox::currentTextChanged, [=](const QString &text){
        m_isComplexPath = (text == "psr-0" || text == "psr-4");
        QString placeholer = m_isComplexPath ? "Namespace: path" : "Path";
        m_path->setPlaceholderText(placeholer);
        okButton->setEnabled(validator());
    });

    connect(m_path, &QLineEdit::textChanged, [=]
    {
        okButton->setEnabled(validator());
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, m_typeMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, m_pathMapper, &QDataWidgetMapper::submit);
    connect(buttonBox, &QDialogButtonBox::accepted, [this]
    {
        auto model = static_cast<model::FixedTreeDuplicateAware *>(const_cast<QAbstractItemModel *>(m_typeMapper->model()));
        model->mergeDuplicates();
    });


    setLayout(editorLayout);
}

void Autoloader::setModelIndex(const QModelIndex &index)
{
    m_typeMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_typeMapper->addMapping(m_type, 0);
    m_typeMapper->setCurrentModelIndex(index.parent());

    if (!index.parent().data().toString().isEmpty()) {
        m_type->setEnabled(false);
    }

    m_pathMapper->setModel(const_cast<QAbstractItemModel *>(index.model()));
    m_pathMapper->addMapping(m_path, 0);
    m_pathMapper->setRootIndex(index.parent());
    m_pathMapper->setCurrentModelIndex(index);
}

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
