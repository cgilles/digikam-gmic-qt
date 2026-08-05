/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : digiKam GmicQt tests.
 *               Simulate: ./gmic_qt -a -l -c "fx_montage 0,\"V(H(0,1),H(2,V(3,4)))\",1,0.5,0,0,0,0,0,255,0,0,0,0,0" -o out.jpg in1.jpg in2.jpg in3.jpg
 *
 * SPDX-FileCopyrightText: 2019-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

// Qt includes

#include <QEventLoop>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

// digiKam includes

#include "digikam_debug.h"
#include "dpluginloader.h"
#include "dimg.h"

// Local includes

#include "gmicqtprocessor.h"

namespace DigikamBqmGmicQtPlugin
{

QString     s_imagePath;

} // namespace DigikamBqmGmicQtPlugin

namespace DigikamGenericGmicQtPlugin
{

QList<QUrl> s_urlList;

} // namespace DigikamGenericGmicQtPlugin

using namespace Digikam;
using namespace DigikamGmicQtPluginCommon;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DPluginLoader::instance()->init();

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument(QString::fromLatin1("image paths"),
                                 QLatin1String("Image file paths"),
                                 QString::fromLatin1("[paths]"));
    parser.process(app);

    DImg img;

    if (!parser.positionalArguments().isEmpty())
    {
        QStringList paths = parser.positionalArguments();
        qCDebug(DIGIKAM_TESTS_LOG) << "Images to Process:" << paths;

        GmicQtProcessor* const gmicProcessor = new GmicQtProcessor();
        gmicProcessor->setInputFiles(paths);

        QLatin1String command("fx_montage 0,\"V(H(0,1),H(2,V(3,4)))\",1,0.5,0,0,0,0,0,255,0,0,0,0,0");  // Apply Montage filter.

        if (!gmicProcessor->setProcessingCommand(command))
        {
            delete gmicProcessor;
            qCDebug(DIGIKAM_DPLUGIN_BQM_LOG) << "GmicGenericTool: cannot setup G'MIC filter!";

            return (-1);
        }

        gmicProcessor->startProcessingFiles();

        QEventLoop loop;

        QObject::connect(gmicProcessor, SIGNAL(signalDone(QString)),
                         &loop, SLOT(quit()));

        qCDebug(DIGIKAM_DPLUGIN_BQM_LOG) << "GmicGenericTool: started G'MIC filter...";

        loop.exec();

        bool b = gmicProcessor->processingComplete();

        qCDebug(DIGIKAM_DPLUGIN_BQM_LOG) << "GmicGenericTool: G'MIC filter completed:" << b;

        gmicProcessor->outputImage().save(QLatin1String("out.jpg"), QLatin1String("JPG"));

        delete gmicProcessor;
    }
    else
    {
        qCDebug(DIGIKAM_TESTS_LOG) << "Image paths are missing...";
    }

    return 0;
}
