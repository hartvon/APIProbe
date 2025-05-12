#include "apiprobe.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpacerItem>

APIProbe::APIProbe(QWidget *parent)
    : QMainWindow(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // 创建中央部件和主布局
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    m_mainLayout = new QVBoxLayout(m_centralWidget);

    // 创建顶部HTTP请求部分
    m_requestLayout = new QHBoxLayout();
    m_methodComboBox = new QComboBox(this);
    m_urlLineEdit = new UrlLineEdit(this);
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

    // 创建Parameters标签页
    m_parametersTab = new QWidget();
    m_parametersLayout = new QVBoxLayout(m_parametersTab);
    m_paramsGridLayout = new QGridLayout();
    m_keyLabel = new QLabel("Key", this);
    m_valueLabel = new QLabel("Value", this);
    m_addParamLayout = new QHBoxLayout();
    m_addParamButton = new QPushButton("+", this);

    // 设置Parameters标签页布局
    m_paramsGridLayout->addWidget(m_keyLabel, 0, 0);
    m_paramsGridLayout->addWidget(m_valueLabel, 0, 1);
    m_paramsGridLayout->setColumnStretch(0, 1);
    m_paramsGridLayout->setColumnStretch(1, 1);
    m_paramsGridLayout->setColumnStretch(2, 0);

    m_addParamLayout->addWidget(m_addParamButton);
    m_addParamLayout->addStretch();

    m_parametersLayout->addLayout(m_paramsGridLayout);
    m_parametersLayout->addLayout(m_addParamLayout);
    // m_parametersLayout->setStretch(0);
    // m_parametersLayout->setStretch(1);
    m_parametersLayout->addStretch();

    // 创建Headers标签页
    m_headersTab = new QWidget();
    m_headersLayout = new QVBoxLayout(m_headersTab);
    m_headersGridLayout = new QGridLayout();
    m_headerKeyLabel = new QLabel("Key", this);
    m_headerValueLabel = new QLabel("Value", this);
    m_addHeaderLayout = new QHBoxLayout();
    m_addHeaderButton = new QPushButton("+", this);

    // 设置Headers标签页布局
    m_headersGridLayout->addWidget(m_headerKeyLabel, 0, 0);
    m_headersGridLayout->addWidget(m_headerValueLabel, 0, 1);
    m_headersGridLayout->setColumnStretch(0, 1);
    m_headersGridLayout->setColumnStretch(1, 1);
    m_headersGridLayout->setColumnStretch(2, 0);

    m_addHeaderLayout->addWidget(m_addHeaderButton);
    m_addHeaderLayout->addStretch();

    m_headersLayout->addLayout(m_headersGridLayout);
    m_headersLayout->addLayout(m_addHeaderLayout);
    m_headersLayout->addStretch();

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
    connect(m_addParamButton, &QPushButton::clicked, this, &APIProbe::addParameterRow);
    connect(m_addHeaderButton, &QPushButton::clicked, this, &APIProbe::addHeaderRow);
}

APIProbe::~APIProbe()
{
    // Qt会自动删除子对象，不需要手动删除
}

void APIProbe::addParameterRow()
{
    int row = m_paramsGridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);
    deleteButton->setMaximumWidth(30);

    m_paramsGridLayout->addWidget(keyEdit, row, 0);
    m_paramsGridLayout->addWidget(valueEdit, row, 1);
    m_paramsGridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(m_paramsGridLayout, row);
    });
}

void APIProbe::addHeaderRow()
{
    int row = m_headersGridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);
    deleteButton->setMaximumWidth(30);

    m_headersGridLayout->addWidget(keyEdit, row, 0);
    m_headersGridLayout->addWidget(valueEdit, row, 1);
    m_headersGridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(m_headersGridLayout, row);
    });
}

void APIProbe::deleteRow(QGridLayout *layout, int row)
{
    for (int col = 0; col < layout->columnCount(); ++col) {
        QLayoutItem *item = layout->itemAtPosition(row, col);
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                layout->removeItem(item);
                delete widget;
            }
        }
    }
}

QJsonObject APIProbe::collectParameters()
{
    QJsonObject params;
    for (int row = 1; row < m_paramsGridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(m_paramsGridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(m_paramsGridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            params.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return params;
}

QMap<QString, QString> APIProbe::collectHeaders()
{
    QMap<QString, QString> headers;
    for (int row = 1; row < m_headersGridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(m_headersGridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(m_headersGridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            headers.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return headers;
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
        m_responseTextEdit->setPlainText("错误: " + reply->errorString());
    }

    reply->deleteLater();
}
