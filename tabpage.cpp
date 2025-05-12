#include "tabpage.h"

TabPage::TabPage(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_gridLayout = new QGridLayout();
    m_keyLabel = new QLabel("Key", this);
    m_valueLabel = new QLabel("Value", this);
    m_addLayout = new QHBoxLayout();
    m_addButton = new QPushButton("+", this);

    // 设置网格布局
    m_gridLayout->addWidget(m_keyLabel, 0, 0);
    m_gridLayout->addWidget(m_valueLabel, 0, 1);
    m_gridLayout->setColumnStretch(0, 1);
    m_gridLayout->setColumnStretch(1, 1);
    m_gridLayout->setColumnStretch(2, 0);

    // 设置添加按钮布局
    m_addLayout->addWidget(m_addButton);
    m_addLayout->addStretch();

    // 添加所有布局
    m_mainLayout->addLayout(m_gridLayout);
    m_mainLayout->addLayout(m_addLayout);
    m_mainLayout->addStretch();

    // 连接信号槽
    connect(m_addButton, &QPushButton::clicked, this, &TabPage::addRow);
}

void TabPage::addRow()
{
    int row = m_gridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);
    deleteButton->setMaximumWidth(30);

    m_gridLayout->addWidget(keyEdit, row, 0);
    m_gridLayout->addWidget(valueEdit, row, 1);
    m_gridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(row);
    });
}

void TabPage::deleteRow(int row)
{
    for (int col = 0; col < m_gridLayout->columnCount(); ++col) {
        QLayoutItem *item = m_gridLayout->itemAtPosition(row, col);
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                m_gridLayout->removeItem(item);
                delete widget;
            }
        }
    }
}

QJsonObject TabPage::collectData()
{
    QJsonObject data;
    for (int row = 1; row < m_gridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(m_gridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(m_gridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            data.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return data;
}

QMap<QString, QString> TabPage::collectDataAsMap()
{
    QMap<QString, QString> data;
    for (int row = 1; row < m_gridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(m_gridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(m_gridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            data.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return data;
}
