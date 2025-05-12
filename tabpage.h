#ifndef TABPAGE_H
#define TABPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>
#include <QMap>

class TabPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabPage(const QString &title, QWidget *parent = nullptr);
    QJsonObject collectData();
    QMap<QString, QString> collectDataAsMap();

private slots:
    void addRow();
    void deleteRow(int row);

private:
    QVBoxLayout *m_mainLayout;
    QGridLayout *m_gridLayout;
    QLabel *m_keyLabel;
    QLabel *m_valueLabel;
    QHBoxLayout *m_addLayout;
    QPushButton *m_addButton;
};

#endif // TABPAGE_H
