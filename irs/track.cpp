#include "track.h"

#include <QAudioDecoder>

Track::Track(QWidget *parent)
    : QCustomPlot(parent)
    , decoder(new QAudioDecoder(this))
{
    wavePlot = addGraph();
    setMinimumHeight(50);
    setInteraction(QCP::iRangeDrag, true);
    setInteraction(QCP::Interaction::iRangeZoom, true);
    connect(decoder, SIGNAL(bufferReady()), this, SLOT(setBuffer()));
    connect(decoder, SIGNAL(finished()), this, SLOT(plot()));
}

Track::~Track()
{
    delete decoder;
    // wavePlot delete auto ?
}

void Track::setSource(const QString &fileName)
{
    samples.clear();
    clearItems();
    clearGraphs();
    buffer = *new QAudioBuffer();

    wavePlot = addGraph();

    decoder->setSourceFilename(fileName);
    decoder->start();
}

void Track::setBuffer()
{
    buffer = decoder->read();
    qreal peak = getPeakValue(buffer.format());
    const qint16 *data = buffer.constData<qint16>();
    float *fdata = buffer.data<float>();
    int count = buffer.sampleCount() / 1;//2;
    for (int i=0; i<count; i++){
        double val;
        if(peak==1.00003)val = fdata[i]/peak;
        else val = data[i]/peak;
        samples.append(val);
    }
}

void Track::plot()
{
    QVector<double> x(samples.size());
    for (int i=0; i<x.size(); i++)
        x[i] = i;
    wavePlot->addData(x, samples);
    yAxis->setRange(QCPRange(-1, 1));
    xAxis->setRange(QCPRange(0, samples.size()));
    replot();
    emit ready();
}

QStringList Track::getData(){
    if(buffer.byteCount()<1)return QStringList();
    QStringList* list = new QStringList();
    QAudioFormat f = buffer.format();
    if(!f.isValid())return QStringList();
    list->append(f.codec());
    if(f.byteOrder()==QAudioFormat::BigEndian)
        list->append("Big Endian");
    else
        list->append("Little Endian");

    if(f.sampleType()==QAudioFormat::SampleType::Float)
        list->append("Float");
    else if(f.sampleType()==QAudioFormat::SampleType::SignedInt)
        list->append("Signed Int");
    else if(f.sampleType()==QAudioFormat::SampleType::UnSignedInt)
        list->append("Unsigned Int");
    else
        list->append("Unknown");
    list->append(QString::number(f.sampleRate()));
    list->append(QString::number(f.sampleSize()));



    list->append(QString::number(f.channelCount()));
    list->append(QString::number(f.bytesPerFrame()));
    return *list;
}
/**
 * https://stackoverflow.com/questions/46947668/draw-waveform-from-raw-data-using-qaudioprobe
 * @brief Track::getPeakValue
 * @param format
 * @return The peak value
 */
qreal Track::getPeakValue(const QAudioFormat &format)
{
    qreal ret(0);
    if (format.isValid()){
        switch (format.sampleType()) {
            case QAudioFormat::Unknown:
            break;
            case QAudioFormat::Float:
                if (format.sampleSize() != 32) // other sample formats are not supported
                    ret = 0;
                else
                    ret =1.00003;
            break;
            case QAudioFormat::SignedInt:
                if (format.sampleSize() == 32)
#ifdef Q_OS_WIN
                    ret = INT_MAX;
#endif
#ifdef Q_OS_UNIX
                    ret = SHRT_MAX;
#endif
                else if (format.sampleSize() == 24){
                    QCPItemText *textLabel = new QCPItemText(this);
                    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
                    textLabel->position->setCoords(0.5, 0.1); // place position at center/top of axis rect
                    textLabel->setText("24-bit data not yet supported");
                    textLabel->setFont(QFont(font().family(), 11)); // make font a bit larger
                    textLabel->setPen(QPen(Qt::black)); // show black border around text
                    ret = 0;
                }
                else if (format.sampleSize() == 16)
                    ret = SHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = CHAR_MAX;
                break;
            case QAudioFormat::UnSignedInt:
                if (format.sampleSize() == 32)
                    ret = UINT_MAX;
                else if (format.sampleSize() == 24){
                    QCPItemText *textLabel = new QCPItemText(this);
                    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
                    textLabel->position->setCoords(2, 0); // place position at center/top of axis rect
                    textLabel->setText("24-bit data not yet supported");
                    textLabel->setFont(QFont(font().family(), 1)); // make font a bit larger
                    textLabel->setPen(QPen(Qt::black)); // show black border around text
                    ret = 0
;
                }
                else if (format.sampleSize() == 16)
                    ret = USHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = UCHAR_MAX;
            break;
        default:
            break;
        }
    }
    return ret;
}
