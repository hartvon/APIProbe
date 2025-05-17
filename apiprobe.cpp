#include "apiprobe.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpacerItem>

APIProbe::APIProbe(QWidget *parent)
    : QWidget(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);

    // 创建顶部HTTP请求部分
    m_requestLayout = new QHBoxLayout();
    m_methodComboBox = new QComboBox(this);
    m_urlLineEdit = new UrlEdit(this);
    m_sendButton = new QPushButton("Send", this);

    // 初始化HTTP方法下拉框
    m_methodComboBox->addItem("GET");
    m_methodComboBox->addItem("POST");
    m_methodComboBox->addItem("PUT");
    m_methodComboBox->addItem("DELETE");

    // 设置URL输入框占位符
    m_urlLineEdit->setPlaceholderText("Enter URL");

    // 添加组件到请求布局
    m_requestLayout->addWidget(m_methodComboBox);
    m_requestLayout->addWidget(m_urlLineEdit);
    m_requestLayout->addWidget(m_sendButton);

    // 创建请求参数和头部标签页
    m_requestTabWidget = new QTabWidget(this);

    // 创建标签页
    m_parametersTab = new TabPage("Parameters", this);
    m_headersTab = new TabPage("Headers", this);

    // 添加标签页到TabWidget
    m_requestTabWidget->addTab(m_parametersTab, "Parameters");
    m_requestTabWidget->addTab(m_headersTab, "Headers");

    // 创建响应部分
    m_responseGroup = new QGroupBox("Response", this);
    m_responseLayout = new QVBoxLayout(m_responseGroup);
    m_responseTextEdit = new QPlainTextEdit(this);
    m_responseTextEdit->setReadOnly(true);
    m_responseLayout->addWidget(m_responseTextEdit);

    // 添加所有部分到主布局
    m_mainLayout->addLayout(m_requestLayout);
    m_mainLayout->addWidget(m_requestTabWidget);
    m_mainLayout->addWidget(m_responseGroup);

    // 设置窗口属性
    setWindowTitle("HttpTinker");
    resize(800, 600);

    // 连接信号槽
    connect(m_sendButton, &QPushButton::clicked, this, &APIProbe::sendRequest);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &APIProbe::handleResponse);

}

APIProbe::~APIProbe()
{
    // Qt会自动删除子对象，不需要手动删除
}

QJsonObject APIProbe::collectParameters()
{
    return m_parametersTab->collectData();
}

QMap<QString, QString> APIProbe::collectHeaders()
{
    return m_headersTab->collectDataAsMap();
}

void APIProbe::sendRequest()
{
    QString url = m_urlLineEdit->text();
    if (url.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter URL");
        return;
    }

    QNetworkRequest request(url);

    // 设置请求头
    QMap<QString, QString> headers = collectHeaders();
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QString method = m_methodComboBox->currentText();
    QJsonDocument paramsDoc(collectParameters());
    QByteArray data = paramsDoc.toJson();

    if (method == "GET") {
        m_networkManager->get(request);
    } else if (method == "POST") {
        m_networkManager->post(request, data);
    } else if (method == "PUT") {
        m_networkManager->put(request, data);
    } else if (method == "DELETE") {
        m_networkManager->deleteResource(request);
    }
}

void APIProbe::handleResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        m_urlLineEdit->addToHistory(m_urlLineEdit->text());
        QByteArray response = reply->readAll();
        // 尝试格式化JSON响应
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (!doc.isNull()) {
            m_responseTextEdit->setPlainText(QString::fromUtf8(doc.toJson(QJsonDocument::Indented)));
        } else {
            m_responseTextEdit->setPlainText(QString::fromUtf8(response));
        }
    } else {
        m_responseTextEdit->setPlainText("Error: " + reply->errorString());
    }

    reply->deleteLater();
}
