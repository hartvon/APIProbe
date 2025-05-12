#include "urledit.h"

UrlEdit::UrlEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_completer(new QCompleter(this))
    , m_model(new QStringListModel(this))
    , m_dropDownButton(new QToolButton(this))
{
    setFrame(true);
    loadHistory();
    m_model->setStringList(m_urlHistory);
    m_completer->setModel(m_model);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    setCompleter(m_completer);

    m_dropDownButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
    m_dropDownButton->setCursor(Qt::PointingHandCursor);
    m_dropDownButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");

    connect(m_dropDownButton, &QToolButton::clicked, this, &UrlEdit::showDropDownMenu);

    // 调整按钮位置
    QSize buttonSize = m_dropDownButton->sizeHint();
    setStyleSheet(QString("QLineEdit { padding-right: %1px; }").arg(buttonSize.width() + 3));
    m_dropDownButton->move(width() - buttonSize.width() - 3, (height() - buttonSize.height()) / 2);
}

UrlEdit::~UrlEdit()
{
    saveHistory();
}

void UrlEdit::addToHistory(const QString &url)
{
    if (url.isEmpty() || m_urlHistory.contains(url)) {
        return;
    }

    m_urlHistory.prepend(url);
    while (m_urlHistory.size() > MAX_HISTORY_SIZE) {
        m_urlHistory.removeLast();
    }

    m_model->setStringList(m_urlHistory);
    saveHistory();
}

void UrlEdit::loadHistory()
{
    m_urlHistory = m_settings.value("urls").toStringList();
}

void UrlEdit::saveHistory()
{
    m_settings.setValue("urls", m_urlHistory);
}

void UrlEdit::showDropDownMenu()
{
    m_completer->complete();
}

void UrlEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    QSize buttonSize = m_dropDownButton->sizeHint();
    m_dropDownButton->move(width() - buttonSize.width() - 3, (height() - buttonSize.height()) / 2);
}
