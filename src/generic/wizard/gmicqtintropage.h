/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layers mode.
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

class GmicQtIntroPage : public DWizardPage
{
    Q_OBJECT

public:

    explicit GmicQtIntroPage(QWizard* const dialog, const QString& title);
    ~GmicQtIntroPage() override = default;
};

} // namespace DigikamGenericGmicQtPlugin
