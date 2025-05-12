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
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // 顶部HTTP请求部分
    QHBoxLayout *requestLayout;
    QComboBox *methodComboBox;
    UrlLineEdit *urlLineEdit;
    QPushButton *sendButton;

    // 请求参数和头部标签页
    QTabWidget *requestTabWidget;

    // Parameters标签页
    QWidget *parametersTab;
    QVBoxLayout *parametersLayout;
    QGridLayout *paramsGridLayout;
    QLabel *keyLabel;
    QLabel *valueLabel;
    QHBoxLayout *addParamLayout;
    QPushButton *addParamButton;

    // Headers标签页
    QWidget *headersTab;
    QVBoxLayout *headersLayout;
    QGridLayout *headersGridLayout;
    QLabel *headerKeyLabel;
    QLabel *headerValueLabel;
    QHBoxLayout *addHeaderLayout;
    QPushButton *addHeaderButton;

    // 响应部分
    QGroupBox *responseGroup;
    QVBoxLayout *responseLayout;
    QPlainTextEdit *responseTextEdit;

    // 网络管理器
    QNetworkAccessManager *networkManager;
};

#endif // APIPROBE_H
