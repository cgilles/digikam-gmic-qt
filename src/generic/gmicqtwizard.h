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

// Local includes

#include "dwizarddlg.h"
#include "dinfointerface.h"

using namespace Digikam;

namespace DigikamGenericGmicQtPlugin
{

class MailWizard : public DWizardDlg
{
    Q_OBJECT

public:

    explicit MailWizard(QWidget* const parent, DInfoInterface* const iface = nullptr);
    ~MailWizard()                    override;

    bool validateCurrentPage()       override;
    int  nextId()              const override;

    DInfoInterface* iface()    const;

    void setItemsList(const QList<QUrl>& urls);

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
