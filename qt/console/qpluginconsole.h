/***************************************************************************
                          qconsole.h  -  description
                             -------------------
    begin                : mar mar 15 2005
    copyright            : (C) 2005 by Houssem BDIOUI
    email                : houssem.bdioui@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QPLUGINCONSOLE_H
#define QPLUGINCONSOLE_H

#include "interceptor.h"
#include <qstringlist.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include "plugins/ScriptEngine.h"

/**An abstract Qt console
 *@author Houssem BDIOUI
 */

class QTextStream;
class QFocusEvent;

class QPluginConsole : public QTextEdit, public ScriptShellInterface
{
    Q_OBJECT
public:
    // constructor
    QPluginConsole(QWidget *parent = NULL, const char *name = NULL, bool initInterceptor = false);
    // set the prompt of the console
    void setPrompt(QString prompt, bool display = true);
    // execCommand(QString) executes the command and displays back its result
    void execCommand(QString command, bool showPrompt = true);
    // clear & reset the console (useful sometimes)
    void clear();
    void reset();
    //cosmetic methods !
    void setCmdColor(QColor c) {cmdColor = c;};
    void setErrColor(QColor c) {errColor = c;};
    void setOutColor(QColor c) {outColor = c;};
    void setCompletionColor(QColor c) {completionColor = c;};
    void setFont(QFont f) {setCurrentFont(f);};

    // ScriptShellInterface
    virtual void print(std::string output);

protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void focusInEvent(QFocusEvent* event);

private:
    // Redefined virtual methods
    void paste();
    //Return false if the command is incomplete (e.g. unmatched braces)
    virtual bool isCommandComplete(QString command);
    //Get the command to validate
    QString getCurrentCommand();
    //Replace current command with a new one
    void replaceCurrentCommand(QString newCommand);
    //Test wether the cursor is in the edition zone
    bool isInEditionZone();
    //displays redirected stdout/stderr
    void stdReceived(QTextStream *s);

//protected attributes
protected:
    //colors
    QColor cmdColor, errColor, outColor, completionColor;
    // Old cursor position
    int oldIndex;
    // New cursor position !
    int newIndex;
    // cached prompt length
    int promptLength;
    // The prompt string
    QString prompt;
    // The commands history
    QStringList history;
    // Current history index (needed because afaik QStringList does not have such an index)
    uint historyIndex;
    //Stdout interceptor
    Interceptor *stdoutInterceptor;
    //Stderr interceptor
    Interceptor *stderrInterceptor;
    //Holds the paragraph number of the prompt (useful for multi-line command handling)
    int promptParagraph;

protected:
    //execute a validated command (should be reimplemented and called at the end)
    virtual void interpretCommand(QString command);
    //give suggestions to autocomplete a command (should be reimplemented)
    //the return value of the function is the string list of all suggestions
    virtual QStringList autocompleteCommand(QString cmd);

private:
    // buffer for keeping commands that span multiple lines
    QString multilineCommand;
    // reference to a scriptengine instance
    ScriptEngine* scriptEngine; 

// Redefined virtual slots
private slots:
    //Reimplemented method
    void removeSelectedText(int selNum = 0);
    //displays the prompt
    void displayPrompt();
    void handleCursorPositionChanged();
};

#endif
