#pragma once

#include <QDialog>
#include "ui_ShowTheory.h"

class ShowTheory : public QDialog
{
	Q_OBJECT

public:
	ShowTheory(QWidget *parent = Q_NULLPTR);
	~ShowTheory();

	void FillText(const QString &text);
private:
	Ui::ShowTheory ui;
};
