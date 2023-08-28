#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>

class LineNumber;

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit();

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    void lineNumberAreaMousePressEvent(QMouseEvent *event);

    void lineNumberAreaWheelEvent(QWheelEvent *event);

    bool save_file();

    bool save_as_file();

    void set_file_name(QString file_name);

    QString get_file_name();

    bool check_saved();
protected:
    void resizeEvent(QResizeEvent *event) override;


private:
    void init_font();

    void init_highlighter();

    int get_line_number_area_width();

    void init_connection();

private slots:
    void hitghlight_current_line();

    void update_line_number_area(const QRect &, int);

    void update_line_number_area_width();

    void update_save_state();


private:
    LineNumber *m_line_number_widget;

    QString m_file_name;

    bool is_save = false;
signals:
};




class LineNumber : public QWidget
{
Q_OBJECT
public:
    explicit LineNumber(CodeEdit *editor = nullptr) : QWidget(editor) {
        m_code_editor = editor;
    }

//    ~LineNumber() {
//    delete m_code_editor;
//    }
//}
signals:

protected:
    void paintEvent(QPaintEvent *event) override {
        m_code_editor->lineNumberAreaPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        m_code_editor->lineNumberAreaMousePressEvent(event);
    }

    void wheelEvent(QWheelEvent *event) override {
        m_code_editor->lineNumberAreaWheelEvent(event);
    }
private:
    CodeEdit *m_code_editor;

};


#endif // CODEEDIT_H
