#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QKeySequence>
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

    void onMenuHovered(QAction *action);

    void showMenu();

private slots:

signals:

    void new_text_file_triggered();

    void open_file_triggered();

    void save_triggered();

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

    // 最右侧功能按钮
    QLabel *m_pIconLabel; //标题栏图标
    QLabel *m_pTitleLabel; //标题栏标题
    QPushButton *m_pMinimizeButton; //最小化按钮
    QPushButton *m_pMaximizeButton; //最大化/还原按钮
    QPushButton *m_pCloseButton; //关闭按钮


private:
    QTimer *m_hover_timer;
    QAction *m_current_hovered_action = nullptr;

private:


};

#endif // TITLEBAR_H
