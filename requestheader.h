#ifndef REQUESTHEADER_H
#define REQUESTHEADER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>
#include <QMap>

class RequestHeader : public QWidget
{
    Q_OBJECT

public:
    explicit RequestHeader(QWidget *parent = nullptr);
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
    QPushButton *m_addButton;
};

#endif // REQUESTHEADER_H
