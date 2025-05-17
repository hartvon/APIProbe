#include "apiprobe.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpacerItem>
#include <QClipboard>

APIProbe::APIProbe(QWidget *parent)
    : QWidget(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // 主布局
    m_mainLayout = new QVBoxLayout(this);

    // 请求行
    m_requestLineLayout = new QHBoxLayout();
    m_methodComboBox = new QComboBox(this);
    m_urlLineEdit = new UrlEdit(this);
    m_sendButton = new QPushButton("Send", this);

    m_methodComboBox->addItem("GET");
    m_methodComboBox->addItem("POST");
    m_methodComboBox->addItem("PUT");
    m_methodComboBox->addItem("DELETE");

    m_urlLineEdit->setPlaceholderText("Enter URL");

    m_requestLineLayout->addWidget(m_methodComboBox);
    m_requestLineLayout->addWidget(m_urlLineEdit);
    m_requestLineLayout->addWidget(m_sendButton);


    // 请求头 + 请求体
    m_requestGroup = new QGroupBox("Request", this);
    m_requestLayout = new QVBoxLayout(m_requestGroup);
    m_requestHeader = new RequestHeader(this);

    m_requestBody = new QPlainTextEdit(this);
    m_requestBody->setPlaceholderText("Enter request body");
    m_requestBody->setMinimumHeight(80);
    m_requestHeader->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    m_requestLayout->addWidget(m_requestHeader, 0);
    m_requestLayout->addWidget(m_requestBody, 1);


    // 响应体
    m_responseGroup = new QGroupBox("Response", this);
    m_responseLayout = new QVBoxLayout(m_responseGroup);
    m_responseTextEdit = new QPlainTextEdit(this);
    m_responseTextEdit->setReadOnly(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_clearButton = new QPushButton("Clear", this);
    m_copyButton = new QPushButton("Copy", this);
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addWidget(m_copyButton);
    buttonLayout->addStretch();

    m_responseLayout->addWidget(m_responseTextEdit);
    m_responseLayout->addLayout(buttonLayout);


    // 添加所有部分到主布局
    m_mainLayout->addLayout(m_requestLineLayout);
    m_mainLayout->addWidget(m_requestGroup, 1);
    m_mainLayout->addWidget(m_responseGroup, 2);

    // 设置窗口属性
    setWindowTitle("HttpTinker");
    resize(800, 600);


    // 连接信号槽
    connect(m_sendButton, &QPushButton::clicked, this, &APIProbe::sendRequest);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &APIProbe::handleResponse);
    connect(m_clearButton, &QPushButton::clicked, this, &APIProbe::clearResponse);
    connect(m_copyButton, &QPushButton::clicked, this, &APIProbe::copyResponse);
}

APIProbe::~APIProbe()
{
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
    QMap<QString, QString> headers = m_requestHeader->collectDataAsMap();
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QString method = m_methodComboBox->currentText();
    QByteArray data;
    if (method == "POST" || method == "PUT") {
        data = m_requestBody->toPlainText().toUtf8();
    }

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

void APIProbe::clearResponse()
{
    m_responseTextEdit->clear();
}

void APIProbe::copyResponse()
{
    QString text = m_responseTextEdit->toPlainText();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}
