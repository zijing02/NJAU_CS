#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QObject>

class addCommand : public QUndoCommand
{
public:
    addCommand(int *value, QUndoCommand* parent = 0);
    ~addCommand();

    /* 重写重做与撤回方法 */
    void redo() override;
    void undo() override;

private:
    /* 新的count */
    int *new_count;

    /* 旧的count */
    int old_count;
};

#endif // COMMAND_H
