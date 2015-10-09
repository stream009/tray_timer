#ifndef ACTION_BUTTON_HPP
#define ACTION_BUTTON_HPP

#include <QtGui/QPushButton>

class QAction;

class ActionButton : public QPushButton
{
    Q_OBJECT
public:
    using QPushButton::QPushButton;

    void setAction(QAction&);

private Q_SLOTS:
    void onActionChanged();

private:
    QAction *m_action = nullptr;
};

#endif // ACTION_BUTTON_HPP
