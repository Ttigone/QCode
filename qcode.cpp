#include "qcode.h"
#include "./ui_qcode.h"
#include "qtextedit.h"
#include "titleBar.h"
#include "QSettings"

#include <qboxlayout.h>
#include <QFileDialog>
#include <QMessageBox>

//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include <qlist.h>
#endif


QSettings *mSettings;

QList<QString> get_history();

qcode::qcode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::qcode)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    resize(1200, 800);

    if (mSettings == nullptr) {
        mSettings = new QSettings("setting.ini", QSettings::IniFormat);
    }


    titleBar *pTitleBar = new titleBar(this);
//    pTitleBar->setFixedHeight(23);
    installEventFilter(pTitleBar);

//    setWindowTitle("Custom Window");
    QIcon w_icon(":/images/qc.png");
    setWindowIcon(w_icon);

    get_history();

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pTitleBar);
//    pLayout->addStretch();
    pLayout->setSpacing(5);  // 与下一部件的间距
    pLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素

    current_text_editor = new QTextEdit(this);
    current_text_editor->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    pLayout->addWidget(current_text_editor);
    current_text_editor->setStyleSheet("border: 2px solid red;");

    // 将菜单栏的 新建文本文件信号关联到匹配的槽函数
    connect(pTitleBar, &titleBar::new_text_file_triggered, this, &qcode::_new_text_file_triggered);
    connect(pTitleBar, &titleBar::open_file_triggered, this, &qcode::_open_file_triggered);
    connect(pTitleBar, &titleBar::save_triggered, this, &qcode::_save_triggered);
    connect(pTitleBar, &titleBar::save_as_triggered, this, &qcode::_save_as_triggered);

    connect(pTitleBar, &titleBar::undo_triggered, this, &qcode::_uodo_triggered);
    connect(pTitleBar, &titleBar::redo_triggered, this, &qcode::_redo_triggered);
    connect(pTitleBar, &titleBar::cut_triggered, this, &qcode::_cut_triggered);
    connect(pTitleBar, &titleBar::copy_triggered, this, &qcode::_copy_triggered);
    connect(pTitleBar, &titleBar::paste_triggered, this, &qcode::_paste_triggered);


    connect(pTitleBar, &titleBar::about_triggered, this, &qcode::_about_triggered);

    setLayout(pLayout);
    m_nBorderWidth = 3;

    init_recent_menu();
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

void qcode::save_history(QString path)
{

    // 读取历史
    QList<QString> lists = get_history();
    lists.append(path);

//    QSet<QString> set(lists.cbegin(), lists.cend());
//    QList<QString> new_lists = QList<QString>(set.cbegin(), set.cend());

    foreach (QString str, lists) {
        if (str == path) {
            lists.removeOne(str);
        }
    }
    lists.append(path);

//    int size = mSettings->beginReadArray("history");
//    mSettings->endArray();

    // 打开开始写入
    mSettings->beginWriteArray("history");
    for (int i = 0; i < lists.size(); ++i) {
        mSettings->setArrayIndex(i);

        // 保留字符串
        mSettings->setValue("path", lists.at(i));
    }


    // 关闭开始写入
    mSettings->endArray();

    init_recent_menu();
}

QList<QString> qcode::get_history()
{
    int size = mSettings->beginReadArray("history");

    QList<QString> lists;
    for (int i = 0; i < size; ++i) {
        mSettings->setArrayIndex(i);
        QString path = mSettings->value("path").toString();
        lists.append(path);
//        qDebug() << path;
    }
    mSettings->endArray();

    return lists;
}

void qcode::open_recent_file()
{
    QAction *action = (QAction *)sender();
    QString file_name = action->text();

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
        return;
    }
    current_file = file_name;
    QTextStream in(&file);
    QString content_text = in.readAll();
    current_text_editor->setText(content_text);
    file.close();

    save_history(current_file);

    init_recent_menu();
}

void qcode::clear_history_record()
{
    mSettings->remove("history");
    init_recent_menu();
}

void qcode::init_recent_menu()
{
    QMenu *recent = this->findChild<QMenu *>("recent");

    QList<QObject *> chlists = recent->children().toList();
    QSet<QObject *> set_lists(chlists.cbegin(), chlists.cend());

    foreach (QObject *ch, set_lists) {
        QAction *action = (QAction *)ch;
        recent->removeAction(action);
    }

    QList<QString> lists = get_history();          // 获取曾经未删除历史记录

//    for (int i = 0; i < lists.size(); ++i) {
//        recent->addAction(lists.at(lists.size() - i - 1), this, &qcode::open_recent_file);  // 添加新记录
//    }

    for (int i = lists.size() - 1; i >= 0; --i) {
        recent->addAction(lists.at(i), this, &qcode::open_recent_file);  // 添加新记录
    }


//    foreach (QString str, lists) {
//        recent->addAction(str, this, &qcode::open_recent_file);  // 添加新记录
//    }


    if (lists.size() > 0) {
        recent->addSeparator();
        recent->addAction("Clear Recently Opened", this, &qcode::clear_history_record);
    }
}

void qcode::_new_text_file_triggered()
{
    qDebug() << "create new file";
    current_file.clear();
    current_text_editor->setText("");
    init_recent_menu();
}

void qcode::_new_file_triggered()
{

}

void qcode::_new_window_triggered()
{

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
    current_text_editor->setText(content_text);
    file.close();

    save_history(current_file);

    init_recent_menu();
}

void qcode::_open_folder_triggered()
{

}

void qcode::_open_recent_triggered()
{

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
    QString content_text = current_text_editor->toPlainText();

    out << content_text;
    file.close();

    save_history(current_file);

    init_recent_menu();
}

void qcode::_save_as_triggered()
{
    file_name = QFileDialog::getSaveFileName(this, "另存文件");
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return;
    }
    current_file = file_name;
    QTextStream out(&file);
    QString content_text = current_text_editor->toPlainText();
    out << content_text;
    file.close();

    save_history(current_file);

    init_recent_menu();
}

void qcode::_close_editor_triggered()
{

}

void qcode::_close_window_triggered()
{

}

void qcode::_uodo_triggered()
{
    current_text_editor->undo();
}

void qcode::_redo_triggered()
{
    current_text_editor->redo();
}

void qcode::_cut_triggered()
{
    current_text_editor->cut();
}

void qcode::_copy_triggered()
{
    current_text_editor->copy();
}

void qcode::_paste_triggered()
{
    current_text_editor->paste();
}

void qcode::_find_triggered()
{

}

void qcode::_replace_triggered()
{

}

void qcode::_select_all_triggered()
{

}

void qcode::_command_palette_triggered()
{

}

void qcode::_welcome_triggered()
{

}

void qcode::_show_all_commands_triggered()
{

}

void qcode::_about_triggered()
{
    QMessageBox::about(this, "QCode", "乞丐版-vscode");
}


