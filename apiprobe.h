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

#include "urledit.h"
#include "tabpage.h"

class APIProbe : public QMainWindow
{
    Q_OBJECT

public:
    APIProbe(QWidget *parent = nullptr);
    ~APIProbe();

private slots:
    void sendRequest();
    void handleResponse(QNetworkReply *reply);

private:
    QJsonObject collectParameters();
    QMap<QString, QString> collectHeaders();

    // UI组件
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;

    // 顶部HTTP请求部分
    QHBoxLayout *m_requestLayout;
    QComboBox *m_methodComboBox;
    UrlEdit *m_urlLineEdit;
    QPushButton *m_sendButton;

    // 请求参数和头部标签页
    QTabWidget *m_requestTabWidget;

    // 标签页
    TabPage *m_parametersTab;
    TabPage *m_headersTab;

    // 响应部分
    QGroupBox *m_responseGroup;
    QVBoxLayout *m_responseLayout;
    QPlainTextEdit *m_responseTextEdit;

    // 网络管理器
    QNetworkAccessManager *m_networkManager;
};

#endif // APIPROBE_H
