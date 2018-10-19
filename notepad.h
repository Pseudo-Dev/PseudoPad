#ifndef NOTEPAD_H
#define NOTEPAD_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include "search.h"
#include <QCloseEvent>
#include <QMessageBox>
namespace Ui {
class Notepad;

}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();
    Search *search;
    int tabIndex;
    int currentTab;
    QHash<int,QPlainTextEdit*> pad;
    QHash<int, QString*> saveFlag;
private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionFont_triggered();

    void on_actionUPPER_triggered();

    void on_actionlower_triggered();

    void on_actionSearch_Word_triggered();

    void on_actionCut_triggered();

    void testfun();

private:
    Ui::Notepad *ui;
protected:
    void closeEvent(QCloseEvent *);
};


#endif // NOTEPAD_H
