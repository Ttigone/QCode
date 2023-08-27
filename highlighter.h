#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *parent = nullptr, QString font_family = "Consolas", int font_size = 13);

protected:
    virtual void highlightBlock(const QString &text);

private:
    void add_normal_text_rule();

    void add_number_rule();

    void add_string_rule();

    void add_comment_rule();

    void add_multi_line_comment_rule(const QString &text);

    void add_keyword_rule();

    void add_classname_rule();

    void add_funtion_rule();

private:
    struct highlight_rule  // 高亮规则
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<highlight_rule> highlight_rules;

    QString m_font_family;
    int m_font_size;

};


#endif // HIGHLIGHTER_H
