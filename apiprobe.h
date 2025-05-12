#ifndef APIPROBE_H
#define APIPROBE_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "urllineedit.h"

class APIProbe : public QMainWindow
{
    Q_OBJECT

public:
    APIProbe(QWidget *parent = nullptr);
    ~APIProbe();

private slots:
    void sendRequest();
    void handleResponse(QNetworkReply *reply);
    void addParameterRow();
    void addHeaderRow();
    void deleteRow(QGridLayout *layout, int row);

private:
    QJsonObject collectParameters();
    QMap<QString, QString> collectHeaders();

    // UI组件
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;

    // 顶部HTTP请求部分
    QHBoxLayout *m_requestLayout;
    QComboBox *m_methodComboBox;
    UrlLineEdit *m_urlLineEdit;
    QPushButton *m_sendButton;

    // 请求参数和头部标签页
    QTabWidget *m_requestTabWidget;

    // Parameters标签页
    QWidget *m_parametersTab;
    QVBoxLayout *m_parametersLayout;
    QGridLayout *m_paramsGridLayout;
    QLabel *m_keyLabel;
    QLabel *m_valueLabel;
    QHBoxLayout *m_addParamLayout;
    QPushButton *m_addParamButton;

    // Headers标签页
    QWidget *m_headersTab;
    QVBoxLayout *m_headersLayout;
    QGridLayout *m_headersGridLayout;
    QLabel *m_headerKeyLabel;
    QLabel *m_headerValueLabel;
    QHBoxLayout *m_addHeaderLayout;
    QPushButton *m_addHeaderButton;

    // 响应部分
    QGroupBox *m_responseGroup;
    QVBoxLayout *m_responseLayout;
    QPlainTextEdit *m_responseTextEdit;

    // 网络管理器
    QNetworkAccessManager *m_networkManager;
};

#endif // APIPROBE_H
