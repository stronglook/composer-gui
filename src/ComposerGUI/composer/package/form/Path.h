#pragma once

#include <QWidget>
#include <QString>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class Path : public QWidget
{
    Q_OBJECT
public:
    Path(QWidget *parent = nullptr);
    const QString &getSelectedPath();

private:
    QString m_selectedPath;

signals:
    void pathChanged(bool);
};

} // form
} // package
} // composer
} // composer_gui
