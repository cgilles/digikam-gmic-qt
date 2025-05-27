/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC settings container
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

    inputImages   = cnf.value(QLatin1String("GmicQtGeneric_InputImages"),   QVariantList()).value<QList<QUrl> >();
    targetUrl     = cnf.value(QLatin1String("GmicQtGeneric_TargetUrl"),     QUrl()).toUrl();
    templateFName = cnf.value(QLatin1String("GmicQtGeneric_TemplateFName"), QLatin1String("gmic_output")).toString();
    format        = cnf.value(QLatin1String("GmicQtGeneric_Format"),        DImg::JPEG).toInt();
}

void GmicQtSettings::writeSettings()
{
    QSettings cnf;

    cnf.setValue(QLatin1String("GmicQtGeneric_InputImages"),     QVariant::fromValue(inputImages));
    cnf.setValue(QLatin1String("GmicQtGeneric_TargetUrl"),       targetUrl);
    cnf.setValue(QLatin1String("GmicQtGeneric_TemplateFName"),   templateFName);
    cnf.setValue(QLatin1String("GmicQtGeneric_Format"),          format);
}

} // namespace DigikamGenericGmicQtPlugin
