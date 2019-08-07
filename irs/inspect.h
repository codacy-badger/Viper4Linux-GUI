#ifndef INSPECT_H
#define INSPECT_H

#include <QDialog>

namespace Ui {
class inspect;
}

class inspect : public QDialog
{
    Q_OBJECT

public:
    explicit inspect(QWidget *parent = nullptr,QString path = "");
    ~inspect();
private slots:
    bool queryData();

private:
    Ui::inspect *ui;
    QString path;
};

#endif // INSPECT_H
