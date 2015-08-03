#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtGui/QApplication>

#include "tray_timer.hpp"

void
loadStyleSheet(QApplication &app)
{
    QFile qss { ":/style_sheet.qss" };
    qss.open(QFile::ReadOnly);
    const auto &styleSheet = qss.readAll();
    app.setStyleSheet(styleSheet);
}

int
main(int argc, char *argv[])
{
    QApplication app { argc, argv };
    app.setOrganizationName("stream");

    loadStyleSheet(app);

    TrayTimer timer { app };

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
