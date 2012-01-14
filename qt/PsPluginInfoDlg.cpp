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

#include "PsPluginInfoDlg.h"
#include "plugins/PluginManager.h"
#include <QtGui>
#include <iostream>

/* constants to describe entry types in the plugin tree view */
const int CATEGORY_ITEM = 1;
const int PLUGIN_ITEM   = 2;

Q_DECLARE_METATYPE(PluginInformationBlock)

PsPluginInfoDlg::PsPluginInfoDlg(QWidget* parent)
: QDialog(parent)
{
     QDialogButtonBox* okButton = 
        new QDialogButtonBox(QDialogButtonBox::Ok);
    mDescription =
        new QTextEdit();
    mPluginTree =
        new QTreeWidget();

    QVBoxLayout *layout = new QVBoxLayout;
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(mPluginTree);
    splitter->addWidget(mDescription);
    layout->addWidget(splitter);
    layout->addWidget(okButton);
    setLayout(layout);

    mPluginTree->header()->setVisible(false);

    connect(mPluginTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), 
            this, SLOT(handleSelectPlugin(QTreeWidgetItem*,int)));
    connect(okButton, SIGNAL(accepted()), 
            this, SLOT(accept()));

    mDescription->setReadOnly(true);

    setWindowTitle(tr("Plugin information"));

    fillPluginList();
    mPluginTree->expandAll();
}

void PsPluginInfoDlg::fillPluginList()
{
    PluginManager* pm = PluginManager::getInstance();
    for (std::vector<PluginInformationBlock>::iterator iter = pm->pluginsBegin();
         iter != pm->pluginsEnd();
         ++iter)
    {
        /* For each plugin check if its category is already available as a root
         * item in the TreeWidget */
        QTreeWidgetItem* category_item;
        QList<QTreeWidgetItem*> findings = mPluginTree->findItems(
            QString(iter->category.c_str()),
            Qt::MatchExactly
        );
        if (findings.empty())
        {
            /* The category is not already in the TreeWidget, so we add it now */
            QStringList category_strings;
            category_strings << QString(iter->category.c_str());
            category_item = new QTreeWidgetItem(category_strings, CATEGORY_ITEM);
            mPluginTree->addTopLevelItem(category_item);
        } else
        {
            /* The category is in the TreeWidget, so use the found item */
            category_item = findings[0];
        }
        QStringList plugin_strings;
        plugin_strings << QString(iter->name.c_str());
        QTreeWidgetItem* plugin_item = new QTreeWidgetItem(category_item, plugin_strings, PLUGIN_ITEM);
        QVariant data = qVariantFromValue(*iter);
        plugin_item->setData(1, Qt::UserRole, data);
    }
}

void PsPluginInfoDlg::handleSelectPlugin(QTreeWidgetItem* item, int column)
{
    if (item->type() == PLUGIN_ITEM)
    {
        const QVariant data = item->data(1, Qt::UserRole);
        PluginInformationBlock info = data.value<PluginInformationBlock>();
        this->fillDescription(info);
    } else
    {
        this->mDescription->clear();
    }
}

void PsPluginInfoDlg::fillDescription(PluginInformationBlock info)
{
    mDescription->clear();
    mDescription->insertHtml(QString(
        "<h1>%1</h1>"
        "<dl><dt>Version</dt><dd>%2.%3.%4 build %5</dd>"
        "<dt>Author</dt><dd>%6</dd>"
        "<dt>Support</dt><dd>%7</dd>"
        "<dt>Description</dt><dd>%8</dd></dl")
        .arg(info.name.c_str())
        .arg(info.major).arg(info.minor).arg(info.patch).arg(info.build)
        .arg(info.author.c_str())
        .arg(info.support.c_str())
        .arg(info.description.c_str())
    );
}

#include "PsPluginInfoDlg.moc"
