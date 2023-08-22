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

public:
    qcode(QWidget *parent = nullptr);
    ~qcode();

    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

private slots:

    void _new_text_file_triggered();   // 处理 新建文本文件 请求

    void _open_file_triggered();       // 处理 打开文件    请求

    void _save_triggered();            // 处理 保存文件    请求

signals:



private:
    Ui::qcode *ui;

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度

    QTextEdit *text;

    QString current_file;

    QString file_name;

};
#endif // QCODE_H
