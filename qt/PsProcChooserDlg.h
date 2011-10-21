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
