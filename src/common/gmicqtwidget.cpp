/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : common implementations for the digiKam plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtwidget.h"

// Qt includes

#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QIcon>
#include <QUrl>
#include <QLabel>
#include <QSettings>

// digiKam includes

#include "digikam_debug.h"
#include "digikam_globals.h"

// Local includes

#include "gmicqtcommon.h"
#include "LanguageSettings.h"
#include "Settings.h"
#include "GmicQt.h"
#include "Widgets/InOutPanel.h"
#include "Widgets/ProgressInfoWidget.h"

using namespace GmicQt;

namespace DigikamGmicQtPluginCommon
{

/**
 * Static strings used to backup the host application properties.
 */
QString         s_hostOrg     = QCoreApplication::organizationName();
QString         s_hostDom     = QCoreApplication::organizationDomain();
QString         s_hostName    = QCoreApplication::applicationName();

/**
 * Static strings used to store the plugin application properties.
 */
QString         s_plugName;
QString         s_plugOrg;
QString         s_plugDom;

/**
 * String to host the digiKam plugin module name used to store settings in RC file.
 */
QString         s_dkModule;

/**
 * The current GmicQt widget instance created.
 */
GmicQtWidget*   s_mainWidget  = nullptr;

/**
 * String used to pass the GmicQt widget filter name selected between the GmicQt widget and the GmiQt Window.
 */
QString         s_filterName;

// --------------------------------------------------------------------------------------

class Q_DECL_HIDDEN GmicQtWidget::Private
{
public:

    Private() = default;

public:

    QString*        filterName  = nullptr;      // TODO: redondant with s_filterName?

    QLabel*         filterLbl   = nullptr;      ///< The filter label instance from the GmicQt widget UI.
    QPushButton*    helpBtn     = nullptr;      ///< Help button add to the GmicQt widget.
    QHBoxLayout*    hlay        = nullptr;      ///< Horizontal layout hosting buttons, labels, and progress bar on the bottom.
};

GmicQtWidget::GmicQtWidget(
                           DPlugin* const tool,
                           QString* const filterName,
                           QWidget* const parent
                          )
    : MainWindow(parent),
      d         (new Private)
{
    d->filterName = filterName;
    d->filterLbl  = findChild<QLabel*>(QLatin1String("filterName"));

    if (!d->filterLbl)
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"filterName\" "
                                          "label from plugin dialog!";
    }

    d->hlay = findChild<QHBoxLayout*>(QLatin1String("horizontalLayout"));

    if (d->hlay)
    {
        d->helpBtn = new QPushButton(this);
        s_gmicQtPluginPopulateHelpButton(this, tool, d->helpBtn);
        d->hlay->insertWidget(0, d->helpBtn);

        QLabel* const lbl = findChild<QLabel*>(QLatin1String("messageLabel"));

        if (lbl)
        {
            d->hlay->setStretchFactor(lbl, 5);
        }
        else
        {
            qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"messageLabel\" "
                                              "label from plugin dialog!";
        }

        QLabel* const lbl2 = findChild<QLabel*>(QLatin1String("rightMessageLabel"));

        if (lbl2)
        {
            d->hlay->setStretchFactor(lbl2, 5);
        }
        else
        {
            qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"rightMessageLabel\" "
                                              "label from plugin dialog!";
        }

        ProgressInfoWidget* const pbar = findChild<ProgressInfoWidget*>(QLatin1String("progressInfoWidget"));

        if (pbar)
        {
            d->hlay->setStretchFactor(pbar, 10);
        }
        else
        {
            qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"progressInfoWidget\" "
                                              "label from plugin dialog!";
        }

    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"horizontalLayout\" "
                                          "layout from plugin dialog!";
    }
}

GmicQtWidget::~GmicQtWidget()
{
    delete d;
}

void GmicQtWidget::setHostType(HostType type)
{
    switch (type)
    {
        case BQM:
        {
            s_dkModule = QLatin1String("digikam-bqm-");
            break;
        }

        case ImageEditor:
        {
            s_dkModule = QLatin1String("digikam-editor-");
            break;
        }

        case Showfoto:
        {
            s_dkModule = QLatin1String("showfoto-");
            break;
        }

        case Generic:
        {
            s_dkModule = QLatin1String("digikam-generic-");
            break;
        }

        default:
        {
            break;
        }
    }
}

void GmicQtWidget::hideButtons()
{
    if (!d->hlay)
    {
        return;
    }

    if (d->helpBtn)
    {
        d->helpBtn->setVisible(false);
        d->hlay->removeWidget(d->helpBtn);
    }

    QPushButton* const pbOk     = findChild<QPushButton*>(QLatin1String("pbOk"));

    if (pbOk)
    {
        pbOk->setVisible(false);
        d->hlay->removeWidget(pbOk);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbOk\" "
                                          "button from plugin dialog!";
    }

    QPushButton* const pbFullScreen = findChild<QPushButton*>(QLatin1String("pbFullscreen"));

    if (pbFullScreen)
    {
        pbFullScreen->setVisible(false);
        d->hlay->removeWidget(pbFullScreen);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbFullscreen\" "
                                          "button from plugin dialog!";
    }

    QPushButton* const pbClose = findChild<QPushButton*>(QLatin1String("pbClose"));

    if (pbClose)
    {
        pbClose->setVisible(false);
        d->hlay->removeWidget(pbClose);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbClose\" "
                                          "button from plugin dialog!";
    }

    // ---

    QPushButton* const pbApply  = findChild<QPushButton*>(QLatin1String("pbApply"));

    if (pbApply)
    {
        pbApply->setVisible(false);
        d->hlay->removeWidget(pbApply);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbApply\" "
                                          "button from plugin dialog!";
    }

    // ---

    QPushButton* const pbCancel = findChild<QPushButton*>(QLatin1String("pbCancel"));

    if (pbCancel)
    {
        pbCancel->setVisible(false);
        d->hlay->removeWidget(pbCancel);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbCancel\" "
                                          "button from plugin dialog!";
    }
}

void GmicQtWidget::setFilterSelectionMode()
{
    if (!d->hlay)
    {
        return;
    }

    QPushButton* const pbOk     = findChild<QPushButton*>(QLatin1String("pbOk"));

    if (pbOk)
    {
        pbOk->setText(QObject::tr("Select Filter"));

        disconnect(pbOk, &QPushButton::clicked,
                   static_cast<MainWindow*>(this), &MainWindow::onOkClicked);

        connect(pbOk, &QPushButton::clicked,
                this, &GmicQtWidget::slotOkClicked);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbOk\" "
                                          "button from plugin dialog!";
    }

    // ---

    QPushButton* const pbApply  = findChild<QPushButton*>(QLatin1String("pbApply"));

    if (pbApply)
    {
        pbApply->setVisible(false);
        d->hlay->removeWidget(pbApply);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbApply\" "
                                          "button from plugin dialog!";
    }

    // ---

    QPushButton* const pbCancel = findChild<QPushButton*>(QLatin1String("pbCancel"));

    if (pbCancel)
    {
        pbCancel->setVisible(false);
        d->hlay->removeWidget(pbCancel);
    }
    else
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC-Qt: Cannot found \"pbCancel\" "
                                          "button from plugin dialog!";
    }
}

void GmicQtWidget::copyGmicCommand()
{
    // Copy the current G'MIC command on the clipboard.

    s_mainWidget->onCopyGMICCommand();
}

void GmicQtWidget::slotOkClicked()
{
    // Return current filter name.

    if (d->filterName && d->filterLbl)
    {
        *d->filterName = d->filterLbl->text()
                         .remove(QLatin1String("<b>"))
                         .remove(QLatin1String("</b>"));
    }

    copyGmicCommand();

    close();
}

void GmicQtWidget::saveParameters()
{
    saveSettings();
}

// --- Static methods ---

GmicQtWidget* GmicQtWidget::createWidget(DPlugin* const tool,
                                         HostType type,
                                         const QString& command)
{
    // Code inspired from GmicQt.cpp::run() and host_none.cpp::main()

    Settings::load(GmicQt::UserInterfaceMode::Full);
    LanguageSettings::installTranslators();

    // ---

    std::list<GmicQt::InputMode> disabledInputModes;
    disabledInputModes.push_back(GmicQt::InputMode::NoInput);
//  disabledInputModes.push_back(GmicQt::InputMode::Active);
    disabledInputModes.push_back(GmicQt::InputMode::All);
    disabledInputModes.push_back(GmicQt::InputMode::ActiveAndBelow);
    disabledInputModes.push_back(GmicQt::InputMode::ActiveAndAbove);
    disabledInputModes.push_back(GmicQt::InputMode::AllVisible);
    disabledInputModes.push_back(GmicQt::InputMode::AllInvisible);

    std::list<GmicQt::OutputMode> disabledOutputModes;
//  disabledOutputModes.push_back(GmicQt::OutputMode::InPlace);
    disabledOutputModes.push_back(GmicQt::OutputMode::NewImage);
    disabledOutputModes.push_back(GmicQt::OutputMode::NewLayers);
    disabledOutputModes.push_back(GmicQt::OutputMode::NewActiveLayers);

    for (const GmicQt::InputMode& mode : disabledInputModes)
    {
        GmicQt::InOutPanel::disableInputMode(mode);
    }

    for (const GmicQt::OutputMode& mode : disabledOutputModes)
    {
        GmicQt::InOutPanel::disableOutputMode(mode);
    }

    // ---

    /**
     * We need to backup QApplication instance properties between plugin sessions else we can
     * seen side effects, for example with the settings to host in RC file.
     */

    s_mainWidget = new GmicQtWidget(tool, &s_filterName, qApp->activeWindow());

    if (
        (type == GmicQtWidget::BQM) ||
        (type == GmicQtWidget::Generic)
       )
    {
        s_mainWidget->setFilterSelectionMode();
    }

    s_mainWidget->setHostType(type);

    RunParameters parameters;

    if (!command.isEmpty())
    {
        parameters.command = command.toStdString();
    }
    else
    {
        parameters = lastAppliedFilterRunParameters(GmicQt::ReturnedRunParametersFlag::BeforeFilterExecution);
    }

    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Start G'MIC-Qt dialog with parameters:";
    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Command:"     << QString::fromStdString(parameters.command);
    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Path:"        << QString::fromStdString(parameters.filterPath);
    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Input Mode:"  << (int)parameters.inputMode;
    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Output Mode:" << (int)parameters.outputMode;
    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Filter name:" << QString::fromStdString(parameters.filterName());

    s_mainWidget->setPluginParameters(parameters);

    // We want a non modal dialog here.

#ifdef Q_OS_MACOS

    s_mainWidget->setWindowFlags(Qt::Tool | Qt::Dialog);

#else

    s_mainWidget->setWindowFlags(Qt::Dialog);

#endif

    s_mainWidget->setWindowModality(Qt::ApplicationModal);

    return s_mainWidget;
}

void GmicQtWidget::backupApplicationProperties()
{
    if (s_plugOrg.isEmpty())
    {
        s_plugOrg  = QCoreApplication::organizationName();
    }

    if (s_plugDom.isEmpty())
    {
        s_plugDom  = QCoreApplication::organizationDomain();
    }

    if (s_plugName.isEmpty())
    {
        s_plugName = s_dkModule + QCoreApplication::applicationName();
    }

    QCoreApplication::setOrganizationName(s_plugOrg);
    QCoreApplication::setOrganizationDomain(s_plugDom);
    QCoreApplication::setApplicationName(s_plugName);
}

void GmicQtWidget::restoreApplicationProperties()
{
    QCoreApplication::setOrganizationName(s_hostOrg);
    QCoreApplication::setOrganizationDomain(s_hostDom);
    QCoreApplication::setApplicationName(s_hostName);
}

} // namespace DigikamGmicQtPluginCommon

#include "moc_gmicqtwidget.cpp"
