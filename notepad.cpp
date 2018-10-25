#include "notepad.h"
#include "ui_notepad.h"
#include "search.h"
#include "ui_search.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPaintEvent>
#include <QFontDialog>
#include <QTextCursor>
#include <qpainter.h>
#include <QTextBlock>
Notepad::Notepad(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::Notepad)
{
    //Graphic Part
    ui->setupUi(this);
    QLabel *label = new QLabel;
    label->setText("Press on the \"+\" button to create a new tab or press ctrl+N");
    label->setAlignment(Qt::AlignHCenter);
    label->setAlignment(Qt::AlignCenter);
    QFont f( "Bitstream Character", 15);
    label->setFont(f);
    this->setCentralWidget(ui->tabWidget); //Make sure that tabWidget occupies the entire parent space.
    ui->tabWidget->setTabsClosable(1);
    ui->tabWidget->setMovable(1);

    //Add the tab
    ui->tabWidget->setCurrentWidget(label); //Change focus to the new tab.
    ui->tabWidget->addTab(label,"");

    QPushButton *button = new QPushButton();
    QRect rect(1,0,20,20);
    button->setGeometry(rect);
    button->setText("+");
    QTabBar *tab = new QTabBar;
    tab = ui->tabWidget->tabBar();
    tab->setTabButton(currentTab,QTabBar::RightSide, button);
    tab->setMovable(1);

    ui->tabWidget->setStyleSheet("QTabWidget::pane { border: 0; }");

    setWindowTitle("PseudoPad");
    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabRemover())); //tab remover signal
    QObject::connect(button, SIGNAL(clicked()), this ,SLOT(on_actionNew_triggered())); //adding tabs
    //for line numbers


}
int Notepad::tabRemover()
{
    currentTab = ui->tabWidget->currentIndex();
    QString name;
    name = ui->tabWidget->tabText(currentTab);
    int ret;
    if (name.contains("*"))
    {
        ret = QMessageBox::question(this, "Information", "You have not saved this document.\nDo you want to save it?", QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);
        switch (ret)
        {
          case QMessageBox::Save:
            Notepad::on_actionSave_triggered();
            break;
          case QMessageBox::Discard:
            break;
          case QMessageBox::Cancel:
            return 0;
            break;
          default:
            break;
        }
    }
   delete ui->tabWidget->widget(currentTab);
   QObject::disconnect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabRemover())); //so that all the tabs dont get deleted in a chain.
   QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabRemover())); //reconnect to delete tabs feature again.
   return 0;
}
Notepad::~Notepad()
{
    delete ui;
}
void Notepad::on_actionNew_triggered()
{
    //Create required components
    note = new PadArea;
    QString *openedFile = new QString;

    //add the tabs
    ui->tabWidget->addTab(note->noteArea, "Untitled");
    ui->tabWidget->setCurrentWidget(note->noteArea);

    currentTab = ui->tabWidget->currentIndex();
    tabIndex = ui->tabWidget->currentIndex();

    //storing the values in the QHash table.
    pad.insert(tabIndex,note);
    ui->tabWidget->setTabToolTip(currentTab, "Not saved Yet");
    QString tabName = "Untitled - PseudoPad";
    setWindowTitle(tabName);
    saveFlag.insert(tabIndex, openedFile);
    QObject::connect(note->noteArea, SIGNAL(textChanged()), this, SLOT(setName()));
    QObject::connect(note->noteArea, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    QObject::connect(note->noteArea, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    QObject::connect(note->noteArea, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    note->lineNumberArea = new LineNumberArea(note);
    note->update();
}
void PadArea::lineNumberAreaPaintEvent(QPaintEvent *event)
{
qDebug("s2");

    QPainter painter(lineNumberArea);

    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int Notepad::lineNumberAreaWidth()
{
    QPlainTextEdit *tempNote = new QPlainTextEdit;
    currentTab = ui->tabWidget->currentIndex();
    tempNote = pad[currentTab];

    int digits = 1;
    qDebug ("%d",digits);
    int max = qMax(1, tempNote->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 * digits * 4;

    return space;
}



void Notepad::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    PadArea *note = new PadArea;
    currentTab = ui->tabWidget->currentIndex();
    note = pad[currentTab];

    int temp = lineNumberAreaWidth();
    note->setMargin(temp);
}



void Notepad::updateLineNumberArea(const QRect &rect, int dy)
{
    qDebug("q");
    PadArea *note = new PadArea;
    currentTab = ui->tabWidget->currentIndex();
    note->noteArea = pad[currentTab];

    if (dy)
        note->noteArea->scroll(0, dy);
    //else
        //note->noteArea->update(0, rect.y(), note->noteArea->width(), rect.height());

    if (rect.contains(note->noteArea->viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void PadArea::resizeEvent(QResizeEvent *e)
{
    PadArea *note = new PadArea;

    note->resizeEvent(e);

    QRect cr = contentsRect();
    note->setGeometry(QRect(cr.left(), cr.top(), 5, cr.height()));
}



void Notepad::highlightCurrentLine()
{
    PadArea *tempNote = new PadArea;
    currentTab = ui->tabWidget->currentIndex();
    tempNote = pad[currentTab];

    QList<QTextEdit::ExtraSelection> extraSelections;

    /*if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = tempNote->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }*/

    tempNote->setExtraSelections(extraSelections);
}
void Notepad::setName()
{
    currentTab = ui->tabWidget->currentIndex();
    QString tabName = ui->tabWidget->tabText(currentTab);
    if (!tabName.contains("*"))
    tabName.append("  *");
    ui->tabWidget->setTabText(currentTab, tabName);
}

void Notepad::on_actionOpen_triggered()
{
    currentTab = ui->tabWidget->currentIndex(); // get the current index
    QString filename = QFileDialog::getOpenFileName(this, "Open the file"); //open the file
    QFile file(filename); //QFile to read  all contents of files.
    QFileInfo information(file); //QFileInfo to get the file reated information.
    QString name = information.fileName(); // get the name of the file.

    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot Open file\n Reason: " + file.errorString());
        return;
    }
    if (currentTab == 0)
    {
        on_actionNew_triggered();
    }


    *saveFlag[currentTab] = filename; //save the name in the saveFlag so that it doesnt ask you to save if you choose to save the file because the file already exists. Choose save as for saving again with different name and/or extensions.

    ui->tabWidget->setTabText(currentTab, name);
    setWindowTitle(name + " - PseudoPad");
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);

    QTextStream in(&file); //QTextStream object to reall all contents.
    QString text = in.readAll(); //store the contents in a QString

    QPlainTextEdit *documentArea = pad[currentTab]; //get the QPlainTextEdit associated to the tab we are working on.

    QObject::disconnect(documentArea, SIGNAL(textChanged()),this, SLOT(setName())); //if you open, you won't have a "*" in the tab name. so disconnect that signal

    documentArea->setPlainText(text);

    QObject::connect(documentArea, SIGNAL(textChanged()), this, SLOT(setName())); //reconnect the signal for the "*"

    file.close();
}
void Notepad::on_actionSave_triggered()
{
    int flag = 0;
    currentTab = ui->tabWidget->currentIndex();
    QString filename;
    QString *chkSave = saveFlag[currentTab]; // Get the QString of the current tab.
    if (chkSave->isEmpty()) //checks if some name was assigned to it.
    {   //If not assigned and it was empty:
        filename = QFileDialog::getSaveFileName(this, "Save");
        *chkSave = filename;
    }
    else//If assigned, the just save.
        filename = *chkSave;

    QFile file(filename);
    QFileInfo information(file);

    QString name = information.fileName();

    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Warning!", "\n" + file.errorString());
        return;
        flag = 1;
    }

    QTextStream out(&file); //Create the QTextStream to save all contents from the QPlainTextEdit to the file.

    QPlainTextEdit *documentArea = pad[currentTab]; //get the QPlainTextEdit associated to the tab we are working on.

    QString text = documentArea->toPlainText();

    out << text; // Copy the contents.

    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);
    if (flag != 1)
    {
        ui->tabWidget->setTabText(currentTab, name);
        setWindowTitle(name + " - PseudoPad");
    }
    file.close();
}
void Notepad::on_actionSave_As_triggered()
{
    //Save_As functions the same way as Save but doesnt have the saveFlag check.
    QString filename;
    filename = QFileDialog::getSaveFileName(this, "Save");
    //openedFile = filename;
    QFile file(filename);
    QFileInfo information(file);
    QString name = information.fileName();
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Warning", "Cannot Save" + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream out(&file);
    QPlainTextEdit *documentArea = pad[currentTab];
    QString text = documentArea->toPlainText();
    out << text;
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);
    ui->tabWidget->setTabText(currentTab, name);
    setWindowTitle(name + " - PseudoPad");
    file.close();
}

void Notepad::on_actionCopy_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    documentArea->copy();
}

void Notepad::on_actionPaste_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    documentArea->paste();
}

void Notepad::on_actionUndo_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    documentArea->undo();
}

void Notepad::on_actionRedo_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    documentArea->redo();
}

void Notepad::on_actionFont_triggered()
{ //Change font of the selected text.
    currentTab = ui->tabWidget->currentIndex();
    bool fontselect;
    QPlainTextEdit *documentArea = pad[currentTab];
    QFont font = QFontDialog::getFont(&fontselect, this);
    if (fontselect)
    {
        documentArea->setFont(font);
    }
}

void Notepad::on_actionUPPER_triggered() //Change the Case of a string
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    QTextCursor cursor = documentArea->textCursor();
    if (cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toUpper());
    }
}

void Notepad::on_actionlower_triggered() //CHange the Case of a string
{

    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    QTextCursor cursor = documentArea->textCursor();
    if (cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toLower());
    }
}

void Notepad::on_actionSearch_Word_triggered()
{

    currentTab = ui->tabWidget->currentIndex();
    QFile file(*saveFlag[currentTab]);
    QTextStream out(&file);
    QPlainTextEdit *documentArea = pad[currentTab];
    QString worda = documentArea->toPlainText(); //store contents in worda.
    search = new Search;
    out << worda; //copy the contents.
    search->word = worda; //pass the contents of the file to the search file
    search->edit_area = documentArea; //pas the current QPlainTextEdit to the search
    search->show();
}
void Notepad::on_actionCut_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QPlainTextEdit *documentArea = pad[currentTab];
    documentArea->cut();
}
void Notepad::closeEvent(QCloseEvent *event)
{
      int flag = 0;
      QHash<int,PadArea*>::iterator i;
      QString tempName;
      QString name;
      for (i = pad.begin(); i != pad.end(); ++i)
      {
          tempName = ui->tabWidget->tabText(i.key());
          if (tempName.contains("*"))
          {
              flag++;
              name.append(tempName + '\n');
          }
      }
      int ret;
      if (flag != 0)
      {
          ret = QMessageBox::question(this, "Information", "These files are not saved:\n" + name + "\nAre you sure you want to close?", QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);
          switch (ret)
          {
            case QMessageBox::Save:
              Notepad::on_actionSave_triggered();
              break;
            case QMessageBox::Discard:
              break;
            case QMessageBox::Cancel:
              event->ignore();
              break;
            default:
              break;
          }
      }
      else
      {
          this->close();
      }
}


