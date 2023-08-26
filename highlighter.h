#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *parent = nullptr);

protected:
    virtual void highlightBlock(const QString &text);

private:
    void add_normal_text_rule();

    void add_number_rule();

    void add_string_rule();

    void add_comment_rule();

    void add_multi_line_comment_rule(const QString &text);

    void add_keyword_rule();

private:
    struct highlight_rule  // 高亮规则
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<highlight_rule> highlight_rules;

    QString m_font_family = "Consolas";
    int m_font_size = 13;
};


#endif // HIGHLIGHTER_H
