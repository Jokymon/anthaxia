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

#include "PsProcChooserDlg.h"
#include <QtGui>
#include <iostream>

PsProcChooserDlg::PsProcChooserDlg(QWidget* parent, QStringList processorList)
: QDialog(parent)
, mSelectedProcessor(QString::null)
{
    QPushButton* cancelButton =
        new QPushButton(tr("&Cancel"));
    QPushButton* loadButton =
        new QPushButton(tr("&Load"));
    QListWidget* list =
        new QListWidget();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(loadButton, 1, 0);
    layout->addWidget(cancelButton, 1, 1);
    layout->addWidget(list, 0, 0, 1, 2);
    setLayout(layout);

    connect(list, SIGNAL(currentTextChanged(const QString&)), 
            this, SLOT(handleSelectProcessor(const QString&)));
    connect(cancelButton, SIGNAL(clicked()), 
            this, SLOT(reject()));
    connect(loadButton, SIGNAL(clicked()), 
            this, SLOT(accept()));

    list->addItems(processorList);
    list->setCurrentRow(0);

    setWindowTitle(tr("Load Processor Model Dialog"));
}

QString PsProcChooserDlg::selectedProcessor() const
{
    return mSelectedProcessor;
}

void PsProcChooserDlg::handleSelectProcessor(const QString& text)
{
    mSelectedProcessor = text;
}

#include "PsProcChooserDlg.moc"
