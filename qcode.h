#ifndef QCODE_H
#define QCODE_H

#include <QWidget>

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

private:
    Ui::qcode *ui;

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度
};
#endif // QCODE_H
