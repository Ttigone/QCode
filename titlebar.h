#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QKeySequence>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QTimer>

class titleBar : public QWidget
{
    Q_OBJECT

public:
    explicit titleBar(QWidget *parent = nullptr);
    ~titleBar();

    void init_icon();

    void init_file();

    void init_edit();

    void init_selection();

    void init_view();

    void init_help();

    void init_btn();

    void init_menu();

    void init_widget();

    void init_all_action();

    void init_connection();

    void init_shortcut_key();
protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    // 进行界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);


private slots:

    // 进行最小化、最大化/还原、关闭操作
    void onClicked();

//    void onMenuHovered(QAction *action);

//    void showMenu();

signals:
    // 新建文本文件信号
    void new_text_file_triggered();
    // 新建文件信号
    void new_file_triggered();
    // 新建窗口信号
    void new_window_triggered();
    // 打开文件信号
    void open_file_triggered();
    // 打开文件夹信号
    void open_folder_triggered();
    // 打开最近文件信号
    void open_recent_triggered();
    // 保存信号
    void save_triggered();
    // 另存为信号
    void save_as_triggered();
    // 关闭编辑器信号
    void close_editor_triggered();
    // 关闭窗口信号
    void close_window_triggered();
    // 撤销信号
    void undo_triggered();
    // 重做信号
    void redo_triggered();
    // 剪切信号
    void cut_triggered();
    // 复制信号
    void copy_triggered();
    // 黏贴信号
    void paste_triggered();
    // 查找信号
    void find_triggered();
    // 替换信号
    void replace_triggered();
    // 全选信号
    void select_all_triggered();
    // 命令面板信号
    void command_palette_triggered();
    // 欢迎界面信号
    void welcome_triggered();
    // 展示全部指令信号
    void show_all_commands_triggered();
    // 关于界面信号
    void about_triggered();

private:

    // 最大化/还原
    void updateMaximize();


private:

    QMenuBar *m_menu;             // 菜单栏

    // 菜单栏 的 QAction
    QAction *m_file;
    QAction *m_edit;
    QAction *m_selection;
    QAction *m_view;
    QAction *m_help;

    // m_file 下属的 QAction
    QAction *m_new_text_file;
    QAction *m_new_file;
    QAction *m_new_window;
    QAction *m_open_file;
    QAction *m_open_folder;
    QAction *m_open_recent;

    QAction *m_save;
    QAction *m_save_as;
    QAction *m_close_editor;
    QAction *m_close_window;



    // m_edit 下属的 QAction
    QAction *m_undo;
    QAction *m_redo;
    QAction *m_cut;
    QAction *m_copy;
    QAction *m_paste;
    QAction *m_find;                // 内容查找
    QAction *m_replace;             // 查找并替换

    // m_selection 下属的 QAction
    QAction *m_select_all;

    // m_view 下属的 QAction
    QAction *m_command_palette;

    // m_help 下属的 QAction
    QAction *m_welcome;
    QAction *m_show_all_commands;
    QAction *m_about;


private:

    // 顶层级的 QMenu
    QMenu *m_file_menu;
    QMenu *m_edit_menu;
    QMenu *m_selection_menu;
    QMenu *m_view_menu;
    QMenu *m_help_menu;

    // 次层级的 QMenu;
    QMenu *m_open_recent_menu;

private:

    // 最右侧功能按钮
    QLabel *m_pIconLabel; //标题栏图标
    QLabel *m_pTitleLabel; //标题栏标题
    QPushButton *m_pMinimizeButton; //最小化按钮
    QPushButton *m_pMaximizeButton; //最大化/还原按钮
    QPushButton *m_pCloseButton; //关闭按钮

private:
//    QTimer *m_hover_timer;
//    QAction *m_current_hovered_action = nullptr;

    QHBoxLayout *m_layout;



};

#endif // TITLEBAR_H
