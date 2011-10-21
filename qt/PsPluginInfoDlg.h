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
