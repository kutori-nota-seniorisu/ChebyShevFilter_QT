#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QtCharts>

#include <vector>
#include <complex>

#include "FFT.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void InitDraw();

private slots:
	void on_pb_draw_clicked();

	void on_pb_fft_clicked();

	void on_pb_lpf_clicked();

	void on_pb_hpf_clicked();

	void on_pb_bpf_clicked();

	void on_pb_filter_fft_clicked();

	void on_pb_nf_clicked();

private:
	Ui::MainWindow *ui;

	// 原信号时域图
	QChart *m_chart_plot;
	QValueAxis *m_axisX1;
	QValueAxis *m_axisY1;
	QSplineSeries *m_series1;
	// 原信号频域图
	QChart *m_chart_fft;
	QValueAxis *m_axisX2;
	QValueAxis *m_axisY2;
	QSplineSeries *m_series2;
	// 滤波后信号时域图
	QChart *m_chart_plot2;
	QValueAxis *m_axisX12;
	QValueAxis *m_axisY12;
	QSplineSeries *m_series12;
	// 滤波后信号频域图
	QChart *m_chart_fft2;
	QValueAxis *m_axisX22;
	QValueAxis *m_axisY22;
	QSplineSeries *m_series22;

	QVector<QPointF> data;
	QVector<QPointF> data_filter;

	int N;
	double fs;
	double Ts;
};
#endif // MAINWINDOW_H
