#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>

#include "titleBar.h"

//调用WIN API需要用到的头文件与库
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif



titleBar::titleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(23);  // 高度设定固定大小

    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);

    m_menu = new QMenuBar(this);   // 菜单栏

    m_file = new QAction("File", this);
    // 创建专属菜单
    QMenu *file_menu = new QMenu(this);
    // 设置专属菜单
    m_file->setMenu(file_menu);
    // 新建 第一部分 QAction
    m_new_text_file = new QAction("New Text File", this);
    m_new_file = new QAction("New File...", this);
    m_new_window = new QAction("New Window", this);
    // 添加 第一部分 QAction
    file_menu->addAction(m_new_text_file);
    file_menu->addAction(m_new_file);
    file_menu->addAction(m_new_window);

    // 添加分割符
    file_menu->addSeparator();



    // 新建 第二部分 QAction
    m_open_file = new QAction("Open File...", this);
    m_open_folder = new QAction("Open folder...", this);
    m_open_recent = new QAction("Open Recent", this);   // 这个还得再设置一个 QMenu
    // 添加 第二部分功能 QAction
    file_menu->addAction(m_open_file);
    file_menu->addAction(m_open_folder);
    file_menu->addAction(m_open_recent);

    // 添加分隔符
    file_menu->addSeparator();

    // 新建 第三部分 QAction
    m_save = new QAction("Save", this);
    m_save_as = new QAction("Save As...", this);
    // 添加 第三部分 QAction
    file_menu->addAction(m_save);
    file_menu->addAction(m_save_as);

    // 添加分隔符
    file_menu->addSeparator();

    // 新建 第四部分 QAction
    m_close_editor = new QAction("Close Editor", this);
    m_close_window = new QAction("Close Window", this);
    // 添加 第四部分功能 QAction
    file_menu->addAction(m_close_editor);
    file_menu->addAction(m_close_window);

    m_edit = new QAction("Edit", this);
    QMenu *edit_menu = new QMenu(this);
    m_edit->setMenu(edit_menu);
    m_undo = new QAction("Uodo", this);
    m_redo = new QAction("Redo", this);
    edit_menu->addAction(m_undo);
    edit_menu->addAction(m_redo);
    edit_menu->addSeparator();
    m_cut = new QAction("Cut", this);
    m_copy = new QAction("Copy", this);
    m_paste = new QAction("Paste", this);
    edit_menu->addAction(m_cut);
    edit_menu->addAction(m_copy);
    edit_menu->addAction(m_paste);
    edit_menu->addSeparator();
    m_find = new QAction("Find", this);
    m_replace = new QAction("Replace", this);
    edit_menu->addAction(m_find);
    edit_menu->addAction(m_replace);




    m_selection = new QAction("Selection", this);
    QMenu *selection_menu = new QMenu(this);
    m_selection->setMenu(selection_menu);
    m_select_all = new QAction("Select All");
    selection_menu->addAction(m_select_all);

    m_view = new QAction("View", this);
    QMenu *view_menu = new QMenu(this);
    m_view->setMenu(view_menu);
    m_command_palette = new QAction("Command Palette...", this);
    view_menu->addAction(m_command_palette);

    m_help = new QAction("Help", this);
    QMenu *help_menu = new QMenu(this);
    m_help->setMenu(help_menu);
    m_welcome = new QAction("Welcome", this);
    m_show_all_commands = new QAction("Show All Commands", this);
    help_menu->addAction(m_welcome);
    help_menu->addAction(m_show_all_commands);
    help_menu->addSeparator();
    m_about = new QAction("About", this);
    help_menu->addAction(m_about);

    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pCloseButton = new QPushButton(this);

    //初始化图标Label
    m_pIconLabel->setFixedSize(20, 20);
    m_pIconLabel->setScaledContents(true);
    QPixmap pixmap(":/images/qc.png");
    m_pIconLabel->setPixmap(pixmap);

    // 设置文本
    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 设置菜单栏部件的名称
    m_file->setObjectName("File");
    m_edit->setObjectName("Edit");
    m_selection->setObjectName("Selection");
    m_view->setObjectName("View");
    m_help->setObjectName("Help");

    // 为菜单栏添加 QAction 控件
    m_menu->addAction(m_file);
    m_menu->addAction(m_edit);
    m_menu->addAction(m_selection);
    m_menu->addAction(m_view);
    m_menu->addAction(m_help);


    //设置按钮的固定大小、图片、取消边框
    m_pMinimizeButton->setIconSize(QSize(30,22));
    m_pMinimizeButton->setIcon(QIcon(":/images/minimize.png"));
    m_pMinimizeButton->setFlat(true);

    m_pMaximizeButton->setIconSize(QSize(27,22));
    m_pMaximizeButton->setIcon(QIcon(":/images/maximize.png"));
    m_pMaximizeButton->setFlat(true);

    m_pCloseButton->setIconSize(QSize(27,22));
    m_pCloseButton->setIcon(QIcon(":/images/close.png"));
    m_pCloseButton->setFlat(true);

    //设置窗口部件的名称
    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");


    //给按钮设置静态tooltip，当鼠标移上去时显示tooltip
    m_pMinimizeButton->setToolTip("Minimize");
    m_pMaximizeButton->setToolTip("Maximize");
    m_pCloseButton->setToolTip("Close");


//    m_menu->setFixedHeight(30);
    m_menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //标题栏布局
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pIconLabel);
    pLayout->setSpacing(5);             // 间隙
//    pLayout->addWidget(m_pTitleLabel);

    pLayout->setSpacing(5);             // 间隙

    pLayout->addWidget(m_menu);         // 添加菜单栏

//    pLayout->setSpacing(5);             // 间隙

    pLayout->addStretch(1);             // 增加一个弹簧

    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);

    this->setLayout(pLayout);


    //连接三个按钮的信号槽
    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    m_hover_timer = new QTimer(this);

//    connect(m_menu, &QMenuBar::hovered, this, &titleBar::onMenuHovered);   // 移动到 menubar 的时候按一定事件显示对应的 QMenu

    connect(m_hover_timer, &QTimer::timeout, this, &titleBar::showMenu);
    m_hover_timer->setSingleShot(true);


    // BUG 连续点击时会导致程序崩溃  // 正常也会
    connect(file_menu, &QMenu::aboutToShow, m_hover_timer, &QTimer::stop);  // 解决问题？
    connect(edit_menu, &QMenu::aboutToHide, m_hover_timer, &QTimer::stop);
    connect(selection_menu, &QMenu::aboutToShow, m_hover_timer, &QTimer::stop);
    connect(view_menu, &QMenu::aboutToHide, m_hover_timer, &QTimer::stop);
    connect(help_menu, &QMenu::aboutToHide, m_hover_timer, &QTimer::stop);

    // 绑定快捷键
    m_save->setShortcut(QKeySequence(tr("Ctrl+s")));
    connect(m_save, &QAction::triggered, this, [&](){
        qDebug() << "sa";
        m_hover_timer->stop();
    });


    connect(m_new_text_file, &QAction::triggered, this, &titleBar::new_text_file_triggered);
    connect(m_open_file, &QAction::triggered, this, &titleBar::open_file_triggered);
    connect(m_save, &QAction::triggered, this, &titleBar::save_triggered);

}

titleBar::~titleBar()
{

}


// BUG 在最大化时，点击最小化，再点击恢复按钮，再点击最大化按钮时出现无响应，显示 resotre, 点击 2 次恢复

//双击标题栏进行界面的最大化/还原
void titleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event); //没有实质性的作用，只是用来允许event可以不使用，用来避免编译器警告

    emit m_pMaximizeButton->clicked();
}

//进行界面的拖动  [一般情况下，是界面随着标题栏的移动而移动，所以我们将事件写在标题栏中比较合理]
void titleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isWindow())
        {
            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
#else
#endif
}

//使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
bool titleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch ( event->type() ) //判断发生事件的类型
    {
    case QEvent::WindowTitleChange: //窗口标题改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj); //获得发生事件的窗口对象
        if (pWidget)
        {
            //窗体标题改变，则标题栏标题也随之改变
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    break;

    case QEvent::WindowIconChange: //窗口图标改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            //窗体图标改变，则标题栏图标也随之改变
            QIcon icon = pWidget->windowIcon();
            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    break;

    case QEvent::Resize:
        updateMaximize(); //最大化/还原
        return true;

    default:
        return QWidget::eventFilter(obj, event);
    }

    return QWidget::eventFilter(obj, event);
}

//进行最小化、最大化/还原、关闭操作
void titleBar::onClicked()
{
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());

    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isWindow())
    {
        //判断发送信号的对象使哪个按钮
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized(); //窗口最小化显示
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();  //窗口最大化/还原显示
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close(); //窗口关闭
        }
    }
}

void titleBar::onMenuHovered(QAction *action)
{
    m_current_hovered_action = action;
    m_hover_timer->start(500);               // 延迟 500 毫秒
}

void titleBar::showMenu()
{
    if (m_current_hovered_action && !m_hover_timer->isActive()) {
        m_current_hovered_action->menu()->popup(QCursor::pos());
    }
}



//最大化/还原
void titleBar::updateMaximize()
{
    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isWindow())
    {
        bool bMaximize = pWindow->isMaximized(); //判断窗口是不是最大化状态，是则返回true，否则返回false
        if (bMaximize)
        {
            //目前窗口是最大化状态，则最大化/还原的toolTip设置为"Restore"
            m_pMaximizeButton->setToolTip(tr("Restore"));
            //设置按钮的属性名为"maximizeProperty"
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            //目前窗口是还原状态，则最大化/还原的toolTip设置为"Maximize"
            m_pMaximizeButton->setToolTip(tr("Maximize"));
            //设置按钮的属性名为"maximizeProperty"
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
        }

        m_pMaximizeButton->setStyle(QApplication::style());
    }
}
