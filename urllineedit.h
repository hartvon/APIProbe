#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QSettings>
#include <QCompleter>
#include <QStringListModel>
#include <QToolButton>
#include <QStyle>

class UrlLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit UrlLineEdit(QWidget *parent = nullptr);
    ~UrlLineEdit();

    void addToHistory(const QString &url);
    void loadHistory();
    void saveHistory();

private:
    QStringList m_urlHistory;
    QCompleter *m_completer;
    QStringListModel *m_model;
    QSettings m_settings{"HttpTinker", "URLHistory"};
    QToolButton *m_dropDownButton;
    static const int MAX_HISTORY_SIZE = 20;

private slots:
    void showDropDownMenu();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // URLLINEEDIT_H
