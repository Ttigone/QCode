#include "highlighter.h"

#include <QFile>

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    // 普通文本
    add_normal_text_rule();
    // 数字
    add_number_rule();
    // 字符串
    add_string_rule();
    // 单行注释
    add_comment_rule();
    // 关键字
    add_keyword_rule();
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const highlight_rule &rule, highlight_rules) {
        QRegularExpression reg_exp(rule.pattern);
        int  index = reg_exp.match(text).capturedStart();
        while (index >= 0) {
            int length = reg_exp.match(text).capturedLength();
            setFormat(index, length, rule.format);
            index = reg_exp.match(text, index + length).capturedStart();
        }
    }

    // 单独实现多行注释
    add_multi_line_comment_rule(text);
}

void Highlighter::add_normal_text_rule()
{
    highlight_rule rule;
    rule.pattern = QRegularExpression("[a-z0-9A-Z]+");

    QTextCharFormat normal_text_format;

    normal_text_format.setFont(QFont("Consolas", 13));
    normal_text_format.setForeground(QColor(0, 100, 100));

    rule.format = normal_text_format;

    highlight_rules.append(rule);

}

void Highlighter::add_number_rule()
{
    highlight_rule rule;
    rule.pattern = QRegularExpression("\\b\\d+|\\d+\\.\\d+\\b");

    QTextCharFormat number_format;

    number_format.setFont(QFont("Consolas", 13));
    number_format.setForeground(QColor(200, 0, 0));

    rule.format = number_format;

    highlight_rules.append(rule);
}

void Highlighter::add_string_rule()
{
    QTextCharFormat number_format;
    number_format.setFont(QFont("Consolas", 13));
    number_format.setForeground(QColor(0, 180, 180));

    highlight_rule rule;

    rule.format = number_format;

    // 单引号
    rule.pattern = QRegularExpression("'[^']*'");
    highlight_rules.append(rule);

    // 双引号
    rule.pattern = QRegularExpression("\"[^\"]*\"");
    highlight_rules.append(rule);

    // 反引号
    rule.pattern = QRegularExpression("`[^`]*`");
    highlight_rules.append(rule);

}

void Highlighter::add_comment_rule()
{
    QTextCharFormat comment_format;
    comment_format.setFont(QFont(m_font_family, m_font_size));
    comment_format.setForeground(Qt::darkGreen);

    highlight_rule rule;

    rule.format = comment_format;

    // 单行注释
    rule.pattern = QRegularExpression("\\/\\/.*$");
    highlight_rules.append(rule);

    // 多行注释
    rule.pattern = QRegularExpression("\\/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/");
    highlight_rules.append(rule);
}

void Highlighter::add_multi_line_comment_rule(const QString &text)
{
    setCurrentBlockState(0);

    // /*
    QRegularExpression comment_start_expression("\\/\\*");
    // */
    QRegularExpression comment_end_expression("\\*\\/");

    QTextCharFormat multi_line_comment_format;
    multi_line_comment_format.setFont(QFont(m_font_family, m_font_size));
    multi_line_comment_format.setForeground(Qt::darkGreen);
//    int start_index = comment_start_expression.match(text).capturedStart();  // 没有捕获到则返回 -1
    int start_index = 0;
    if (previousBlockState() != 1) {
        start_index = comment_start_expression.match(text).capturedStart();
    }
    while (start_index >= 0) {
        int end_index = comment_end_expression.match(text, start_index).capturedStart();
//        int end_index = comment_end_expression.match(text ).capturedStart();
        int comment_length = 0;
        if (end_index == -1) { // 多行注释
            setCurrentBlockState(1);
            comment_length = text.length() - start_index;
        } else { // 单行注释
            comment_length = end_index - start_index + comment_end_expression.match(text).capturedLength();
        }

        setFormat(start_index,
                  comment_length,
                  multi_line_comment_format);
        start_index = comment_start_expression.match(text, comment_length + start_index).capturedStart();
    }
}

void Highlighter::add_keyword_rule()
{
    QFile file(":/config/keywords.txt");
    QTextStream keywords_stream(&file);

    highlight_rule rule;
    QTextCharFormat keyword_format;
    keyword_format.setFont(QFont(m_font_family, m_font_size));
    keyword_format.setForeground(Qt::darkMagenta);
    rule.format = keyword_format;

    if (file.open(QIODevice::ReadOnly)) {
        keywords_stream.seek(0);
        QString line;
        while (!keywords_stream.atEnd()) {
            line = keywords_stream.readLine();
            rule.pattern = QRegularExpression("\\b" + line + "\\b");
            highlight_rules.append(rule);
        }
        file.close();
    }
}
