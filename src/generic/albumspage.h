/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layer mode.
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QList>
#include <QUrl>
#include <QString>

// Local includes

#include "dwizardpage.h"

using namespace Digikam;

namespace DigikamGenericSendByMailPlugin
{

class MailAlbumsPage : public DWizardPage
{
    Q_OBJECT

public:

    explicit MailAlbumsPage(QWizard* const dialog, const QString& title);
    ~MailAlbumsPage()       override;

    bool validatePage()     override;
    bool isComplete() const override;

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericSendByMailPlugin
