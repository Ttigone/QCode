#include "textedit.h"
#include "ui_textedit.h"


TextEdit::TextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEdit)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);
    // textedit 的滚动条与自定义的滚动条关联
    connect(ui->textEdit->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, &TextEdit::text_edit_h_scroll_bar_value_changed);

    connect(ui->horizontalScrollBar, &QAbstractSlider::valueChanged, this, &TextEdit::scroll_bar_value_changed);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &TextEdit::text_changed);

    QFont font("Consolas", 13);
    ui->textEdit->setFont(font);
    ui->textBrowser->setFont(font);

    connect(ui->textEdit->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &TextEdit::text_edit_v_scroll_bar_value_changed);

    connect(ui->textBrowser->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &TextEdit::textBrowser_v_scroll_bar_value_changed);


    Highlighter *m_highlighter = new Highlighter(ui->textEdit->document());


}

TextEdit::~TextEdit()
{
    delete ui;
}

void TextEdit::text_edit_h_scroll_bar_value_changed()
{
    // 实现滚动条的同步
    ui->horizontalScrollBar->setMaximum(ui->textEdit->horizontalScrollBar()->maximum());
    ui->horizontalScrollBar->setMinimum(ui->textEdit->horizontalScrollBar()->minimum());
    // 设置相同的步长
    ui->horizontalScrollBar->setPageStep(ui->textEdit->horizontalScrollBar()->pageStep());
    ui->horizontalScrollBar->setValue(ui->textEdit->horizontalScrollBar()->value());
}

void TextEdit::text_edit_v_scroll_bar_value_changed()
{
    ui->textBrowser->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
}

void TextEdit::scroll_bar_value_changed()
{
    ui->textEdit->horizontalScrollBar()->setValue(ui->horizontalScrollBar->value());  // 被自定义的滚动条控制

}

void TextEdit::textBrowser_v_scroll_bar_value_changed()
{
    ui->textEdit->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->value());
}

void TextEdit::text_changed()     // 有问题   点击某一行时，会将改行调至到末尾出现
{
    int line_count_of_textEdit = ui->textEdit->document()->lineCount();  // 统计行数
//    qDebug() << line_count_of_textEdit;

    QString text = ui->textBrowser->toPlainText();
    int line_count_of_textBrowser = text.trimmed().split("\n").length();
//    qDebug() << line_count_of_textBrowser;

    if (line_count_of_textBrowser == 1 && text.length() < 1) {
        text += "1\n";
    } else if (line_count_of_textBrowser > line_count_of_textEdit) {
        for (int i = line_count_of_textBrowser; i > line_count_of_textEdit; --i) {
            text.chop((QString::number(i) + "\n").length());
        }
    } else if (line_count_of_textBrowser < line_count_of_textEdit) {
        for (int i = line_count_of_textBrowser; i < line_count_of_textEdit; ++i) {
            text += QString::number(i + 1) + "\n";
        }
    }


    ui->textBrowser->setMaximumWidth(25 + QString::number(line_count_of_textEdit).length() * 7);  // 设置左侧显示行号的宽度
    ui->textBrowser->setText(text);

//    int line_count = ui->textEdit->document()->lineCount();
//    for (int i = 0; i < line_count; ++i) {
//        text += QString::number(i + 1) + "\n";    // 左侧显示行号
//    }  // 效率低下
//    ui->textBrowser->setMaximumWidth(25 + QString::number(line_count).length() * 7);  // 设置左侧显示行号的宽度
//    ui->textBrowser->setText(text);
}
