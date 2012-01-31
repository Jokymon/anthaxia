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

#ifndef _PSPLUGININFODLG_H
#define _PSPLUGININFODLG_H

#include "plugins/Plugin.h"
#include <QDialog>
#include <QStringList>

class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QWidget;

class PsPluginInfoDlg : public QDialog
{
Q_OBJECT

public:
    PsPluginInfoDlg(QWidget* parent =0);
private:
    QTreeWidget* mPluginTree;
    QTextEdit* mDescription;
    void fillPluginList();
    void fillDescription(PluginInformationBlock info);
private slots:
    void handleSelectPlugin(QTreeWidgetItem* item, int column);
};

#endif
