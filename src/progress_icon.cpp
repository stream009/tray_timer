#include "progress_icon.hpp"

#include <cassert>

#include <QtCore/QVariant>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QStyle>

ProgressIcon::
ProgressIcon()
{
    m_bar.setObjectName("progressBar");
    m_bar.setGeometry(0, 0, 16, 22);
    m_bar.setOrientation(Qt::Vertical);
    m_bar.setMinimum(0);
    m_bar.setMaximum(1);
    m_bar.setTextVisible(false);
    m_font = m_bar.font();
}

ProgressIcon::
operator QIcon()
{
    auto pixmap = QPixmap::grabWidget(&m_bar);

    const auto current = m_bar.value();
    if (current < 60) {
        drawSeconds(pixmap, current);
    }

    return QIcon { pixmap };
}

QColor ProgressIcon::
color() const
{
    return m_bar.palette().color(QPalette::Highlight);
}

void ProgressIcon::
setMaximum(const size_t value)
{
    assert(value > 0);
    m_bar.setMaximum(value);
}

void ProgressIcon::
setCurrent(const size_t value)
{
    assert(value <= static_cast<size_t>(m_bar.maximum()));

    m_bar.setValue(value);
}

void ProgressIcon::
setColor(const QColor &color)
{
    QPalette palette { m_bar.palette() };
    palette.setColor(QPalette::Highlight, color);
    m_bar.setPalette(palette);
}

void ProgressIcon::
setFont(const QFont &font)
{
    m_font = font;
}

void ProgressIcon::
drawSeconds(QPixmap &pixmap, const size_t secs)
{
    QPainter p { &pixmap };
    p.setPen(Qt::white);
    p.setFont(m_font);

    const auto &text = QString { "%1" }.arg(secs);
    const QFontMetrics fm { p.font() };
    const auto width = fm.width(text);
    const auto height = fm.height();

    const auto x = (m_bar.width()  - width)  / 2;
    const auto y = height + (m_bar.height() - height) / 2;

    p.drawText(x, y, text);
}
