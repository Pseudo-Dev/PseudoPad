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
    QTextEdit *tempPad = new QTextEdit(); //Insert a textEdit when you open a new tab.
    QString *openedFile = new QString; //For the saveFlag. The saveFlag checks if a file was saved in the current session.

    //Add the tab
    ui->tabWidget->addTab(tempPad, "New Text"); //Add a tab with the textEdit.
    ui->tabWidget->setCurrentWidget(tempPad); //Change focus to the new tab.

    //tab related ops and storing
    currentTab = ui->tabWidget->currentIndex(); //Get the current tab index for operations on the tab.
    tabIndex = ui->tabWidget->currentIndex(); //Get the current index for saving on the QHash table (pad).
    pad.insert(tabIndex,tempPad); //Save the index with the corresponding QTextEdit.
    saveFlag.insert(tabIndex, openedFile); //Save the index with the saveFlag.
    ui->tabWidget->setTabToolTip(currentTab, "Not saved yet");
    setWindowTitle("PseudoPad");
}
Notepad::~Notepad()
{
    delete ui;
}
void Notepad::on_actionNew_triggered()
{
    //Create required components
    QTextEdit *tempPad = new QTextEdit();
    QString *openedFile = new QString;

    //add the tabs
    ui->tabWidget->addTab(tempPad, "New Text");
    ui->tabWidget->setCurrentWidget(tempPad);

    currentTab = ui->tabWidget->currentIndex();
    tabIndex = ui->tabWidget->currentIndex();

    //storing the values in the QHash table.
    pad.insert(tabIndex,tempPad);
    ui->tabWidget->setTabToolTip(currentTab, "Not saved Yet");
    saveFlag.insert(tabIndex, openedFile);
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
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);

    QTextStream in(&file); //QTextStream object to reall all contents.
    QString text = in.readAll(); //store the contents in a QString
    QTextEdit *documentArea = pad[currentTab]; //get the current QTextEdit we are working on using the tabIndex for openning the contents in that textedit.
    documentArea->setText(text);
    file.close();
}
void Notepad::on_actionSave_triggered()
{
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
    ui->tabWidget->setTabText(currentTab, name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Warning!", "No file selected\n" + file.errorString());
        return;
    }

    QTextStream out(&file); //Create the QTextStream to save all contents from the QTextEdit to the file.

    QTextEdit *documentArea = pad[currentTab]; //get the QTextEdit associated to the tab we are working on.
    QString text = documentArea->toPlainText();

    out << text; // Copy the contents.
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);
    file.close();
}
void Notepad::on_actionSave_As_triggered()
{
    //Save_As functions the same way as Save but doesnt have the saveFlag check.
    QString filename;
    filename = QFileDialog::getSaveFileName(this, "Save");
    //openedFile = filename;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Warning", "Cannot Save" + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream out(&file);
    QTextEdit *documentArea = pad[currentTab];
    QString text = documentArea->toPlainText();
    out << text;
    ui->tabWidget->setTabToolTip(currentTab, "Location: "+filename);
    file.close();
}

void Notepad::on_actionCopy_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    documentArea->copy();
}

void Notepad::on_actionPaste_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    documentArea->paste();
}

void Notepad::on_actionUndo_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    documentArea->undo();
}

void Notepad::on_actionRedo_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    documentArea->redo();
}

void Notepad::on_actionFont_triggered()
{ //CHange font of the selected text.
    currentTab = ui->tabWidget->currentIndex();
    bool fontselect;
    QTextEdit *documentArea = pad[currentTab];
    QFont font = QFontDialog::getFont(&fontselect, this);
    if (fontselect)
    {
        documentArea->setFont(font);
    }
}

void Notepad::on_actionUPPER_triggered() //CHange the Case of a string
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    QTextCursor cursor = documentArea->textCursor();
    if (cursor.hasSelection())
    {
        cursor.insertText(cursor.selectedText().toUpper());
    }
}

void Notepad::on_actionlower_triggered() //CHange the Case of a string
{

    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
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
    QTextEdit *documentArea = pad[currentTab];
    QString worda = documentArea->toPlainText(); //store contents in worda.

    out << worda; //copy the contents.
    search.word = worda; //pass the contents of the file to the search file
    search.edit_area = documentArea; //pas the current QTextEdit to the search
    search.show();
}

void Notepad::on_actionCut_triggered()
{
    currentTab = ui->tabWidget->currentIndex();
    QTextEdit *documentArea = pad[currentTab];
    documentArea->cut();
}
