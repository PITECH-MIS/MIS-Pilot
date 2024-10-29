#ifndef DYNAMICCHART_H
#define DYNAMICCHART_H

#include <QChartView>
#include <QObject>
#include <QWidget>
#include <QValueAxis>
#include <QTimer>
#include <QLineSeries>
#include <QSplineSeries>
#include <QHash>
#include <QPointF>
#include <QGraphicsLayout>

class DynamicChart : public QChartView
{
    Q_OBJECT
public:
    DynamicChart(QWidget *parent);
    void createSeries(int seriesId, const QString &name);
    void addPoint(int seriesId, const QPointF &point);
private:
    QChart* m_chart;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QTimer* timer;
    QHash<int, QLineSeries*> seriesMap;
    QPointF lastPoint;
    QPointF newPoint;
    int currentSeriesId = 0;
    int currentStep = 0;
    int stepsCount = 0;
    void animateDrawing();
};

#endif // DYNAMICCHART_H
