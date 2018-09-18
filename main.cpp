#include "notepad.h"
#include "search.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication note(argc, argv);
    Notepad editor;
    editor.show();
    return note.exec();
}
