#pragma once

#include <QWidget>
#include "ui_ShowResultView.h"
#include <QVector>

class ShowResultView : public QDialog
{
	Q_OBJECT

public:
	ShowResultView(QWidget *parent, const double sys_in, const QVector<bool> &v, const double rel, const double time);
	~ShowResultView();
	
	void DrawGraphs(const double bottom, const double top);

private slots:
	void OnFromChanged();
	void OnToChanged();

private:
	Ui::ShowResultView ui;
	double intensity_;
	double from_;
	double to_;
};
