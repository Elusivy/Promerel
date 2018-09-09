#pragma once

#include <QDialog>

#include "ui_SetElementSettingsView.h"
#include "MyDataTypes.h"

class SetElementSettingsView : public QDialog
{
	Q_OBJECT

public:
	SetElementSettingsView(const QString &label = QTEXT("Создание элемента"));
	~SetElementSettingsView();

	VectFloat GetData() const;
	void SetData(const VectFloat &v);
	const QString &GetElemTitle() const;
	void SetTitle(const QString &str);

private slots:
	void OnDialogAccepted();
	void OnDialogRejected();
	void OnIntensityChanged();
	void OnReliabilityChanged();
	void OnTitleChanged();
	void OnTimeChanged();
	void OnTimeTChanged();
	void OnUnits1Changed(const int index);
	void OnUnits2Changed(const int index);
	void OnUnits3Changed(const int index);
	void OnGroupBox1Changed();
	void OnGroupBox2Changed();
	void OnGroupBox3Changed();

private:
	Ui::SetElementSettingsView ui_;

	QString elem_title_;	// Название элемента
	double intensity_;		// Интенсивность отказов
	double time_;
	int units_;
	int units_2_;
	double reliability_;	// Вероятность безотказной работы
	double timeT_;			// Среднее время безотказной работы
};
