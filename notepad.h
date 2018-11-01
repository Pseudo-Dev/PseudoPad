#ifndef NOTEPAD_H
#define NOTEPAD_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include "search.h"
#include "padarea.h"
#include "linenumberarea.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include "highlighter.h"
class LineNumberArea;
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
    PadArea *note;
    QHash<int,PadArea*> pad;
    QHash<int, QString*> saveFlag;
    Highlighter *highlighter;

    int lineNumberAreaWidth();


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

    //void on_actionC_C_triggered();

    //Custom Slots:

    void setName(); //Adds the (*) to mark unsaved document

    int tabRemover(); // Removes Tabs


    void on_actionC_C_triggered();

private:
    Ui::Notepad *ui;

protected:
    void closeEvent(QCloseEvent *);

};
#endif // NOTEPAD_H
