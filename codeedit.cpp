#include "codeedit.h"
#include "highlighter.h"

#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>

CodeEdit::CodeEdit(QWidget *parent)
    : QPlainTextEdit{parent}
{

    m_line_number_widget = new LineNumber(this);


//    m_line_number_widget->setAttribute(Qt::WA_StyledBackground);


    // 不换行
//    setLineWrapMode(QPlainTextEdit::NoWrap);


    init_font();

    init_highlighter();

    init_connection();

    hitghlight_current_line();

    update_line_number_area_width();

    setLineWrapMode(QPlainTextEdit::NoWrap);
}

CodeEdit::~CodeEdit() noexcept
{
    if (m_line_number_widget) {
        delete m_line_number_widget;
        m_line_number_widget = nullptr;
    }
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_line_number_widget);

    // 绘制序列框背景色                                             最后一个值是透明度
    painter.fillRect(event->rect(), QColor(13, 13, 13 , 200));

    QTextBlock block = firstVisibleBlock();

    int block_number = block.blockNumber();

    int cursor_top = blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top();


    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();

    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        // 绘制数字
        if (cursor_top == top) {   // 位于当前行的绘制
//            painter.setPen(QColor(204, 204, 204, 240));   // 文字颜色
            painter.setPen(QColor(204, 204, 204));   // 文字颜色
            painter.drawText(0, top, get_line_number_area_width() - 3, bottom - top, Qt::AlignCenter, QString::number(block_number + 1));
        } else {  // 非当前行的绘制
//            painter.setPen(QColor(100, 100, 100));
            painter.setPen(QColor(100, 100, 100));
            painter.drawText(0, top, get_line_number_area_width() - 3, bottom - top, Qt::AlignRight, QString::number(block_number + 1));
        }

        block = block.next();

        top = bottom;

        bottom = top + (int) blockBoundingRect(block).height();
        ++block_number;
    }
}

void CodeEdit::lineNumberAreaMousePressEvent(QMouseEvent *event)
{
    QTextBlock block = document()->findBlockByLineNumber(event->position().y() / fontMetrics().height() + verticalScrollBar()->value());
    setTextCursor(QTextCursor(block));
}

void CodeEdit::lineNumberAreaWheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() != 0) {   // 垂直滚动
        verticalScrollBar()->setValue(verticalScrollBar()->value() - event->angleDelta().y() / 2);
    } else {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->angleDelta().x() / 2);
    }
//    qDebug() << event->angleDelta().y();

}

bool CodeEdit::save_file()
{
    QString file_name;
    if (m_file_name.isEmpty()) {
        file_name = QFileDialog::getSaveFileName(this, "保存文件");  // 调用对话框的时候有一些 bug, 保存不能预先自己定义
        m_file_name = file_name;
    } else {
        file_name = m_file_name;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return false;
    }
    QTextStream out(&file);

    out << toPlainText();

    file.close();

    is_save = true;
    return true;
}

bool CodeEdit::save_as_file()
{

    QString file_name = QFileDialog::getSaveFileName(this, "另存文件");
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return false;
    }
    m_file_name = file_name;
    QTextStream out(&file);
    out << toPlainText();
    file.close();

    is_save = true;
    return true;
}

void CodeEdit::set_file_name(QString file_name)
{
    m_file_name = file_name;
}

QString CodeEdit::get_file_name()
{
    return m_file_name;
}

bool CodeEdit::check_saved()
{
    return is_save;
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    m_line_number_widget->setGeometry(0, 0, get_line_number_area_width(), contentsRect().height());
}

void CodeEdit::init_font()
{
    this->setFont(QFont("Consolas", 13));
}

void CodeEdit::init_highlighter()
{
    new Highlighter{this->document()};
}

int CodeEdit::get_line_number_area_width()
{
    return 16 + QString::number(blockCount()).length() * fontMetrics().horizontalAdvance(QChar('0'));
}

void CodeEdit::init_connection()
{
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEdit::hitghlight_current_line);

    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEdit::update_line_number_area);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEdit::update_line_number_area_width);

    connect(this, &QPlainTextEdit::textChanged, this, &CodeEdit::update_save_state);
}

void CodeEdit::hitghlight_current_line()
{
    QList<QTextEdit::ExtraSelection> extra_selections;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0, 100, 100, 20));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();

    extra_selections.append(selection);
    this->setExtraSelections(extra_selections);


}

void CodeEdit::update_line_number_area(const QRect &rect, int dy)
{
    if (dy) {
        m_line_number_widget->scroll(0, dy);
    } else {
        m_line_number_widget->update(0, rect.y(), get_line_number_area_width(), rect.height());
    }
}

void CodeEdit::update_line_number_area_width()
{
    setViewportMargins(get_line_number_area_width(), 0, 0, 0);
}

void CodeEdit::update_save_state()
{
    is_save = false;
}
