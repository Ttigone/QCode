#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>
#include <QMenuBar>
#include <QPushButton>

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

private:

    // 最大化/还原
    void updateMaximize();


private:

    QMenuBar *m_menu;             // 菜单栏

    //QAction
    QAction *m_file;
    QAction *m_edit;
    QAction *m_selection;
    QAction *m_view;
    QAction *m_help;

    QAction *new_text_file;
    QAction *new_file;
    QAction *new_window;
    QAction *open_file;
    QAction *open_folder;
    QAction *open_recent;
    QAction *save;
    QAction *save_as;
    QAction *close_editor;
    QAction *close_window;

    QAction *undo;
    QAction *redo;
    QAction *cut;
    QAction *copy;
    QAction *paste;
    QAction *find;                // 内容查找
    QAction *replace;             // 查找并替换


private:

    // 最右侧功能按钮
    QLabel *m_pIconLabel; //标题栏图标
    QLabel *m_pTitleLabel; //标题栏标题
    QPushButton *m_pMinimizeButton; //最小化按钮
    QPushButton *m_pMaximizeButton; //最大化/还原按钮
    QPushButton *m_pCloseButton; //关闭按钮
};

#endif // TITLEBAR_H
