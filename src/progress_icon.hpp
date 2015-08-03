#ifndef PROGRESS_ICON_HPP
#define PROGRESS_ICON_HPP

#include <QtGui/QProgressBar>

class ProgressIcon
{
public:
    ProgressIcon();

    operator QIcon();

    void setMaximum(const size_t value);
    void setCurrent(const size_t value);
    void setSuspended(const bool);

private:
    void drawSeconds(QPixmap&, const size_t secs);
    void changeBarColor();
    void polishBar();

private:
    QProgressBar m_bar;
};

#endif // PROGRESS_ICON_HPP
