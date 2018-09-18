#ifndef SEARCH_H
#define SEARCH_H
#include <QDialog>
#include <QTextEdit>
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
    QTextEdit *edit_area;
    explicit Search(QWidget *parent = 0);
    ~Search();


private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::Search *ui;
};
#endif // SEARCH_H
