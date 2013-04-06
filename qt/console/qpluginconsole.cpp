/***************************************************************************
                          qconsole.cpp  -  description
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

#include "qpluginconsole.h"
// TODO: For testing:
#include "Poco/Util/Application.h"
#include "plugins/PluginManager.h"

#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>

#include <iostream>

// Clear the console
void QPluginConsole::clear()
{
   QTextEdit::clear();
}

// Reset the console
void QPluginConsole::reset()
{
    clear();
    //set the style of the QTextEdit
    setCurrentFont(QFont("Courier"));
    // init attributes
    historyIndex = 0;
    history.clear();
}

void QPluginConsole::print(std::string output)
{
    insertPlainText(QString(output.c_str()));
}

// QPluginConsole constructor (init the QTextEdit & the attributes)
QPluginConsole::QPluginConsole(QWidget *parent, const char *name, bool initInterceptor) 
: QTextEdit(name, parent),
  cmdColor(Qt::black), errColor(Qt::red), outColor(Qt::blue),
  completionColor(Qt::green), stdoutInterceptor(NULL), stderrInterceptor(NULL),
  multilineCommand(""), scriptEngine(NULL)
{
    // don't provide a context menu
    setContextMenuPolicy(Qt::NoContextMenu);

    // resets the console
    reset();

    if (initInterceptor)
    {
	    // Initialize the interceptors
	    stdoutInterceptor = new Interceptor(this);
	    stdoutInterceptor->initialize(1);
	    connect(stdoutInterceptor, SIGNAL(received(QTextStream *)), SLOT(displayPrompt()));

	    stderrInterceptor = new Interceptor(this);
	    stderrInterceptor->initialize(2);
	    connect(stderrInterceptor, SIGNAL(received(QTextStream *)), SLOT(displayPrompt()));
    }

    connect(this, SIGNAL(cursorPositionChanged()), SLOT(handleCursorPositionChanged()));

    // TODO: This is only for testing purposes, the shell to be used should be
    // set somewhere else in the future
    PluginManager& plugin_manager = Poco::Util::Application::instance().getSubsystem<PluginManager>();
    scriptEngine = plugin_manager.createScriptEngine("PythonShell");
    scriptEngine->init(this);

    // initialize according to pointer to script engine
    if (scriptEngine!=NULL) {
        setPrompt(scriptEngine->getNormalPrompt().c_str());
    }
}

// Sets the prompt and cache the prompt length to optimize the processing speed
void QPluginConsole::setPrompt(QString newPrompt, bool display)
{
    prompt = newPrompt;
    promptLength = prompt.length();
    // display the new prompt
    if (display)
    {
        displayPrompt();
    }
}

// Displays the prompt and move the cursor to the end of the line.
void QPluginConsole::displayPrompt()
{
    QTextCursor cursor = textCursor();

    // flush the stdout/stderr before displaying the prompt
    if (stdoutInterceptor)
    {
        setTextColor(outColor);
        stdReceived(stdoutInterceptor->textIStream());
    }
    if (stderrInterceptor)
    {
        setTextColor(errColor);
        stdReceived(stderrInterceptor->textIStream());
    }

    //displays the prompt
    setTextColor(cmdColor);
    if (cursor.columnNumber()>0)
    {
        append(prompt);
    } else
    {
        insertPlainText(prompt);
    }
    QTextEdit::moveCursor(QTextCursor::EndOfLine);
    // Saves the position index of the prompt
    promptParagraph = cursor.position();
}

// displays redirected stdout/stderr
void QPluginConsole::stdReceived(QTextStream *s)
{
    QString line;
    line = s->readAll();
    append(line);
}

// Don't remove any text if the selection is not inside the edition zone
void QPluginConsole::removeSelectedText(int selNum)
{
#if 0
    //get where the current selection starts and where it ends
    int paraFrom, indexFrom, paraTo, indexTo;
    getSelection( &paraFrom, &indexFrom, &paraTo, &indexTo, selNum);
    //check if the selection is inside the selection zone and then delete it
    //else unselect
    if (isInEditionZone() && ((paraFrom > promptParagraph) ||
        ( (paraFrom == promptParagraph) && (indexFrom >= promptLength) )) )
        QTextEdit::removeSelectedText(selNum);
    else
        selectAll(false);
#endif
}

// give suggestions to autocomplete a command (should be reimplemented)
// the return value of the function is the string list of all suggestions
QStringList QPluginConsole::autocompleteCommand(QString)
{
    return QStringList();
}
  
// Reimplemented key press event
void QPluginConsole::keyPressEvent( QKeyEvent *e )
{
    QTextCursor cursor = textCursor();
    QTextBlock  block  = cursor.block();
    int         index  = cursor.position();

    // Don't delete the prompt if backspace is pressed
    if (e->key() == Qt::Key_Backspace)
    {
        if (e->modifiers() == Qt::ControlModifier)
        {
            // Control-Backspace deleting complete words
            // trick to get the new position of the cursor
            cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor, 1);
            // exit if the new position is out of the edition zone (we can't use
            // the editionZone function here because that would check the
            // current cursor, but we only modified a local cursor
            bool error = false;
            if (cursor.position()<block.position()+promptLength)
            {
                error = true;
            }
            cursor.setPosition(index, QTextCursor::KeepAnchor);
            if (error)
            {
                e->accept();
                return;
            }
        } else
        {
            // Simple backspace deleting single characters
            if (cursor.position()<=block.position()+promptLength)
            {
                // Don't backspace beyond the prompt
                e->accept();
                return;
            }
        }
    }
    // If return pressed, do the evaluation and append the result
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        e->accept();
        // Get the command to validate
        QString command = getCurrentCommand();
        // execute the command and get back its text result and its return value
        append("");
        execCommand(command);
        return;
    }

    if (e->key() == Qt::Key_Home)
    {
        if (!isInEditionZone())
        {
            QTextEdit::keyPressEvent( e );
            return;
        }
        QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
        if (e->modifiers()==Qt::ShiftModifier)
            mode = QTextCursor::KeepAnchor;

        cursor.setPosition(block.position()+promptLength, mode);
        setTextCursor(cursor);
        e->accept();
        return;
    }

    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        if (isInEditionZone() && history.size())
        {
            if ( (e->key() == Qt::Key_Down) && (historyIndex + 1 < history.size()) )
                historyIndex ++;
            else if ((e->key() == Qt::Key_Up) && historyIndex)
                historyIndex --;
            // replace the current command with the command found in the history
            replaceCurrentCommand(history[historyIndex]);
            
            e->accept();
            return;
        } else
        {
            QTextEdit::keyPressEvent( e );
        }
    }

    // If Ctrl + C pressed, then undo the current command
    /*if ( (e->key() == Qt::Key_C) && (e->state() == Qt::ControlModifier) )
        displayPrompt();*/
    // Treat the tab key & autocomplete the current command
    else if (e->key() == Qt::Key_Tab)
    {
        QString command = getCurrentCommand();
        QStringList sl = autocompleteCommand(command);
        QString str = sl.join(" ");
        if (sl.count() == 1)
            replaceCurrentCommand(sl[0] + " ");
        else if (sl.count() > 1)
        {
            setTextColor(completionColor);
            append(sl.join(" "));
            setTextColor(cmdColor);
            displayPrompt();
            QTextCursor cursor = textCursor();
            cursor.insertText(command);
        }
    }
    else
    {
        QTextEdit::keyPressEvent( e );
    }
}

void QPluginConsole::focusInEvent(QFocusEvent* event)
{
    if (isInEditionZone())
    {
        setReadOnly(false);
    } else
    {
        setReadOnly(true);
        Qt::TextInteractionFlags flags = Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard;
        setTextInteractionFlags(flags);
    }
}

// Get the current command
QString QPluginConsole::getCurrentCommand()
{
    // First go to the end of the line
    QTextEdit::moveCursor(QTextCursor::EndOfLine);
    QTextCursor cursor = textCursor();
    // Get the current command: we just remove the prompt
    cursor.setPosition(cursor.block().position()+promptLength, 
                       QTextCursor::KeepAnchor);
    QString command = cursor.selectedText();
    cursor.clearSelection();
    return command;
}

// Replace current command with a new one
void QPluginConsole::replaceCurrentCommand(QString newCommand)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.block().position()+promptLength,
                       QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.insertText(newCommand);
}

// default implementation: command always complete
bool QPluginConsole::isCommandComplete(QString )
{
    return true;
}

// Tests whether the cursor is in th edition zone or not (after the prompt
// or in the next lines (in case of multi-line mode)
bool QPluginConsole::isInEditionZone()
{
    int index;
    QTextCursor cursor = textCursor();
    index = cursor.position();
    return (index >= promptParagraph);
}

// Basically, puts the command into the history list
void QPluginConsole::interpretCommand(QString command)
{
    if (multilineCommand.length()!=0)
    {
        multilineCommand.append("\n");
    }
    multilineCommand.append(command);

    if (scriptEngine!=0)
    {
        ScriptEngine::ExecutionResult result = scriptEngine->executeCommand(multilineCommand.toStdString());
        if (result == ScriptEngine::eER_SUCCESS)
        {
            multilineCommand = "";
            setPrompt(scriptEngine->getNormalPrompt().c_str(), false);
        } else if (result == ScriptEngine::eER_MORE_INPUT_REQUIRED)
        {
            setPrompt(scriptEngine->getMultilinePrompt().c_str(), false);
        } else
        {
            // TODO: command execution failed
            multilineCommand = "";
            setPrompt(scriptEngine->getNormalPrompt().c_str(), false);
        }
    } else
    {
        multilineCommand = "";
    }
    // update the history and its index
    history.append(command.replace("\n", "\\n"));
    historyIndex = history.size();
}

// execCommand(QString) executes the command and displays back its result
void QPluginConsole::execCommand(QString command, bool showPrompt)
{
    // execute the command and get back its text result and its return value
    insertPlainText("");
    interpretCommand(command);
    // TODO: According to the return value, display the result either in red or in blue
    setTextColor(outColor);
    // Display the prompt again
    if (showPrompt)
        displayPrompt();
}

void QPluginConsole::handleCursorPositionChanged()
{
    if (isInEditionZone())
    {
        setReadOnly(false);
    } else
    {
        setReadOnly(true);
        Qt::TextInteractionFlags flags = Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard;
        setTextInteractionFlags(flags);
    }
}

// Allows pasting with middle mouse button (x window)
// when clicking outside of the edition zone
void QPluginConsole::paste()
{
#if 0
    setCursorPosition(oldIndex );
    QTextEdit::paste();
#endif
}

#include "qpluginconsole.moc"

