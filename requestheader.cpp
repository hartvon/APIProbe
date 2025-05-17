#include "requestheader.h"

RequestHeader::RequestHeader(QWidget *parent)
    : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_gridLayout = new QGridLayout();

    // 设置网格布局
    m_gridLayout->setColumnStretch(0, 1);
    m_gridLayout->setColumnStretch(1, 1);
    m_gridLayout->setColumnStretch(2, 0);

    m_keyLabel = new QLabel("Key", this);
    m_valueLabel = new QLabel("Value", this);
    m_addButton = new QPushButton("+", this);

    m_gridLayout->addWidget(m_keyLabel, 0, 0);
    m_gridLayout->addWidget(m_valueLabel, 0, 1);
    m_gridLayout->addWidget(m_addButton, 0, 2);

    // 添加所有布局
    m_mainLayout->addLayout(m_gridLayout);
    m_mainLayout->addLayout(m_addLayout);
    m_mainLayout->addStretch();

    // 连接信号槽
    connect(m_addButton, &QPushButton::clicked, this, &RequestHeader::addRow);
}

void RequestHeader::addRow()
{
    int row = m_gridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);

    m_gridLayout->addWidget(keyEdit, row, 0);
    m_gridLayout->addWidget(valueEdit, row, 1);
    m_gridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(row);
    });
}

void RequestHeader::deleteRow(int row)
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

QMap<QString, QString> RequestHeader::collectDataAsMap()
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
