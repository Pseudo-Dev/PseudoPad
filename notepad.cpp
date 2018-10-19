#include "notepad.h"
#include "ui_notepad.h"
#include "search.h"
#include "ui_search.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QTextCursor>
Notepad::Notepad(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::Notepad)
{
    //Graphic Part
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget); //Make sure that tabWidget occupies the entire parent space.
    ui->tabWidget->setTabShape(QTabWidget::Triangular); //Make the tab look triangular.

    //Create the required Components
    QPlainTextEdit *tempPad = new QPlainTextEdit(); //Insert a textEdit when you open a new tab.
    QString *openedFile = new QString; //For the saveFlag. The saveFlag checks if a file was saved in the current session.

    //Add the tab
    ui->tabWidget->addTab(tempPad, "New Text"); //Add a tab with the textEdit.
    ui->tabWidget->setCurrentWidget(tempPad); //Change focus to the new tab.

    //tab related ops and storing
    currentTab = ui->tabWidget->currentIndex(); //Get the current tab index for operations on the tab.
    tabIndex = ui->tabWidget->currentIndex(); //Get the current index for saving on the QHash table (pad).
    pad.insert(tabIndex,tempPad); //Save the index with the corresponding QPlainTextEdit.
    saveFlag.insert(tabIndex, openedFile); //Save the index with the saveFlag.
    ui->tabWidget->setTabToolTip(currentTab, "Not saved yet");
    setWindowTitle("PseudoPad");
    QObject::connect(tempPad, SIGNAL(textChanged()), this, SLOT(setName()));
}
Notepad::~Notepad()
{
    delete ui;
}
void Notepad::on_actionNew_triggered()
{
    //Create required components
    QPlainTextEdit *tempPad = new QPlainTextEdit();
    QString *openedFile = new QString;

    //add the tabs
    ui->tabWidget->addTab(tempPad, "New Text");
    ui->tabWidget->setCurrentWidget(tempPad);

    currentTab = ui->tabWidget->currentIndex();
    tabIndex = ui->tabWidget->currentIndex();

    //storing the values in the QHash table.
    pad.insert(tabIndex,tempPad);
    ui->tabWidget->setTabToolTip(currentTab, "Not saved Yet");
    QString tabName = "Untitled - PseudoPad";
    setWindowTitle(tabName);
    saveFlag.insert(tabIndex, openedFile);
    QObject::connect(tempPad, SIGNAL(textChanged()), this, SLOT(setName()));
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

    *saveFlag[currentTab] = filename; //save the name in the saveFlag so that it doesnt ask you to save if you choose to save the file because the file already exists. Choose save as for saving again with different name and/or extensions.

    ui->tabWidget->setTabText(currentTab, name);
    setWindowTitle(name + " - PseudoPad");
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);

    QTextStream in(&file); //QTextStream object to reall all contents.
    QString text = in.readAll(); //store the contents in a QString
    QPlainTextEdit *documentArea = pad[currentTab]; //get the QPlainTextEdit associated to the tab we are working on.

    QObject::disconnect(documentArea, SIGNAL(textChanged()),this, SLOT(setName()));

    documentArea->setPlainText(text);

    QObject::connect(documentArea, SIGNAL(textChanged()), this, SLOT(setName()));

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
      QHash<int,QPlainTextEdit*>::iterator i;
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
          ret = QMessageBox::question (this, "Information", "These files are not saved:\n" + name + "\nAre you sure?", QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);
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

