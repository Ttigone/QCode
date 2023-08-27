#ifndef TEXTEDIT_H
#define TEXTEDIT_H


/*
 * 已被弃用
 *
 */

#include <QWidget>
#include "highlighter.h"

namespace Ui {
class TextEdit;
}

class TextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent = nullptr);
    ~TextEdit();

private slots:
    void text_edit_h_scroll_bar_value_changed();

    void text_edit_v_scroll_bar_value_changed();

    void scroll_bar_value_changed();

    void textBrowser_v_scroll_bar_value_changed();

    void text_changed();
private:
    Ui::TextEdit *ui;
    QFont m_font;

    void init_hightlight();

    void hitghlight_current_line();
};

#endif // TEXTEDIT_H
