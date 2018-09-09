#include "SetCalcSettings.h"
#include "ShowTheory.h"

SetCalcSettings::SetCalcSettings(QWidget *parent)
: QDialog(parent),
rel_{},
time_{}
{
	ui_.setupUi(this);

	QStringList list;
	list << QTEXT("сек") << QTEXT("мин") << QTEXT("час");
	ui_.units->addItems(list);
	ui_.units->setCurrentIndex(2);

	//QRegExp rx("[0-9]\\d{0,3}");
	//QValidator *v = new QRegExpValidator(rx, this);
	auto *v = new QDoubleValidator(this);
	ui_.rel_edit->setValidator(v);
	ui_.time_edit->setValidator(v);

	connect(ui_.CALC, &QPushButton::clicked, this, &SetCalcSettings::OnDialogAccepted);
	connect(ui_.Cancel, &QPushButton::clicked, this, &SetCalcSettings::OnDialogRejected);
	connect(ui_.rel_edit, &QLineEdit::textChanged, this, &SetCalcSettings::OnRelChanged);
	connect(ui_.time_edit, &QLineEdit::textChanged, this, &SetCalcSettings::OnTimeChanged);
	connect(ui_.theory, &QPushButton::clicked, this, &SetCalcSettings::OnShowTheory);
}

SetCalcSettings::~SetCalcSettings()
{
}

void SetCalcSettings::OnDialogAccepted()
{
	accept();
}

void SetCalcSettings::OnDialogRejected()
{
	reject();
}

QVector<bool> SetCalcSettings::GetSets() const
{
	QVector<bool> v{};
	v.append(ui_.checkBox->checkState());
	v.append(ui_.checkBox_2->checkState());
	v.append(ui_.checkBox_3->checkState());
	v.append(ui_.checkBox_4->checkState());
	v.append(ui_.checkBox_5->checkState());
	return v;
}

double SetCalcSettings::GetRel() const
{
	return rel_;
}

double SetCalcSettings::GetTime() const
{
	return time_;
}

void SetCalcSettings::OnRelChanged()
{
	rel_ = ui_.rel_edit->text().toDouble();
}

void SetCalcSettings::OnTimeChanged()
{
	time_ = ui_.time_edit->text().toDouble();
}

void SetCalcSettings::OnShowTheory()
{
	ShowTheory st(this);
	st.exec();
}