/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layers mode.
 *
 * SPDX-FileCopyrightText: 2025-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QList>
#include <QUrl>

// Local includes

#include "dwizarddlg.h"
#include "dinfointerface.h"
#include "gmicqtsettings.h"

using namespace Digikam;

namespace DigikamGenericGmicQtPlugin
{

class GmicQtWizard : public DWizardDlg
{
    Q_OBJECT

public:

    explicit GmicQtWizard(QWidget* const parent, DInfoInterface* const iface = nullptr);
    ~GmicQtWizard()                    override;

    bool validateCurrentPage()         override;

    DInfoInterface* iface()      const;

    GmicQtSettings* settings()   const;

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
