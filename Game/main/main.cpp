#include "mapwindow.hh"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapWindow w;

    w.setWindowTitle(MAIN_TITLE);
    w.show();

    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            qApp->desktop()->availableGeometry()
        )
    );

    return a.exec();
}
