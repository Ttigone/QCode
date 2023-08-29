#include "qcode.h"
#include "./ui_qcode.h"
#include "qtextedit.h"
#include "titleBar.h"
#include "QSettings"
//#include "textedit.h"
#include "codeedit.h"

#include <qboxlayout.h>
#include <QFileDialog>
#include <QMessageBox>

//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include <qlist.h>
#endif


QSettings *mSettings;    // 配置文件

QList<QString> get_history();

qcode::qcode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::qcode)
{
    ui->setupUi(this);




    // 配置文件
    if (mSettings == nullptr) {
        mSettings = new QSettings("setting.ini", QSettings::IniFormat);
    }

    // 初始化界面
    init_widget();
    // 初始化信号槽连接
    init_connection();

    // 初始化最近菜单
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

void qcode::init_widget()
{
    // 设置无标题框
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // 设置初始大小
    resize(1200, 800);

    setAttribute(Qt::WA_StyledBackground);

    // 设置标题图标
    QIcon w_icon(":/images/qc.png");
    setWindowIcon(w_icon);

    // 为主界面窗口设置垂直布局
    QVBoxLayout *pLayout = new QVBoxLayout(this);

    // 自定义标题框
    m_title_bar = new titleBar(this);
    // 安装过滤器
    installEventFilter(m_title_bar);

    // 添加第一个 widget
    pLayout->addWidget(m_title_bar);

    // 设置间隙
    pLayout->setSpacing(5);  // 与下一部件的间距

    pLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素

    // 添加第二个 widget
    pLayout->addWidget(ui->tabWidget);

    // 应用界面布局
    setLayout(pLayout);

    // 可拖拽的边距
    m_nBorderWidth = 3;
}

void qcode::init_connection()
{
    // 将菜单栏的 新建文本文件信号关联到匹配的槽函数
    connect(m_title_bar, &titleBar::new_text_file_triggered, this, &qcode::_new_text_file_triggered);
    connect(m_title_bar, &titleBar::new_window_triggered, this, &qcode::_new_window_triggered);

    connect(m_title_bar, &titleBar::open_file_triggered, this, &qcode::_open_file_triggered);

    connect(m_title_bar, &titleBar::save_triggered, this, &qcode::_save_triggered);
    connect(m_title_bar, &titleBar::save_as_triggered, this, &qcode::_save_as_triggered);

    connect(m_title_bar, &titleBar::undo_triggered, this, &qcode::_undo_triggered);
    connect(m_title_bar, &titleBar::redo_triggered, this, &qcode::_redo_triggered);
    connect(m_title_bar, &titleBar::cut_triggered, this, &qcode::_cut_triggered);
    connect(m_title_bar, &titleBar::copy_triggered, this, &qcode::_copy_triggered);
    connect(m_title_bar, &titleBar::paste_triggered, this, &qcode::_paste_triggered);

    connect(m_title_bar, &titleBar::about_triggered, this, &qcode::_about_triggered);
}

void qcode::save_history(QString path)
{
    // 读取历史
    QList<QString> lists = get_history();
    lists.append(path);

    foreach (QString str, lists) {
        if (str == path) {
            lists.removeOne(str);
        }
    }
    lists.append(path);

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
    QTextStream in(&file);
    QString content_text = in.readAll();

    CodeEdit *code_editor = new CodeEdit(this);
    code_editor->setPlainText(content_text);
    ui->tabWidget->addTab(code_editor, file_name);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    file.close();

    save_history(file_name);

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

    qDebug() << recent;  // 能找到

    // 去重
    QList<QObject *> chlists = recent->children().toList();
    QSet<QObject *> set_lists(chlists.cbegin(), chlists.cend());

    foreach (QObject *ch, set_lists) {
        QAction *action = (QAction *)ch;
        recent->removeAction(action);
    }

    // BUG
    QList<QString> lists = get_history();          // 获取曾经未删除历史记录


    for (int i = lists.size() - 1; i >= 0; --i) {
        recent->addAction(lists.at(i), this, &qcode::open_recent_file);  // 添加新记录
    }


    if (lists.size() > 0) {
        recent->addSeparator();
        recent->addAction("Clear Recently Opened", this, &qcode::clear_history_record);
    }
}

int qcode::get_current_table_count()
{
    return ui->tabWidget->count();
}

void qcode::_new_text_file_triggered()
{
    ui->tabWidget->addTab(new CodeEdit(this), "Untitled.txt");
}

void qcode::_new_file_triggered()
{

}

void qcode::_new_window_triggered()
{
    qcode *new_window = new qcode();
    new_window->show();
}

void qcode::_open_file_triggered()
{
    file_name = QFileDialog::getOpenFileName(this, "打开文件");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString content_text = in.readAll();


    CodeEdit *code_editor = new CodeEdit(this);

    code_editor->setPlainText(content_text);
    ui->tabWidget->addTab(code_editor, file_name);

    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);


    file.close();

    save_history(file_name);

    init_recent_menu();
}

void qcode::_open_folder_triggered()
{

}

void qcode::_save_triggered()
{

    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();

    if (code_editor) {
        if (code_editor->save_file()) {
            QString file_name = code_editor->get_file_name();
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), file_name);
            save_history(file_name);
            init_recent_menu();
        }
    }


}

void qcode::_save_as_triggered()
{


    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();

    if (code_editor) {
        if (code_editor->save_as_file()) {
            QString file_name = code_editor->get_file_name();
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), file_name);
            save_history(file_name);
            init_recent_menu();
        }
    }



}

void qcode::_close_editor_triggered()
{

}

void qcode::_close_window_triggered()
{

}

void qcode::_undo_triggered()
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (code_editor) {
        code_editor->undo();
    }
}

void qcode::_redo_triggered()
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (code_editor) {
        code_editor->redo();
    }
}

void qcode::_cut_triggered()
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (code_editor) {
        code_editor->cut();
    }
}

void qcode::_copy_triggered()
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (code_editor) {
        code_editor->copy();
    }
}

void qcode::_paste_triggered()
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (code_editor) {
        code_editor->paste();
    }
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

void qcode::on_tabWidget_tabCloseRequested(int index)
{
    CodeEdit *code_editor = (CodeEdit *)ui->tabWidget->currentWidget();
    if (!code_editor->check_saved()) {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "警告", "文件未保存, 是否保存文件", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);


        if (btn == QMessageBox::Yes) {
            if (code_editor->save_file()) {
                QString file_name = code_editor->get_file_name();
                save_history(file_name);
                init_recent_menu();
            }
            return;
        } else if (btn == QMessageBox::Cancel) {
            return;
        }
    }
//    delete ui->tabWidget->currentWidget();
    ui->tabWidget->removeTab(index);
    delete code_editor;
}


