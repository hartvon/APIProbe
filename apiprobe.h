#ifndef APIPROBE_H
#define APIPROBE_H

#include <QWidget>
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
#include "requestheader.h"

class APIProbe : public QWidget
{
    Q_OBJECT

public:
    APIProbe(QWidget *parent = nullptr);
    ~APIProbe();

private slots:
    void sendRequest();
    void handleResponse(QNetworkReply *reply);
    void clearResponse();
    void copyResponse();

private:
    // 网络管理器
    QNetworkAccessManager *m_networkManager;

    // 快捷键
    QShortcut *m_sendShortcut;

private:
    // UI组件
    QVBoxLayout *m_mainLayout;

    // 顶部HTTP请求部分
    QHBoxLayout *m_requestLineLayout;
    QComboBox *m_methodComboBox;
    UrlEdit *m_urlLineEdit;
    QPushButton *m_sendButton;
    QPushButton *m_clearButton;
    QPushButton *m_copyButton;

    // 请求部分
    QGroupBox *m_requestGroup;
    QVBoxLayout *m_requestLayout;
    RequestHeader *m_requestHeader;
    QPlainTextEdit *m_requestBody;

    // 响应部分
    QGroupBox *m_responseGroup;
    QVBoxLayout *m_responseLayout;
    QPlainTextEdit *m_responseTextEdit;
};

#endif // APIPROBE_H
