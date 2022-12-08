#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spuce/filters/iir_coeff.h"
#include "spuce/filters/chebyshev_iir.h"
#include "spuce/filters/iir_df.h"
#include "spuce/filters/notch_allpass.h"

using namespace spuce;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	InitDraw();

	// 采样频率
	fs = 1000;
	// 采样周期
	Ts = 1 / fs;
	// 采样点数
	N = 2000;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::InitDraw()
{
	// 原信号时域图
	m_chart_plot = new QChart();
	m_axisX1 = new QValueAxis();
	m_axisY1 = new QValueAxis();
	m_series1 = new QSplineSeries();
	ui->wt_plot->setChart(m_chart_plot);
	m_chart_plot->legend()->hide();
	m_chart_plot->addAxis(m_axisX1, Qt::AlignBottom);
	m_chart_plot->addAxis(m_axisY1, Qt::AlignLeft);

	m_axisX1->setRange(0, 1);
	m_axisY1->setRange(0, 1);
	m_chart_plot->addSeries(m_series1);
	m_series1->attachAxis(m_axisX1);
	m_series1->attachAxis(m_axisY1);

	m_chart_plot->setTitleBrush(QBrush(QColor(0, 0, 0)));
	m_chart_plot->setTitle("plot f(t)-t s");

	// 原信号频域图
	m_chart_fft = new QChart();
	m_axisX2 = new QValueAxis();
	m_axisY2 = new QValueAxis();
	m_series2 = new QSplineSeries();
	ui->wt_fft->setChart(m_chart_fft);
	m_chart_fft->legend()->hide();
	m_chart_fft->addAxis(m_axisX2, Qt::AlignBottom);
	m_chart_fft->addAxis(m_axisY2, Qt::AlignLeft);

	m_axisX2->setRange(0, 1);
	m_axisY2->setRange(0, 1);
	m_chart_fft->addSeries(m_series2);
	m_series2->attachAxis(m_axisX2);
	m_series2->attachAxis(m_axisY2);

	m_chart_fft->setTitleBrush(QBrush(QColor(0, 0, 0)));
	m_chart_fft->setTitle("fft F(f)-f Hz");

	// 滤波后信号时域图
	m_chart_plot2 = new QChart();
	m_axisX12 = new QValueAxis();
	m_axisY12 = new QValueAxis();
	m_series12 = new QSplineSeries();
	ui->wt_plot2->setChart(m_chart_plot2);
	m_chart_plot2->legend()->hide();
	m_chart_plot2->addAxis(m_axisX12, Qt::AlignBottom);
	m_chart_plot2->addAxis(m_axisY12, Qt::AlignLeft);

	m_axisX12->setRange(0, 1);
	m_axisY12->setRange(0, 1);
	m_chart_plot2->addSeries(m_series12);
	m_series12->attachAxis(m_axisX12);
	m_series12->attachAxis(m_axisY12);

	m_chart_plot2->setTitleBrush(QBrush(QColor(0, 0, 0)));
	m_chart_plot2->setTitle("plot f(t)-t s");

	// 滤波后信号频域图
	m_chart_fft2 = new QChart();
	m_axisX22 = new QValueAxis();
	m_axisY22 = new QValueAxis();
	m_series22 = new QSplineSeries();
	ui->wt_fft2->setChart(m_chart_fft2);
	m_chart_fft2->legend()->hide();
	m_chart_fft2->addAxis(m_axisX22, Qt::AlignBottom);
	m_chart_fft2->addAxis(m_axisY22, Qt::AlignLeft);

	m_axisX22->setRange(0, 1);
	m_axisY22->setRange(0, 1);
	m_chart_fft2->addSeries(m_series22);
	m_series22->attachAxis(m_axisX22);
	m_series22->attachAxis(m_axisY22);

	m_chart_fft2->setTitleBrush(QBrush(QColor(0, 0, 0)));
	m_chart_fft2->setTitle("fft F(f)-f Hz");
}

void MainWindow::on_pb_draw_clicked()
{
	// 生成正弦信号，频率为50Hz，100Hz，200Hz，400Hz
	double f1 = 50;
	double f2 = 100;
	double f3 = 200;
	double f4 = 400;
	for (int i = 0; i < N; ++i)
	{
		double t = i * Ts;
		double x1 = 2 * 0.5 * qSin(2 * M_PI * f1 * t);
		double x2 = 2 * 0.2 * qSin(2 * M_PI * f2 * t);
		double x3 = 2 * 0.3 * qSin(2 * M_PI * f3 * t);
		double x4 = 2 * 0.6 * qSin(2 * M_PI * f4 * t);
		double y = x1 + x2 + x3 + x4;
		data.append(QPointF(t, y));
	}
	m_axisX1->setRange(0, N * Ts);
	m_axisY1->setRange(-2, 2);
	m_series1->replace(data);
}

void MainWindow::on_pb_fft_clicked()
{
	// 输入数据
	std::vector<double> x(N);
	for (int i = 0; i < N; ++i)
		x[i] = data[i].y();

	// 傅里叶变换
	std::vector<std::complex<double>> y(N);
	y = DFT(x);

	QVector<QPointF> d;
	// 频率间隔
	double f0 = fs / N;

	// 输出数据，单边谱，将幅值与正弦函数幅值等价
	// 双边转单边需要将幅值乘以2
	for (int i = 0; i < N / 2; ++i)
	{
		double f = f0 * i;
		double y_m = abs(y[i]) / N * 2;
		d.append(QPointF(f, y_m));
	}
	// 设置坐标轴范围
	m_axisX2->setRange(0, fs / 2);
	m_axisX2->setTickCount(11);

	m_axisY2->setRange(0, 1.2);
	m_axisY2->setTickCount(7);
	m_series2->replace(d);
}

void MainWindow::on_pb_lpf_clicked()
{
	data_filter.clear();
	// 滤波器阶数
	long O = 6;
	// 截止频率，单位为kHz
	float_type f_cutoff = 0.1;
	// 通带波纹
	float_type ripple = 0.5;

	iir_coeff BPF(O);
	chebyshev_iir(BPF, f_cutoff, ripple);

	iir_df<float_type> LPF(BPF);

	for (int i = 0; i < N; ++i)
	{
		double t = i * Ts;
		double y = LPF.clock(data[i].y());
		data_filter.append(QPointF(t, y));
	}

	m_axisX12->setRange(0, N * Ts);
	m_axisY12->setRange(-2, 2);
	m_series12->replace(data_filter);
}

void MainWindow::on_pb_hpf_clicked()
{
	data_filter.clear();
	// 滤波器阶数
	long O = 6;
	// 截止频率，单位为kHz
	float_type f_cutoff = 0.2;
	// 通带波纹
	float_type ripple = 0.5;

	iir_coeff BPF(O, filter_type::high);
	chebyshev_iir(BPF, f_cutoff, ripple);

	iir_df<float_type> LPF(BPF);

	for (int i = 0; i < N; ++i)
	{
		double t = i * Ts;
		double y = LPF.clock(data[i].y());
		data_filter.append(QPointF(t, y));
	}

	m_axisX12->setRange(0, N * Ts);
	m_axisY12->setRange(-2, 2);
	m_series12->replace(data_filter);
}

void MainWindow::on_pb_bpf_clicked()
{
	data_filter.clear();

	// 滤波器阶数
	long O = 6;
	// 截止频率，单位为kHz
	float_type f_cutoff = 0.25;
	// 通带波纹
	float_type ripple = 0.5;
	float_type f_center = sqrt(0.2 * 0.4);

	iir_coeff BPF(O, filter_type::bandpass);
	// 我认为此带通滤波器的参数f_cutoff表征了滤波器带宽
	// 滤波器的截止频率为f_center±f_cutoff/2
	BPF.set_center(f_center);
	chebyshev_iir(BPF, f_cutoff, ripple);
	iir_df<float_type> LPF(BPF);

//	double imp = 1.0;
	for (int i = 0; i < N; ++i)
	{
		double t = i * Ts;
//		double y = LPF.clock(imp);
//		imp = 0;
		double y = LPF.clock(data[i].y());
		data_filter.append(QPointF(t, y));
	}

	m_axisX12->setRange(0, N * Ts);
	m_axisY12->setRange(-2, 2);
	m_series12->replace(data_filter);
}

void MainWindow::on_pb_filter_fft_clicked()
{
	// 输入数据
	std::vector<double> x(N);
	for (int i = 0; i < N; ++i)
		x[i] = data_filter[i].y();

	// 傅里叶变换
	std::vector<std::complex<double>> y(N);
	y = DFT(x);

	QVector<QPointF> d;
	// 频率间隔
	double f0 = fs / N;

	// 输出数据，单边谱，将幅值与正弦函数幅值等价
	// 双边转单边需要将幅值乘以2
	for (int i = 0; i < N / 2; ++i)
	{
		double f = f0 * i;
		double y_m = abs(y[i]) / N * 2;
		d.append(QPointF(f, y_m));
	}
	// 设置坐标轴范围
	m_axisX22->setRange(0, fs / 2);
	m_axisX22->setTickCount(11);

	m_axisY22->setRange(0, 1.2);
	m_axisY22->setTickCount(7);
	m_series22->replace(d);
}

void MainWindow::on_pb_nf_clicked()
{
	data_filter.clear();
	// 陷波滤波的频率，单位kHz
	notch_allpass<double, double> Notch(0.05);
	for (int i = 0; i < N; ++i)
	{
		double t = i * Ts;
		double y = Notch.clock(data[i].y());
		data_filter.append(QPointF(t, y));
	}

	m_axisX12->setRange(0, N * Ts);
	m_axisY12->setRange(-2, 2);
	m_series12->replace(data_filter);
}
