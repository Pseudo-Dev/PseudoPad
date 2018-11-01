#ifndef SEARCH_H
#define SEARCH_H
#include <QDialog>
#include <QPlainTextEdit>
#include <QCloseEvent>
#include "highlighter.h"
namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    QString word;
    QString selectedWord;
    int j = 0;
    QPlainTextEdit *edit_area;
    explicit Search(QWidget *parent = 0);
    ~Search();


private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::Search *ui;
protected:
    void closeEvent(QCloseEvent *);
};
#endif // SEARCH_H
