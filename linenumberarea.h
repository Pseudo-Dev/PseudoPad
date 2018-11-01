#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include "search.h"
#include "padarea.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include "highlighter.h"

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
#endif // LINENUMBERAREA_H
