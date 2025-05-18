/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC processor progress dialog
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtprocessordlg.h"

// Qt includes

#include <QPushButton>
#include <QProgressBar>
#include <QEventLoop>
#include <QLabel>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QGridLayout>
#include <QDialogButtonBox>

// digiKam includes

#include "dimg.h"
#include "digikam_debug.h"
#include "ditemtooltip.h"
#include "dhistoryview.h"
#include "dimgloaderobserver.h"

// Local includes

#include "gmicqtcommon.h"

namespace DigikamGenericGmicQtPlugin
{

class GmicQtProcessorThreadObserver;

class Q_DECL_HIDDEN GmicQtProcessorThread::Private
{

public:

    Private() = default;

public:

    bool                            cancel      = false;
    GmicQtProcessor*                proc        = nullptr;
    GmicQtProcessorThreadObserver*  observer    = nullptr;
    QStringList                     inputPaths;
    QString                         command;
    QString                         outputPath;
    QString                         outputFormat;
};

// ---

class Q_DECL_HIDDEN GmicQtProcessorThreadObserver : public DImgLoaderObserver
{
public:

    explicit GmicQtProcessorThreadObserver(GmicQtProcessorThread::Private* const priv)
        : DImgLoaderObserver(),
          d                 (priv)
    {
    }

    ~GmicQtProcessorThreadObserver() override = default;

    bool continueQuery() override
    {
        return (!d->cancel);
    }

private:

    GmicQtProcessorThread::Private* const d = nullptr;
};

// ---

GmicQtProcessorThread::GmicQtProcessorThread(QObject* const parent)
    : QThread(parent),
      d      (new Private)
{
    d->observer = new GmicQtProcessorThreadObserver(d);
}

GmicQtProcessorThread::~GmicQtProcessorThread()
{
    wait();

    delete d->observer;
    delete d;
}

void GmicQtProcessorThread::cancel()
{
    d->proc->cancel();
    d->cancel = true;
}

void GmicQtProcessorThread::setSettings(const QStringList& inputPaths,
                                        const QString& command,
                                        const QString& outputPath,
                                        const QString& outputFormat)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC command      :" << command;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Images to Process  :" << inputPaths;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Ouput image file   :" << outputPath;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Ouput image format :" << outputFormat;

    d->inputPaths   = inputPaths;
    d->command      = command;
    d->outputPath   = outputPath;
    d->outputFormat = outputFormat;
}

void GmicQtProcessorThread::run()
{
    QString error;

    d->proc = new GmicQtProcessor();

    connect(d->proc, SIGNAL(signalProgressInfo(QString)),
            this, SIGNAL(signalProgressInfo(QString)));

    d->proc->setInputFiles(d->inputPaths);

    if (!d->proc->setProcessingCommand(d->command))
    {
        error = tr("Cannot setup G'MIC filter!");
    }
    else
    {
        QEventLoop loop;

        connect(d->proc, SIGNAL(signalDone(QString)),
                &loop, SLOT(quit()));

        d->proc->startProcessingFiles();

        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: started G'MIC filter...";

        loop.exec();

        if (d->proc->processingComplete())
        {
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC filter completed";

            Q_EMIT signalProgressInfo(tr("Save data into %1").arg(d->outputPath));

            if (d->proc->outputImage().save(d->outputPath, d->outputFormat, d->observer))
            {
                qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC save data completed";
            }
            else
            {
                error = tr("Cannot save G'MIC filter data!");
            }
        }
        else
        {
            error = tr("Cannot process G'MIC filter!");
        }
    }

    delete d->proc;

    Q_EMIT signalComplete(error);
}

// ----------------------------------------------------------------------------------

class Q_DECL_HIDDEN GmicQtProcessorDlg::Private
{
public:

    Private() = default;

public:

    GmicQtProcessorThread* thread  = nullptr;
    DHistoryView*          history = nullptr;
    QDialogButtonBox*      buttons = nullptr;
    QProgressBar*          pbar    = nullptr;
};

GmicQtProcessorDlg::GmicQtProcessorDlg(QWidget* const parent)
    : QDialog(parent, Qt::FramelessWindowHint),
      d      (new Private)
{
    d->buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Close, this);
    d->buttons->button(QDialogButtonBox::Close)->setVisible(false);

    QWidget* const page     = new QWidget(this);
    QGridLayout* const grid = new QGridLayout(page);

    // --------------------------------------------------------

    QLabel* const logo      = new QLabel(page);
    logo->setPixmap(s_gmicQtPluginIcon().pixmap(48, 48));

    // --------------------------------------------------------

    QLabel* const header    = new QLabel(page);
    header->setWordWrap(true);
    header->setText(tr("Processing G'MIC Filter"));

    // --------------------------------------------------------

    d->history              = new DHistoryView(page);
    d->pbar                 = new QProgressBar(page);
    d->pbar->setMinimum(0);
    d->pbar->setMaximum(0);
    d->pbar->setValue(0);

    // --------------------------------------------------------

    grid->addWidget(logo,        0, 0, 1, 1);
    grid->addWidget(header,      0, 1, 1, 1);
    grid->addWidget(d->history,  1, 0, 1, 2);
    grid->addWidget(d->pbar,     2, 0, 1, 2);
    grid->setColumnStretch(1, 10);
    grid->setRowStretch(1, 10);
    grid->setContentsMargins(QMargins());
    grid->setSpacing(layoutSpacing());

    QVBoxLayout* const vbx = new QVBoxLayout(this);
    vbx->addWidget(page);
    vbx->addWidget(d->buttons);
    setLayout(vbx);

    resize(600, 400);

    // --------------------------------------------------------

    connect(d->buttons->button(QDialogButtonBox::Close), SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(d->buttons->button(QDialogButtonBox::Help), SIGNAL(clicked()),
            this, SLOT(slotHelp()));

    d->thread = new GmicQtProcessorThread(this);

    connect(d->thread, SIGNAL(signalComplete(QString)),
            this, SLOT(slotComplete(QString)));

    connect(d->thread, SIGNAL(signalProgressInfo(QString)),
            this, SLOT(slotProgressInfo(QString)));
}

GmicQtProcessorDlg::~GmicQtProcessorDlg()
{
    delete d;
}

void GmicQtProcessorDlg::setSettings(const QStringList& inputPaths,
                                     const QString& command,
                                     const QString& outputPath,
                                     const QString& outputFormat)
{
    d->thread->setSettings(inputPaths, command, outputPath, outputFormat);
    d->thread->start();
}

void GmicQtProcessorDlg::slotProgressInfo(const QString& info)
{
    d->history->addEntry(info, DHistoryView::ProgressEntry);
}

void GmicQtProcessorDlg::slotComplete(const QString& error)
{
    d->buttons->button(QDialogButtonBox::Close)->setVisible(true);
    d->pbar->setMaximum(1);
    d->pbar->setValue(1);

    if (error.isEmpty())
    {
        d->history->addEntry(tr("G'MIC filter is done"), DHistoryView::SuccessEntry);
    }
    else
    {
        d->history->addEntry(tr("Error while processing G'MIC filter"), DHistoryView::ErrorEntry);
        d->history->addEntry(error, DHistoryView::ErrorEntry);
    }
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessordlg.cpp"
