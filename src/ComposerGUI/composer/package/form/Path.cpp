#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileInfo>
#include <QFileDialog>
#include <QFormLayout>

#include "Path.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Path::Path(QWidget *parent) : QWidget(parent)
{
    QLineEdit *path = new QLineEdit;
    path->setPlaceholderText("No path selected");
    path->setReadOnly(true);

    QLabel *status = new QLabel;
    status->setWordWrap(true);

    QPushButton *browseButton = new QPushButton("Browse");
    QObject::connect(browseButton, &QPushButton::clicked, [=]
    {
        m_selectedPath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(nullptr, "Select path"));
        if (!m_selectedPath.isEmpty())
        {
            path->setText(m_selectedPath);

            QFileInfo pathInfo(m_selectedPath);

            if (!pathInfo.isWritable()) {
                status->setText("Path is not writeable");
                emit pathChanged(false);
            } else {
                QString msg;
                bool failure;

                if ((failure = pathInfo.exists(m_selectedPath + QDir::separator() + "composer.json"))) {
                    msg = "Directory already has composer.json";
                }

                status->setText(msg);
                emit pathChanged(!failure);
            }
        }
    });

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(path);
    pathLayout->addWidget(browseButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Project path:", pathLayout);
    formLayout->addWidget(status);
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);

    setLayout(formLayout);
}

const QString &Path::getSelectedPath()
{
    return m_selectedPath;
}

} // form
} // package
} // composer
} // composer_gui
