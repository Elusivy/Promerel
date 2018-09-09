#include <QMessageBox.h>
#include <QGroupBox>

#include "SetElementSettingsView.h"
#include "HelpFuncs.h"

SetElementSettingsView::SetElementSettingsView(const QString &label)
: QDialog{},
elem_title_{},
intensity_{},
time_{ 1 },
timeT_{ 0 },
units_{},
units_2_{},
reliability_{}
{
	ui_.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowTitle(label);

	QStringList list;
	list << QTEXT("сек") << QTEXT("мин") << QTEXT("час");
	ui_.comboBox->addItems(list);
	ui_.comboBox_3->addItems(list);
	ui_.comboBox->setCurrentIndex(2);
	ui_.comboBox_3->setCurrentIndex(2);
	ui_.time_edit->setText(QTEXT("1"));

	list.clear();
	list << QTEXT("1/сек") << QTEXT("1/мин") << QTEXT("1/час");
	ui_.comboBox_2->addItems(list);
	ui_.comboBox_2->setCurrentIndex(2);

	QRegExp rx("[1-9]\\d{0,3}");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui_.time_edit->setValidator(validator);

	connect(ui_.elem_title_edit, &QLineEdit::textEdited, this, &SetElementSettingsView::OnTitleChanged);
	connect(ui_.reliability_edit, &QLineEdit::textEdited, this, &SetElementSettingsView::OnReliabilityChanged);
	connect(ui_.intensity_edit, &QLineEdit::textEdited, this, &SetElementSettingsView::OnIntensityChanged);
	connect(ui_.time_edit, &QLineEdit::textEdited, this, &SetElementSettingsView::OnTimeChanged);
	connect(ui_.timeT, &QLineEdit::textEdited, this, &SetElementSettingsView::OnTimeTChanged);
	connect(ui_.comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SetElementSettingsView::OnUnits1Changed);
	connect(ui_.comboBox_2, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SetElementSettingsView::OnUnits2Changed);
	connect(ui_.comboBox_3, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SetElementSettingsView::OnUnits3Changed);

	connect(ui_.groupBox, &QGroupBox::clicked, this, &SetElementSettingsView::OnGroupBox1Changed);
	connect(ui_.groupBox2, &QGroupBox::clicked, this, &SetElementSettingsView::OnGroupBox2Changed);
	connect(ui_.groupBox3, &QGroupBox::clicked, this, &SetElementSettingsView::OnGroupBox3Changed);

	connect(ui_.OK, &QPushButton::clicked, this, &SetElementSettingsView::OnDialogAccepted);
	connect(ui_.Cancel, &QPushButton::clicked, this, &SetElementSettingsView::OnDialogRejected);
}

SetElementSettingsView::~SetElementSettingsView()
{
}

VectFloat SetElementSettingsView::GetData() const
{
	VectFloat v{};
	v.push_back(intensity_);
	v.push_back(time_);
	v.push_back(units_);
	v.push_back(reliability_);
	v.push_back(timeT_);
	return v;
}

void SetElementSettingsView::SetData(const VectFloat &v)
{
	intensity_ = v.at(0);
	ui_.intensity_edit->setText(QString("%1").arg(intensity_));
	time_ = v.at(1);
	ui_.reliability_edit->setText(QString("%1").arg(time_));
	units_ = v.at(2);
	ui_.comboBox->setCurrentIndex(units_);
	ui_.comboBox_3->setCurrentIndex(units_);
	reliability_ = v.at(3);
	ui_.reliability_edit->setText(QString("%1").arg(reliability_));
	timeT_ = v.at(4);
	ui_.timeT->setText(QString("%1").arg(timeT_));
	if (reliability_)
	{
		ui_.groupBox->setChecked(true);
		ui_.groupBox2->setChecked(false);
		ui_.groupBox3->setChecked(false);
	}
	else if (intensity_)
	{
		if (timeT_)
		{
			ui_.groupBox->setChecked(false);
			ui_.groupBox2->setChecked(false);
			ui_.groupBox3->setChecked(true);
		}
		else
		{
			ui_.groupBox2->setChecked(true);
			ui_.groupBox->setChecked(false);
			ui_.groupBox3->setChecked(false);
		}
	}
}

const QString &SetElementSettingsView::GetElemTitle() const
{
	return elem_title_;
}

void SetElementSettingsView::SetTitle(const QString &str)
{
	elem_title_ = str;
	ui_.elem_title_edit->setText(QTEXT("%1").arg(str));
}

void SetElementSettingsView::OnDialogAccepted()
{
	if (!elem_title_.isEmpty() && (ui_.groupBox->isChecked() ? reliability_ : (ui_.groupBox2->isChecked() ? (intensity_ && time_) : timeT_)))
	{
		accept();
	}
	else
		QMessageBox::warning(this, QString::fromLocal8Bit("ќшибка"), QString::fromLocal8Bit("¬ведены не все данные!"));
}

void SetElementSettingsView::OnDialogRejected()
{
	reject();
}

void SetElementSettingsView::OnIntensityChanged()
{
	const QString str{ ui_.intensity_edit->text() };
	if (str.isEmpty())
		return;
	const auto pos = GetCommaPos(str);
	if (pos > 0)
	{
		ui_.intensity_edit->setText(QString("%1").arg(str.left(pos)));
	}
	const float value = GetFloatFromStr(str);
	if (value == 1001.)
	{
		ui_.intensity_edit->setText(QString("%1").arg(str.left(str.indexOf(str.at(str.size() - 1)))));
	}
	else
	{
		intensity_ = value;
	}
}

void SetElementSettingsView::OnTimeChanged()
{
	time_ = ui_.time_edit->text().toDouble();
}

void SetElementSettingsView::OnTimeTChanged()
{
	timeT_ = ui_.timeT->text().toDouble();
}

void SetElementSettingsView::OnUnits1Changed(const int index)
{
	units_ = index;
	ui_.comboBox_3->setCurrentIndex(units_);
}

void SetElementSettingsView::OnUnits2Changed(const int index)
{
	units_2_ = index;
}

void SetElementSettingsView::OnUnits3Changed(const int index)
{
	units_ = index;
	ui_.comboBox->setCurrentIndex(units_);
}

void SetElementSettingsView::OnGroupBox1Changed()
{
	if (ui_.groupBox->isChecked())
	{
		ui_.groupBox2->setChecked(false);
		ui_.groupBox3->setChecked(false);
	}
	else
	{
		ui_.groupBox2->setChecked(true);
		ui_.groupBox3->setChecked(false);
	}
}

void SetElementSettingsView::OnGroupBox2Changed()
{
	if (ui_.groupBox2->isChecked())
	{
		ui_.groupBox->setChecked(false);
		ui_.groupBox3->setChecked(false);
	}
	else
	{
		ui_.groupBox->setChecked(true);
		ui_.groupBox3->setChecked(false);
	}
}

void SetElementSettingsView::OnGroupBox3Changed()
{
	if (ui_.groupBox3->isChecked())
	{
		ui_.groupBox->setChecked(false);
		ui_.groupBox2->setChecked(false);
	}
	else
	{
		ui_.groupBox->setChecked(true);
		ui_.groupBox2->setChecked(false);
	}
}

void SetElementSettingsView::OnReliabilityChanged()
{
	const QString str{ ui_.reliability_edit->text() };
	if (str.isEmpty())
		return;
	const auto pos = GetCommaPos(str);
	if (pos > 0)
	{
		ui_.reliability_edit->setText(QString("%1").arg(str.left(pos)));
	}
	const float value = GetFloatFromStr(str);
	if (value == 1001.)
	{
		ui_.reliability_edit->setText(QString("%1").arg(str.left(str.indexOf(str.at(str.size() - 1)))));
	}
	else
	{
		reliability_ = value;
	}
}

void SetElementSettingsView::OnTitleChanged()
{
	elem_title_ = ui_.elem_title_edit->text();
}