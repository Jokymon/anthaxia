/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef _PSPROCCHOOSERDLG_H
#define _PSPROCCHOOSERDLG_H

#include <QDialog>
#include <QStringList>

class QListWidgetItem;
class QWidget;

class PsProcChooserDlg : public QDialog
{
Q_OBJECT

public:
    PsProcChooserDlg(QWidget* parent =0, 
                     QStringList processorList =QStringList());
    QString selectedProcessor() const;
private:
    QString mSelectedProcessor;
private slots:
    void handleSelectProcessor(const QString& text);
};

#endif
