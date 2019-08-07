#ifndef TRACK_H
#define TRACK_H

#include "qcustomplot.h"
#include <QAudioBuffer>

class QAudioDecoder;

class Track : public QCustomPlot
{
    Q_OBJECT

public:
    Track(QWidget *parent = Q_NULLPTR);
    ~Track();
    void setSource(const QString &fileName);
    QStringList getData();

public slots:
    void setBuffer();
    void plot();
signals:
    void ready();

private:
    qreal getPeakValue(const QAudioFormat& format);

    QAudioDecoder *decoder;
    QAudioBuffer buffer;
    QVector<double> samples;
    QCPGraph *wavePlot;
};
#endif // TRACK_H
