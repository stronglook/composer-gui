#include <QPixmap>
#include <QLineEdit>
#include <QPlainTextEdit>

#include "ViewPackage.h"
#include "globals.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace overview
{
namespace state
{

ViewPackage::ViewPackage(QWidget *parent)
    : QWidget(parent),
      m_mapper(new QDataWidgetMapper(this)),
      m_mapperSection(0)
{

    QStandardItemModel *model = new QStandardItemModel(12, 1, this);
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->setModel(model);

    QHBoxLayout *statistics = createStatisticsLayout();

    QHBoxLayout *forms = new QHBoxLayout;
    forms->addLayout(createFormLayout({"Minimum-stability:", "Type:", "License:", "Abandoned:"}));
    forms->addLayout(createFormLayout({"Keywords:", "Time:", "Homepage:", "Readme:"}));

    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::HLine);

    QVBoxLayout *viewPackageLayout = new QVBoxLayout;
    viewPackageLayout->addLayout(statistics);
    viewPackageLayout->addWidget(frame);
    viewPackageLayout->addLayout(forms);
    viewPackageLayout->addLayout(createDescriptionFormLayout());

    m_mapper->toFirst();

    connect(model, &QStandardItemModel::itemChanged, m_mapper, &QDataWidgetMapper::toFirst);

    setLayout(viewPackageLayout);
    setStyleSheet(COMPOSERGUI_WIDGET_BACKGROUND);
}

QFormLayout *ViewPackage::createFormLayout(const QStringList &labels)
{
    QFormLayout *form = new QFormLayout;

    for (const QString &label : labels)
    {
        QLineEdit *input = new QLineEdit;
        input->setReadOnly(true);
        input->setPlaceholderText("value not set");
        form->addRow(label, input);

        m_mapper->addMapping(input, m_mapperSection++);
    }

    form->setRowWrapPolicy(QFormLayout::WrapAllRows);

    return form;
}

QFormLayout *ViewPackage::createDescriptionFormLayout()
{
    QFormLayout *form = new QFormLayout;

    QPlainTextEdit *textEdit = new QPlainTextEdit;
    textEdit->setReadOnly(true);
    textEdit->setPlaceholderText("value not set");

    m_mapper->addMapping(textEdit, m_mapperSection++);

    form->addRow("Description:", textEdit);
    form->setRowWrapPolicy(QFormLayout::WrapAllRows);

    return form;
}

QHBoxLayout *ViewPackage::createStatisticsLayout()
{
    QLabel *downloadsIcon = new QLabel;
    downloadsIcon->setPixmap(QPixmap(":/icons/down.png"));

    QLabel *rateIcon = new QLabel;
    rateIcon->setPixmap(QPixmap(":/icons/rate.png"));

    QLabel *downloads = new QLabel;
    m_mapper->addMapping(downloads, m_mapperSection++, "text");

    QLabel *rate = new QLabel;
    m_mapper->addMapping(rate, m_mapperSection++, "text");

    QHBoxLayout *numericStatistics = new QHBoxLayout;
    numericStatistics->addWidget(downloadsIcon);
    numericStatistics->addWidget(downloads);
    numericStatistics->addWidget(rateIcon);
    numericStatistics->addWidget(rate);
    numericStatistics->setAlignment(Qt::AlignLeft);

    QLabel *locked = new QLabel;
    m_mapper->addMapping(locked, m_mapperSection++, "text");

    QHBoxLayout *lockedVersion = new QHBoxLayout;
    lockedVersion->addWidget(m_locked = new QLabel("Locked:"));
    lockedVersion->addWidget(locked);
    lockedVersion->setAlignment(Qt::AlignRight);

    QHBoxLayout *statistics = new QHBoxLayout;
    statistics->addLayout(numericStatistics);
    statistics->addLayout(lockedVersion);

    return statistics;
}

QStandardItemModel *ViewPackage::getMappingModel()
{
    return static_cast<QStandardItemModel *>(m_mapper->model());
}

void ViewPackage::showLocked(bool flag)
{
    m_locked->setVisible(flag);
}

} // state
} // overview
} // package
} // composer
} // composer_gui
