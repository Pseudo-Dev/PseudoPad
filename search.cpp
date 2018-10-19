#include "search.h"
#include "ui_search.h"
#include "notepad.h"
#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QMessageBox>
Search::Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);

}

Search::~Search()
{
    delete ui;
}
void Search::on_pushButton_clicked()
{
    int pos = 0;
    QString searchString;
    searchString = ui->lineEdit->text(); //get the string to be searched.
    QTextCursor Cursor = edit_area->textCursor(); //Make a QTextCursor object and pass the QTextEdit to do operations on the QTextEdit
    //If forwards checking is selected.
    if (ui->radioButton->isChecked())
    {
        j = Cursor.position()+1;
        if (ui->checkBox->isChecked())
        {
            pos = word.indexOf(searchString, j ,Qt::CaseSensitive);
        }
        else
        {
            pos = word.indexOf(searchString, j ,Qt::CaseInsensitive);
        }
    }

    //if you want to check the other direction?
    else if (ui->radioButton_2->isChecked())
    {
        j = Cursor.position()-searchString.length()-1;
        if (ui->checkBox->isChecked())
        {
            pos = word.lastIndexOf(searchString, j ,Qt::CaseSensitive);
        }
        else
        {
            pos = word.lastIndexOf(searchString, j ,Qt::CaseInsensitive);
        }
    }

    //if nothing is checked, the it goes front by default.
    else
    {
        j = Cursor.position()+1;
        if (ui->checkBox->isChecked())
        {
            pos = word.indexOf(searchString, j ,Qt::CaseSensitive);
        }
        else
        {
            pos = word.indexOf(searchString, j ,Qt::CaseInsensitive);
        }
    }


    int position = pos; // store the pos in position


    if(word.contains(searchString, Qt::CaseInsensitive)) // CHecks if the word exists in the  in the document.
    {
         Cursor.setPosition(position, QTextCursor::MoveAnchor); //Moves the cursor to the position of the word.
         if (position != -1) //If the word is found, select the word.
         {
             for (int i = 0;  i < searchString.length(); i++)
             Cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
             edit_area->setTextCursor(Cursor);
         }
         else if(pos == -1 || ui->radioButton->isChecked()) //Otherwise ask to go the other direction?.
         {
             QMessageBox::information(this, "Info", "Word Not found.\n(Maybe try going the other direction?)");
         }
    }
    else //If the document doesnt have the word
    {
         QMessageBox::information(this, "Info","Word Not Found!");
    }

}
void Search::on_pushButton_2_clicked()
{
    //replacing the text.
    QString replaceString = ui->lineEdit_2->text();
    QTextCursor Cursor = edit_area->textCursor();
    Cursor.removeSelectedText();
    Cursor.insertText(replaceString);
}
void Search::closeEvent(QCloseEvent *event)
{
    delete this;
}
