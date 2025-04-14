#include "command.h"
#include <QDebug>

addCommand::addCommand(int *value, QUndoCommand *parent)
{
    /* 使用Q_UNUSED,避免未使用的数据类型 */
    Q_UNUSED(parent);

    /* undoView显示的操作信息 */
    setText("进行了加1操作");

    /* value的地址赋值给new_count */
    new_count = value;

    /* 让构造函数传过来的*new_count的值赋值给old_count */
    old_count = *new_count;
}

/* 执行stack push时或者重做操作时会自动调用 */
void addCommand::redo()
{
    /* 重新赋值给new_count */
    *new_count = old_count;

    /* 打印出*new_count的值 */
    qDebug()<<"redo:"<<*new_count<<endl;
}

/* 回撤操作时执行 */
void addCommand::undo()
{
    /* 回撤操作每次应减一 */
    (*new_count)--;

    /* 打印出*new_count的值 */
    qDebug()<<"undo:"<<*new_count<<endl;
}

addCommand::~addCommand()
{

}


