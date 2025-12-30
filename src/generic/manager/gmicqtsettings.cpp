/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC settings container
 *
 * SPDX-FileCopyrightText: 2025-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtsettings.h"

// Qt includes

#include <QSettings>

namespace DigikamGenericGmicQtPlugin
{

void GmicQtSettings::readSettings()
{
    QSettings cnf;

    inputImages   = cnf.value(QLatin1String("DigikamGmicQtGeneric_InputImages"),   QVariantList()).value<QList<QUrl> >();
    targetUrl     = cnf.value(QLatin1String("DigikamGmicQtGeneric_TargetUrl"),     QUrl()).toUrl();
    templateFName = cnf.value(QLatin1String("DigikamGmicQtGeneric_TemplateFName"), QLatin1String("gmic_output")).toString();
    format        = cnf.value(QLatin1String("DigikamGmicQtGeneric_Format"),        DImg::JPEG).toInt();
    gmicCommand   = cnf.value(QLatin1String("DigikamGmicQtGeneric_GmicCommand"),   QString()).toString();
}

void GmicQtSettings::writeSettings()
{
    QSettings cnf;

    cnf.setValue(QLatin1String("DigikamGmicQtGeneric_InputImages"),     QVariant::fromValue(inputImages));
    cnf.setValue(QLatin1String("DigikamGmicQtGeneric_TargetUrl"),       targetUrl);
    cnf.setValue(QLatin1String("DigikamGmicQtGeneric_TemplateFName"),   templateFName);
    cnf.setValue(QLatin1String("DigikamGmicQtGeneric_Format"),          format);
    cnf.setValue(QLatin1String("DigikamGmicQtGeneric_GmicCommand"),     gmicCommand);
}

QDebug operator<<(QDebug dbg, const GmicQtSettings& t)
{
    QString temp;
    QTextStream stream(&temp);

    stream << "Images to Process    :" << Qt::endl;

    for (const QUrl& url : std::as_const(t.inputImages))
    {
        stream << "    " << url.toLocalFile() << Qt::endl;
    }

    stream << "Output image path    :" << t.targetUrl.toLocalFile()                      << Qt::endl;
    stream << "Output image filename:" << t.templateFName                                << Qt::endl;
    stream << "Output image format  :" << DImg::formatToMimeType((DImg::FORMAT)t.format) << Qt::endl;
    stream << "G'MIC command        :" << t.gmicCommand                                  << Qt::endl;

    dbg.nospace().noquote() << stream.readAll();

    return dbg.space();
}

} // namespace DigikamGenericGmicQtPlugin
