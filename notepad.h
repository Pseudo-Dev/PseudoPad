#ifndef NOTEPAD_H
#define NOTEPAD_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include "search.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTextBlock>
class LineNumberArea;
namespace Ui {
class Notepad;

}
class PadArea : public QPlainTextEdit
{
public:
    QPlainTextEdit *noteArea;
    PadArea()
    {
        noteArea = new QPlainTextEdit;
    }
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void setMargin(int temp)
    {
        QPlainTextEdit::setViewportMargins(temp, 0, 0, 0);
    }
    QWidget *lineNumberArea;
protected:
    void resizeEvent(QResizeEvent *e);

};
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

    void updateLineNumberAreaWidth(int newBlockCount);

    void highlightCurrentLine();

    void updateLineNumberArea(const QRect &, int);


private:
    Ui::Notepad *ui;
    PadArea *pads;

protected:
    void closeEvent(QCloseEvent *);

};
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(PadArea *editor) : QWidget(editor) {
        codeEditor = editor;
    }



protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    PadArea *codeEditor;
};
#endif // NOTEPAD_H
