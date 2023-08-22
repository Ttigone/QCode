#include "qcode.h"
#include "./ui_qcode.h"
#include "qtextedit.h"
#include "titleBar.h"


#include <qboxlayout.h>


//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include <QFileDialog>
#include <QMessageBox>
#endif


qcode::qcode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::qcode)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    resize(1200, 800);

    titleBar *pTitleBar = new titleBar(this);
//    pTitleBar->setFixedHeight(23);
    installEventFilter(pTitleBar);

//    setWindowTitle("Custom Window");
    QIcon w_icon(":/images/qc.png");
    setWindowIcon(w_icon);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pTitleBar);
//    pLayout->addStretch();
    pLayout->setSpacing(5);  // 与下一部件的间距
    pLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素

    text = new QTextEdit(this);
    text->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    pLayout->addWidget(text);
    text->setStyleSheet("border: 2px solid red;");

    // 将菜单栏的 新建文本文件信号关联到匹配的槽函数
    connect(pTitleBar, &titleBar::new_text_file_triggered, this, &qcode::_new_text_file_triggered);
    connect(pTitleBar, &titleBar::open_file_triggered, this, &qcode::_open_file_triggered);
    connect(pTitleBar, &titleBar::save_triggered, this, &qcode::_save_triggered);

    setLayout(pLayout);
    m_nBorderWidth = 3;
}

qcode::~qcode()
{
    delete ui;
}



// 拖动窗口边缘，实现缩放
bool qcode::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != nullptr)              // 设置这个会导致 titlebar 无效
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
            && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void qcode::_new_text_file_triggered()
{
    qDebug() << "create new file";
    current_file.clear();
    text->setText("");
}

void qcode::_open_file_triggered()
{
    file_name = QFileDialog::getOpenFileName(this, "打开文件");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
        return;
    }
    current_file = file_name;
    QTextStream in(&file);
    QString content_text = in.readAll();
    text->setText(content_text);
    file.close();
}

void qcode::_save_triggered()
{
    if (current_file.isEmpty()) {
        file_name = QFileDialog::getSaveFileName(this, "保存文件");
        current_file = file_name;
    } else {
        file_name = current_file;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString content_text = text->toHtml();

    out << content_text;
    file.close();
}

