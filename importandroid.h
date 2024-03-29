#ifndef IMPORTANDROID_H
#define IMPORTANDROID_H

#include <QDialog>

namespace Ui {
class importandroid;
}

class importandroid : public QDialog
{
    Q_OBJECT

public:
    explicit importandroid(QWidget *parent = nullptr);
    ~importandroid();
private slots:
    void import();
private:
    Ui::importandroid *ui;
    QString pathAppend(const QString& path1, const QString& path2);
};

#endif // IMPORTANDROID_H
