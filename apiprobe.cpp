#include "apiprobe.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpacerItem>

APIProbe::APIProbe(QWidget *parent)
    : QMainWindow(parent)
    , networkManager(new QNetworkAccessManager(this))
{
    // 创建中央部件和主布局
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // 创建顶部HTTP请求部分
    requestLayout = new QHBoxLayout();
    methodComboBox = new QComboBox(this);
    urlLineEdit = new UrlLineEdit(this);
    sendButton = new QPushButton("Send", this);

    // 初始化HTTP方法下拉框
    methodComboBox->addItem("GET");
    methodComboBox->addItem("POST");
    methodComboBox->addItem("PUT");
    methodComboBox->addItem("DELETE");

    // 设置URL输入框占位符
    urlLineEdit->setPlaceholderText("Enter URL");

    // 添加组件到请求布局
    requestLayout->addWidget(methodComboBox);
    requestLayout->addWidget(urlLineEdit);
    requestLayout->addWidget(sendButton);

    // 创建请求参数和头部标签页
    requestTabWidget = new QTabWidget(this);

    // 创建Parameters标签页
    parametersTab = new QWidget();
    parametersLayout = new QVBoxLayout(parametersTab);
    paramsGridLayout = new QGridLayout();
    keyLabel = new QLabel("Key", this);
    valueLabel = new QLabel("Value", this);
    addParamLayout = new QHBoxLayout();
    addParamButton = new QPushButton("+", this);

    // 设置Parameters标签页布局
    paramsGridLayout->addWidget(keyLabel, 0, 0);
    paramsGridLayout->addWidget(valueLabel, 0, 1);
    paramsGridLayout->setColumnStretch(0, 1);
    paramsGridLayout->setColumnStretch(1, 1);
    paramsGridLayout->setColumnStretch(2, 0);

    addParamLayout->addWidget(addParamButton);
    addParamLayout->addStretch();

    parametersLayout->addLayout(paramsGridLayout);
    parametersLayout->addLayout(addParamLayout);
    // parametersLayout->setStretch(0);
    // parametersLayout->setStretch(1);
    parametersLayout->addStretch();

    // 创建Headers标签页
    headersTab = new QWidget();
    headersLayout = new QVBoxLayout(headersTab);
    headersGridLayout = new QGridLayout();
    headerKeyLabel = new QLabel("Key", this);
    headerValueLabel = new QLabel("Value", this);
    addHeaderLayout = new QHBoxLayout();
    addHeaderButton = new QPushButton("+", this);

    // 设置Headers标签页布局
    headersGridLayout->addWidget(headerKeyLabel, 0, 0);
    headersGridLayout->addWidget(headerValueLabel, 0, 1);
    headersGridLayout->setColumnStretch(0, 1);
    headersGridLayout->setColumnStretch(1, 1);
    headersGridLayout->setColumnStretch(2, 0);

    addHeaderLayout->addWidget(addHeaderButton);
    addHeaderLayout->addStretch();

    headersLayout->addLayout(headersGridLayout);
    headersLayout->addLayout(addHeaderLayout);
    headersLayout->addStretch();

    // 添加标签页到TabWidget
    requestTabWidget->addTab(parametersTab, "Parameters");
    requestTabWidget->addTab(headersTab, "Headers");

    // 创建响应部分
    responseGroup = new QGroupBox("Response", this);
    responseLayout = new QVBoxLayout(responseGroup);
    responseTextEdit = new QPlainTextEdit(this);
    responseTextEdit->setReadOnly(true);
    responseLayout->addWidget(responseTextEdit);

    // 添加所有部分到主布局
    mainLayout->addLayout(requestLayout);
    mainLayout->addWidget(requestTabWidget);
    mainLayout->addWidget(responseGroup);

    // 设置窗口属性
    setWindowTitle("HttpTinker");
    resize(800, 600);

    // 连接信号槽
    connect(sendButton, &QPushButton::clicked, this, &APIProbe::sendRequest);
    connect(networkManager, &QNetworkAccessManager::finished, this, &APIProbe::handleResponse);
    connect(addParamButton, &QPushButton::clicked, this, &APIProbe::addParameterRow);
    connect(addHeaderButton, &QPushButton::clicked, this, &APIProbe::addHeaderRow);
}

APIProbe::~APIProbe()
{
    // Qt会自动删除子对象，不需要手动删除
}

void APIProbe::addParameterRow()
{
    int row = paramsGridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);
    deleteButton->setMaximumWidth(30);

    paramsGridLayout->addWidget(keyEdit, row, 0);
    paramsGridLayout->addWidget(valueEdit, row, 1);
    paramsGridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(paramsGridLayout, row);
    });
}

void APIProbe::addHeaderRow()
{
    int row = headersGridLayout->rowCount();

    QLineEdit *keyEdit = new QLineEdit(this);
    QLineEdit *valueEdit = new QLineEdit(this);
    QPushButton *deleteButton = new QPushButton("-", this);
    deleteButton->setMaximumWidth(30);

    headersGridLayout->addWidget(keyEdit, row, 0);
    headersGridLayout->addWidget(valueEdit, row, 1);
    headersGridLayout->addWidget(deleteButton, row, 2);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        deleteRow(headersGridLayout, row);
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
    for (int row = 1; row < paramsGridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(paramsGridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(paramsGridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            params.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return params;
}

QMap<QString, QString> APIProbe::collectHeaders()
{
    QMap<QString, QString> headers;
    for (int row = 1; row < headersGridLayout->rowCount(); ++row) {
        QLineEdit *keyEdit = qobject_cast<QLineEdit*>(headersGridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit *valueEdit = qobject_cast<QLineEdit*>(headersGridLayout->itemAtPosition(row, 1)->widget());
        if (keyEdit && valueEdit && !keyEdit->text().isEmpty()) {
            headers.insert(keyEdit->text(), valueEdit->text());
        }
    }
    return headers;
}

void APIProbe::sendRequest()
{
    QString url = urlLineEdit->text();
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

    QString method = methodComboBox->currentText();
    QJsonDocument paramsDoc(collectParameters());
    QByteArray data = paramsDoc.toJson();

    if (method == "GET") {
        networkManager->get(request);
    } else if (method == "POST") {
        networkManager->post(request, data);
    } else if (method == "PUT") {
        networkManager->put(request, data);
    } else if (method == "DELETE") {
        networkManager->deleteResource(request);
    }
}

void APIProbe::handleResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        urlLineEdit->addToHistory(urlLineEdit->text());
        QByteArray response = reply->readAll();
        // 尝试格式化JSON响应
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (!doc.isNull()) {
            responseTextEdit->setPlainText(QString::fromUtf8(doc.toJson(QJsonDocument::Indented)));
        } else {
            responseTextEdit->setPlainText(QString::fromUtf8(response));
        }
    } else {
        responseTextEdit->setPlainText("错误: " + reply->errorString());
    }

    reply->deleteLater();
}
