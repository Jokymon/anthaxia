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

#include "PsUI.h"
#include "PsMainWindow.h"
#include <QApplication>

PsUI::PsUI()
: QObject()
, mMainWindow(0)
{ }

PsUI::~PsUI()
{ }

void PsUI::parseArgs(int &argc, char **argv)
{
    mApp = new QApplication(argc, argv);
}

void PsUI::install()
{
    mMainWindow = new PsMainWindow();
    mMainWindow->show();
}

void PsUI::loop()
{
    mApp->exec();
}

void PsUI::uninstall()
{
    delete mMainWindow;
    mMainWindow = 0;

    delete mApp;
    mApp = 0;
}

std::string PsUI::getApplicationDirPath()
{
    Q_ASSERT(0!=mApp);
    return mApp->applicationDirPath().toStdString();
}

#include "PsUI.moc"
