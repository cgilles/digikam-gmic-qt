/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC Generic plugin settings dialog
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QString>
#include <QList>
#include <QDialog>
#include <QUrl>

// digiKam includes

#include "dplugin.h"
#include "dinfointerface.h"

using namespace Digikam;

namespace DigikamGenericGmicQtPlugin
{

class GmicQtSettingsDlg : public QDialog
{
    Q_OBJECT

public:

    /**
     * G'MIC filter output image format.
     */
    enum
    {
        GMIC_JPEG = 0,
        GMIC_PNG,
        GMIC_TIFF
    };

public:

    explicit GmicQtSettingsDlg(DPlugin* const tool,
                              DInfoInterface* const iface,
                              QWidget* const parent = nullptr);
    ~GmicQtSettingsDlg()           override;

    QList<QUrl> imageUrls()  const;
    QString outputTemplate() const;
    int     outputFormat()   const;

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
