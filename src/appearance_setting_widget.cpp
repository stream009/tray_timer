#include "appearance_setting_widget.hpp"

#include "progress_icon.hpp"
#include "settings.hpp"

#include <cassert>

#include <QtGui/QBoxLayout>
#include <QtGui/QColorDialog>
#include <QtGui/QFont>
#include <QtGui/QFontComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

class ColorPreview : public QWidget
{
public:
    ColorPreview(const size_t seconds, const QColor &color, const QFont &font)
    {
        assert(seconds <= 60);

        this->setMinimumSize({ 24, 24 });
        this->setMaximumSize({ 24, 24 });
        m_icon.setMaximum(60);
        m_icon.setCurrent(seconds);
        m_icon.setColor(color);
        m_icon.setFont(font);
    }

    // query
    QColor color() const { return m_icon.color(); }

    // modifier
    void setColor(const QColor &color) { m_icon.setColor(color); }
    void setFont(const QFont &font)
    {
        m_icon.setFont(font);
        this->update(this->rect());
    }

protected:
    void paintEvent(QPaintEvent* const event) override
    {
        assert(event);

        const auto &icon = static_cast<QIcon>(m_icon);
        QPainter painter { this };

        icon.paint(&painter, event->rect());
    }

private:
    ProgressIcon m_icon;
};

class ColorSelector : public QWidget
{
    Q_OBJECT
public:
    ColorSelector(const QString &title,
            const size_t seconds, const QColor &color, const QFont &font)
        : m_preview { seconds, color, font }
    {
        auto* const layout = new QVBoxLayout { this };
        assert(layout);

        layout->addWidget(&m_preview);
        const auto rc = layout->setAlignment(&m_preview, Qt::AlignHCenter);
        assert(rc); (void)rc;

        auto* const button = new QPushButton { title, this };
        assert(button);
        layout->addWidget(button);

        this->connect(button, SIGNAL(clicked()),
                      this,     SLOT(onButtonClicked()));
    }

    // query
    QColor color() const { return m_preview.color(); }

    // modifier
    void setFont(const QFont &font)
    {
        m_preview.setFont(font);
    }

Q_SIGNALS:
    void changed();

private Q_SLOTS:
    void onButtonClicked()
    {
        const auto &result =
            QColorDialog::getColor(m_preview.color(), this);

        if (result.isValid()) {
            m_preview.setColor(result);
            Q_EMIT changed();
        }
    }

private:
    ColorPreview m_preview;
};


class AppearanceSettingWidget::ColorPanel : public QGroupBox
{
    Q_OBJECT
public:
    ColorPanel(Settings &settings, const QFont &font)
        : m_settings { settings }
    {
        this->setTitle(tr("Color"));

        auto* const layout = new QHBoxLayout { this };
        assert(layout);

        m_normalColor.reset(new ColorSelector {
                         tr("Normal"), 55, m_settings.normalColor(), font });
        assert(m_normalColor);
        layout->addWidget(m_normalColor.get());

        m_suspendedColor.reset(new ColorSelector {
                   tr("Suspended"), 55, m_settings.suspendedColor(), font });
        assert(m_suspendedColor);
        layout->addWidget(m_suspendedColor.get());

        m_lessThan50Color.reset(new ColorSelector {
                      tr("< 50%"), 30, m_settings.lessThan50Color(), font });
        assert(m_lessThan50Color);
        layout->addWidget(m_lessThan50Color.get());

        m_lessThan20Color.reset(new ColorSelector {
                      tr("< 20%"), 12, m_settings.lessThan20Color(), font });
        assert(m_lessThan20Color);
        layout->addWidget(m_lessThan20Color.get());

        connectWidgets();
    }

    // modifier
    void save()
    {
        m_settings.setNormalColor(m_normalColor->color());
        m_settings.setSuspendedColor(m_suspendedColor->color());
        m_settings.setLessThan50Color(m_lessThan50Color->color());
        m_settings.setLessThan20Color(m_lessThan20Color->color());
    }

Q_SIGNALS:
    void changed();

private:
    void connectWidgets()
    {
        this->connect(m_normalColor.get(), SIGNAL(changed()),
                      this,                SIGNAL(changed()));
        this->connect(m_suspendedColor.get(), SIGNAL(changed()),
                      this,                   SIGNAL(changed()));
        this->connect(m_lessThan50Color.get(), SIGNAL(changed()),
                      this,                    SIGNAL(changed()));
        this->connect(m_lessThan20Color.get(), SIGNAL(changed()),
                      this,                    SIGNAL(changed()));
    }

private Q_SLOTS:
    void onFontChanged(const QFont &font)
    {
        m_normalColor->setFont(font);
        m_suspendedColor->setFont(font);
        m_lessThan50Color->setFont(font);
        m_lessThan20Color->setFont(font);
    }

private:
    Settings &m_settings;
    std::unique_ptr<ColorSelector> m_normalColor;
    std::unique_ptr<ColorSelector> m_suspendedColor;
    std::unique_ptr<ColorSelector> m_lessThan50Color;
    std::unique_ptr<ColorSelector> m_lessThan20Color;
};


class AppearanceSettingWidget::FontPanel : public QWidget
{
    Q_OBJECT
public:
    explicit FontPanel(Settings &settings)
        : m_settings { settings }
    {
        setUpWidgets();
        initWidgets();
        connectWidgets();
    }

    // query
    QFont font() const
    {
        QFont result { m_fonts->currentFont() };
        result.setPointSize(m_size->value());

        return result;
    }

    // modifier
    void save()
    {
        const auto &font = m_fonts->currentFont();
        m_settings.setFontName(font.family());
        m_settings.setFontSize(m_size->value());
    }

Q_SIGNALS:
    void changed(const QFont&);

private:
    void setUpWidgets()
    {
        auto* const layout = new QHBoxLayout { this };
        assert(layout);

        auto* const label = new QLabel { tr("Font:"), this };
        assert(label);
        layout->addWidget(label, 0);

        m_fonts.reset(new QFontComboBox { this });
        assert(m_fonts);
        layout->addWidget(m_fonts.get(), 1);

        m_size.reset(new QSpinBox { this });
        assert(m_size);
        layout->addWidget(m_size.get(), 0);
    }

    void connectWidgets()
    {
        this->connect(m_fonts.get(), SIGNAL(currentFontChanged(const QFont&)),
                      this,            SLOT(onFontChanged(const QFont&)));
        this->connect(m_size.get(), SIGNAL(valueChanged(int)),
                      this,            SLOT(onSizeChanged(const int)));
    }

    void initWidgets()
    {
        QFont font { m_settings.fontName(), static_cast<int>(m_settings.fontSize()) };
        m_fonts->setCurrentFont(font);
        m_size->setValue(m_settings.fontSize());
        m_size->setRange(0, 50);
    }

private Q_SLOTS:
    void onFontChanged(const QFont &f)
    {
        QFont font { f };
        font.setPointSize(m_size->value());

        Q_EMIT changed(font);
    }

    void onSizeChanged(const int size)
    {
        QFont font { m_fonts->currentFont() };
        font.setPointSize(size);

        Q_EMIT changed(font);
    }

private:
    Settings &m_settings;
    std::unique_ptr<QFontComboBox> m_fonts;
    std::unique_ptr<QSpinBox> m_size;
};


AppearanceSettingWidget::
AppearanceSettingWidget(Settings &settings)
{
    auto* const layout = new QVBoxLayout { this };
    assert(layout);

    m_fontPanel.reset(new FontPanel { settings });
    assert(m_fontPanel);

    m_colorPanel.reset(new ColorPanel { settings, m_fontPanel->font() });
    assert(m_colorPanel);

    layout->addWidget(m_colorPanel.get());
    layout->addWidget(m_fontPanel.get());

    layout->addStretch();

    connectWidgets();
}

AppearanceSettingWidget::~AppearanceSettingWidget() = default;

void AppearanceSettingWidget::
save()
{
    m_colorPanel->save();
    m_fontPanel->save();
}

void AppearanceSettingWidget::
connectWidgets()
{
    this->connect(m_colorPanel.get(), SIGNAL(changed()),
                  this,               SIGNAL(changed()));
    this->connect(m_fontPanel.get(), SIGNAL(changed(const QFont&)),
                  this,              SIGNAL(changed()));
    this->connect(m_fontPanel.get(), SIGNAL(changed(const QFont&)),
                  m_colorPanel.get(),  SLOT(onFontChanged(const QFont&)));
}

#include "appearance_setting_widget.moc"
