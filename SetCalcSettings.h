#pragma once

#include <QDialog>
#include "ui_SetCalcSettings.h"
#include "MyDataTypes.h"

class SetCalcSettings : public QDialog
{
	Q_OBJECT

public:
	SetCalcSettings(QWidget *parent = Q_NULLPTR);
	~SetCalcSettings();

	QVector<bool> GetSets() const;
	double GetRel() const;
	double GetTime() const;

private slots:
	void OnDialogAccepted();
	void OnDialogRejected();
	void OnRelChanged();
	void OnTimeChanged();
	void OnShowTheory();

private:
	Ui::SetCalcSettings ui_;

	double rel_;
	double time_;
};
