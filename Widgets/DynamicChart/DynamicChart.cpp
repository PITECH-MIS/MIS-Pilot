#include "DynamicChart.h"
#include "../../utils.h"

DynamicChart::DynamicChart(QWidget *parent)
    : QChartView(new QChart(), parent), m_chart(this->chart())
{
    // m_chart->setTitle("Dynamic Data Plot");
    m_chart->legend()->hide();
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    setRenderHint(QPainter::Antialiasing);

    m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);

    axisX = new QValueAxis();
    axisX->setRange(0, 100);
    m_chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &DynamicChart::animateDrawing);
    timer->setInterval(30);
}

void DynamicChart::setTitle(QString title)
{
    m_chart->setTitle(title);
}

void DynamicChart::setVerticalRange(float lower, float upper)
{
    axisY->setRange(lower, upper);
}

void DynamicChart::createSeries(int seriesId, const QString &name)
{
    QLineSeries *series = new QLineSeries();
    series->setName(name);
    m_chart->addSeries(series);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    seriesMap.insert(seriesId, series);
}

void DynamicChart::addPoint(int seriesId, const QPointF &point)
{
    if (!seriesMap.contains(seriesId)) return;
    QLineSeries *series = seriesMap[seriesId];

    if (!series->points().isEmpty())
    {
        lastPoint = series->points().last();
    }
    else
    {
        lastPoint = point;
        series->append(point);
    }
    newPoint = point;
    currentSeriesId = seriesId;
    currentStep = 0;
    stepsCount = 10; // 分10步完成连线
    timer->start();
}

void DynamicChart::animateDrawing()
{
    QLineSeries *series = seriesMap[currentSeriesId];
    if (currentStep >= stepsCount)
    {
        timer->stop();
        series->append(newPoint);
        return;
    }
    qreal x = lastPoint.x() + (newPoint.x() - lastPoint.x()) * currentStep / stepsCount;
    qreal y = lastPoint.y() + (newPoint.y() - lastPoint.y()) * currentStep / stepsCount;
    series->append(x, y);

    // 动态更新坐标轴
    if (x > axisX->max())
    {
        auto window = x - axisX->max();
        axisX->setMax(x);
        axisX->setMin(axisX->min() + window);
    }
    if (y > axisY->max() || y < axisY->min())
    {
        axisY->setMax(qMax(1.5f * y, axisY->max()));
        axisY->setMin(qMin(1.5f * y, axisY->min()));
    }

    currentStep++;
}
