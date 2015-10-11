#ifndef PROGRESS_ICON_HPP
#define PROGRESS_ICON_HPP

#include <QtGui/QProgressBar>

class QColor;
class QFont;
class QPixmap;

class ProgressIcon
{
public:
    ProgressIcon();

    // query
    operator QIcon();
    QColor color() const;

    // modifier
    void setMaximum(const size_t value);
    void setCurrent(const size_t value);
    void setColor(const QColor&);
    void setFont(const QFont&);

private:
    void drawSeconds(QPixmap&, const size_t secs);

private:
    QProgressBar m_bar;
    QFont m_font;
};

#endif // PROGRESS_ICON_HPP
