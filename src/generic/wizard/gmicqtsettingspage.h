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

#include <QString>

// Local includes

#include "dwizardpage.h"

using namespace Digikam;

namespace DigikamGenericGmicQtPlugin
{

class GmicQtSettingsPage : public DWizardPage
{
    Q_OBJECT

public:

    explicit GmicQtSettingsPage(QWizard* const dialog, const QString& title);
    ~GmicQtSettingsPage()    override;

    void initializePage()    override;
    bool validatePage()      override;
    bool isComplete() const  override;

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
