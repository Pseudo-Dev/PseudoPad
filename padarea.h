#ifndef PADAREA_H
#define PADAREA_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include "search.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTextBlock>
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



#endif // PADAREA_H
