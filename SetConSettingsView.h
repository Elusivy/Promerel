#pragma once

#include <QDialog>

#include "ui_SetConSettingsView.h"

class SetConSettingsView : public QDialog
{
	Q_OBJECT

public:
	SetConSettingsView(QWidget *parent = Q_NULLPTR);
	~SetConSettingsView();

	int GiveBranchCount();

private slots:
	void OnSaveSettings();

private:
	Ui::SetConSettingsView ui_;
	int branch_count_;
};
