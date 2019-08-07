#include "inspect.h"
#include "ui_inspect.h"

inspect::inspect(QWidget *parent,QString _path) :
    QDialog(parent),
    ui(new Ui::inspect)
{
    ui->setupUi(this);

    path = _path;
    ui->widget->setSource(path);
    connect(ui->widget,SIGNAL(ready()),SLOT(queryData()));
}

inspect::~inspect()
{
    delete ui;
}

bool inspect::queryData(){
    ui->dataView->clear();
    QStringList list = ui->widget->getData();
    if(list.count()<1){
        ui->dataView->setText("Unable to load details");
        return false;
    }
    for (int i=0;i<list.count();i++) {
        QString prefix = "";
        switch(i){
            case 0:
                prefix = "Codec: ";
            break;
            case 1:
                prefix = "Byteorder: ";
            break;
            case 2:
                prefix = "Sampletype: ";
            break;
            case 3:
                prefix = "Samplerate: ";
            break;
            case 4:
                prefix = "Samplesize: ";
            break;
            case 5:
                prefix = "Channels: ";
            break;
            case 6:
                prefix = "Bytes per frame: ";
            break;
        }
        ui->dataView->append(prefix + list[i]);
    }
    return true;
}
