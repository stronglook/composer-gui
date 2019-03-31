#-------------------------------------------------
#
# Project created by QtCreator 2018-11-13T16:19:50
#
#-------------------------------------------------

QT       += core gui network concurrent

RC_ICONS = images/composer.ico
VERSION = 1.0.0.0
QMAKE_TARGET_COMPANY = Asif Mammadov
QMAKE_TARGET_PRODUCT = ComposerGUI
QMAKE_TARGET_DESCRIPTION = Graphical client for Composer
QMAKE_TARGET_COPYRIGHT = Asif Mammadov

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComposerGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        main.cpp \
    console/Console.cpp \
    console/command/Clear.cpp \
    console/command/Pwd.cpp \
    console/command/Ls.cpp \
    console/command/Cd.cpp \
    console/command/Unknown.cpp \
    console/command/Composer.cpp \
    console/command/completer/Completer.cpp \
    console/command/Php.cpp \
    console/command/Help.cpp \
    console/command/invoker/Invoker.cpp \
    console/command/AbstractCommand.cpp \
    console/factory/Factory.cpp \
    console/command/Command.cpp \
    composer/package/form/General.cpp \
    composer/package/form/visitor/DataProviderVisitor.cpp \
    composer/package/form/Support.cpp \
    composer/package/form/Authors.cpp \
    composer/package/form/delegate/AuthorRow.cpp \
    composer/package/form/delegate/editor/Author.cpp \
    composer/package/form/Config.cpp \
    composer/package/form/Repositories.cpp \
    composer/package/form/delegate/RepositoryRow.cpp \
    composer/package/form/delegate/editor/Repository.cpp \
    composer/package/form/Scripts.cpp \
    composer/package/form/Autoload.cpp \
    composer/package/form/visitor/DataExtractVisitor.cpp \
    composer/package/form/delegate/ScriptRow.cpp \
    composer/package/form/delegate/editor/Script.cpp \
    composer/package/form/delegate/AutoloadRow.cpp \
    composer/package/form/delegate/editor/Autoloader.cpp \
    composer/package/form/controls/Standard.cpp \
    composer/package/form/controls/Context.cpp \
    composer/package/form/Dependencies.cpp \
    composer/api/packagist/Packagist.cpp \
    composer/package/Package.cpp \
    composer/package/layout/Search.cpp \
    composer/package/form/delegate/editor/Dependency.cpp \
    concurrent/thread/Thread.cpp \
    composer/package/form/delegate/DependencyRow.cpp \
    composer/package/form/controls/FixedTree.cpp \
    composer/package/form/model/FixedTreeDuplicateAwareDuplicateAware.cpp \
    main/Window.cpp \
    paginator/Paginator.cpp \
    composer/project/explorer/Explorer.cpp \
    composer/package/form/Form.cpp \
    composer/package/form/Path.cpp \
    composer/project/explorer/controls/ProjectExplorerContext.cpp \
    main/factory/MenuBar.cpp \
    main/factory/MainWindow.cpp \
    composer/package/overview/Overview.cpp \
    composer/package/overview/state/Stub.cpp \
    composer/package/overview/state/ViewPackage.cpp \
    settings/Settings.cpp \
    composer/package/form/delegate/BaseDelegate.cpp

HEADERS += \
    console/Console.h \
    console/command/Clear.h \
    console/command/AbstractCommand.h \
    console/command/Pwd.h \
    console/command/Ls.h \
    console/command/Cd.h \
    console/command/Unknown.h \
    console/command/Composer.h \
    console/command/completer/Completer.h \
    console/command/Php.h \
    console/command/Help.h \
    console/command/invoker/Invoker.h \
    console/factory/Factory.h \
    console/command/Command.h \
    composer/package/form/General.h \
    composer/package/form/visitor/DataProviderVisitor.h \
    composer/package/form/visitor/VisitorInterface.h \
    composer/package/form/visitor/AcceptorInterface.h \
    composer/package/form/Support.h \
    composer/package/form/Authors.h \
    composer/package/form/delegate/AuthorRow.h \
    composer/package/form/delegate/editor/Author.h \
    composer/package/form/Config.h \
    composer/package/form/Repositories.h \
    composer/package/form/delegate/RepositoryRow.h \
    composer/package/form/delegate/editor/Repository.h \
    composer/package/form/Scripts.h \
    composer/package/form/Autoload.h \
    composer/package/form/visitor/DataExtractVisitor.h \
    composer/package/form/delegate/ScriptRow.h \
    composer/package/form/delegate/editor/Script.h \
    composer/package/form/delegate/AutoloadRow.h \
    composer/package/form/delegate/editor/Autoloader.h \
    composer/package/form/controls/Standard.h \
    composer/package/form/controls/Context.h \
    composer/package/form/Dependencies.h \
    composer/package/form/delegate/DependencyRow.h \
    composer/package/form/delegate/editor/Dependency.h \
    composer/api/packagist/Packagist.h \
    composer/package/Package.h \
    composer/package/layout/Search.h \
    concurrent/thread/Thread.h \
    composer/package/form/controls/FixedTree.h \
    composer/package/form/model/FixedTreeDuplicateAware.h \
    main/Window.h \
    paginator/Paginator.h \
    composer/project/explorer/Explorer.h \
    composer/package/form/Form.h \
    composer/package/form/Path.h \
    composer/project/explorer/controls/ProjectExplorerContext.h \
    main/factory/MenuBar.h \
    main/factory/MainWindow.h \
    composer/package/overview/Overview.h \
    composer/package/overview/state/Stub.h \
    composer/package/overview/state/ViewPackage.h \
    settings/Settings.h \
    composer/package/form/delegate/BaseDelegate.h \
    globals.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/images.qrc \
    styles/styles.qrc
