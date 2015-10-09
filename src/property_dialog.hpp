#ifndef PROPERTY_DIALOG_HPP
#define PROPERTY_DIALOG_HPP

#include <memory>

#include <QtGui/QDialog>

class QDialogButtonBox;
class QPushButton;
class QTabWidget;

class Settings;
class TimerListWidget;
class AppearanceSettingWidget;

class PropertyDialog : public QDialog
{
    Q_OBJECT
public:
    PropertyDialog(Settings&);
    ~PropertyDialog();

private:
    QTabWidget &createTabWidget();
    void createButtonBox();
    QPushButton &okButton();

private Q_SLOTS:
    void onPropertyChanged();

private:
    Settings &m_settings;
    std::unique_ptr<TimerListWidget> m_timer;
    std::unique_ptr<AppearanceSettingWidget> m_appearance;
    std::unique_ptr<QDialogButtonBox> m_buttonBox;
};

#endif // PROPERTY_DIALOG_HPP
