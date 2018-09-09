#include "ShowResultView.h"
#include "MyDataTypes.h"

ShowResultView::ShowResultView(QWidget *parent, const double sys_in, const QVector<bool> &v, const double rel, const double time)
: QDialog(parent),
intensity_{ sys_in },
from_{ 0 },
to_{ 1000 }
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowTitle(QTEXT("Результаты"));
	QVector<QPair<QString, QString> > items{};
	if (v.at(0)) items.append({ QTEXT("Интенсивность отказа всей системы :"), QTEXT("%1").arg(sys_in) });
	if (v.at(1)) items.append({ QTEXT("Вероятность безотказной работы системы:"), QTEXT("e^(-%1*t)").arg(sys_in) });
	if (v.at(2)) items.append({ QTEXT("Безотказность системы в течение %1 час. :").arg(time), QTEXT("%1").arg(rel) });
	if (v.at(3)) items.append({ QTEXT("Среднее время безотказной работы системы :"), QTEXT("%1 час.").arg(1 / sys_in) });
	if (v.at(4)) items.append({ QTEXT("Время безотказной работы с вероятность %1 :").arg(rel), QTEXT("%1 час.").arg(floor(log(rel) / (-intensity_))) });

	auto *val = new QDoubleValidator(this);
	val->setNotation(QDoubleValidator::StandardNotation);
	ui.from->setValidator(val);
	ui.from->setText(QTEXT("%1").arg(from_));
	ui.to->setValidator(val);
	ui.to->setText(QTEXT("%1").arg(to_));

	for (int i = 0; i < items.count(); ++i)
	{
		auto *h = new QHBoxLayout(this);
		auto *l = new QLabel(this);
		l->setText(items.at(i).first);
		auto *le = new QLineEdit(this);
		le->setValidator(val);
		le->setText(items.at(i).second);
		QFontMetrics fm(font());
		le->setFixedWidth(fm.boundingRect(items.at(i).second).width() + 10);
		le->setReadOnly(true);
		h->addWidget(l);
		h->addWidget(le);
		ui.verticalLayout->insertLayout(i, h);
	}
	DrawGraphs(from_, to_);

	connect(ui.from, &QLineEdit::textChanged, this, &ShowResultView::OnFromChanged);
	connect(ui.to, &QLineEdit::textChanged, this, &ShowResultView::OnToChanged);
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &ShowResultView::accept);

	ui.widget->setFixedWidth(400);
	ui.widget->setFixedHeight(300);
}

ShowResultView::~ShowResultView()
{
}

void ShowResultView::OnFromChanged()
{
	const auto d = ui.from->text().toDouble();
	if (d >= to_)
	{
		ui.from->clear();
		return;
	}
	from_ = d;
	DrawGraphs(from_, to_);
}

void ShowResultView::OnToChanged()
{
	const auto d = ui.to->text().toDouble();
	if (d <= from_ || !d)
	{
		ui.to->clear();
		return;
	}
	to_ = d;
	DrawGraphs(from_, to_);
}

void ShowResultView::DrawGraphs(const double bottom, const double top)
{
	const double a{ bottom };
	const double b{ top };
	const double h{ (top - bottom) / 1000 }; //Шаг

	const int N = (b - a) / h + 2; // Вычисляем количество точек, которые будем отрисовывать
	QVector<double> x(N), y(N); // Векторы координат точек

	int i{};
	for (double X = a; X <= b; X += h) //Пробегаем по всем точкам
	{
		x[i] = X;
		y[i] = exp(-intensity_*X); //Формула нашего экспоненциального распределения
		++i;
	}
	ui.widget->clearGraphs();
	ui.widget->addGraph();
	ui.widget->graph(0)->setData(x, y);
	ui.widget->xAxis->setRange(a, b); //Для оси Ox

	//Вычислим минимальное и максимальное значения
	double min_y = y[0], max_y = y[0];
	for (int i = 1; i < N; ++i)
	{
		if (y[i]<min_y) min_y = y[i];
		if (y[i]>max_y) max_y = y[i];
	}
	ui.widget->yAxis->setRange(min_y, max_y); //Для оси Oy
	ui.widget->replot();
}


