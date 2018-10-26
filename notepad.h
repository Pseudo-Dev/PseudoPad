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
    Q_OBJECT

public:
    PadArea(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(PadArea *editor) : QWidget(editor) {
        padArea = editor;
    }

    QSize sizeHint() const override {
        return QSize(padArea->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        padArea->lineNumberAreaPaintEvent(event);
    }

private:
    PadArea *padArea;
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


private:
    Ui::Notepad *ui;

protected:
    void closeEvent(QCloseEvent *);

};
#endif // NOTEPAD_H
