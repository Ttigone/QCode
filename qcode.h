#ifndef QCODE_H
#define QCODE_H

#include <QWidget>
#include <qtextedit.h>

QT_BEGIN_NAMESPACE
namespace Ui { class qcode; }
QT_END_NAMESPACE

class qcode : public QWidget
{
    Q_OBJECT

    friend class titleBar;
public:
    qcode(QWidget *parent = nullptr);
    ~qcode();

    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

    void save_history(QString path);

    QList<QString> get_history();

    void open_recent_file();

    void clear_history_record();

    void init_recent_menu();

    int get_current_table_count();

private slots:

    void _new_text_file_triggered();   // 处理 新建文本文件

    void _new_file_triggered();        // 处理 新建文件

    void _new_window_triggered();      // 处理 打开新窗口

    void _open_file_triggered();       // 处理 打开文件


    void _open_folder_triggered();     // 处理 打开文件夹

    void _open_recent_triggered();     // 处理 打开最近文件

    void _save_triggered();            // 处理 保存文件

    void _save_as_triggered();         // 处理 另存为文件

    void _close_editor_triggered();    // 处理 关闭编辑

    void _close_window_triggered();    // 处理 关闭窗口


    void _undo_triggered();            // 处理 撤销

    void _redo_triggered();            // 处理

    void _cut_triggered();             // 处理 剪切

    void _copy_triggered();            // 处理 复制

    void _paste_triggered();           // 处理 黏贴

    void _find_triggered();            // 处理 寻找

    void _replace_triggered();         // 处理 替换


    void _select_all_triggered();      // 处理 全选择


    void _command_palette_triggered(); // 处理 打开命令


    void _welcome_triggered();

    void _show_all_commands_triggered();

    void _about_triggered();


    void on_tabWidget_tabCloseRequested(int index);

signals:



private:
    Ui::qcode *ui;

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度

    QString current_file;

    QString file_name;

};
#endif // QCODE_H
