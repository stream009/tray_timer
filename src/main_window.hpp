#ifndef MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
#define MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4

#include "ui_main_window.h"

#include <QtGui/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* const parent = nullptr);

private:
    Ui::MainWindow m_ui;
};

#endif // MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
