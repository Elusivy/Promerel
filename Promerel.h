#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Promerel.h"
#include "CustomScene.h"
#include "Connection.h"
#include "Element.h"
#include "MyDataTypes.h"
#include "XPoint.h"

class Promerel : public QMainWindow
{
    Q_OBJECT

public:
    Promerel(QWidget *parent = Q_NULLPTR);
	~Promerel();
	void AddNewElement(const int node_id);
	int GetConId();										// Получить ID связи
	int GetElemId();									// Получить ID элемента
	Element *GetElemById(const int id) const;			// Поиск элемента по ID
	Connection *GetConById(const int id) const;			// Поиск связи по ID
	void MakeHistory(const int obj_type, const int id); // Тип объекта (1 - элемент, 2 - связь), ID объекта
	void SetConClosed(const int id);
	bool IsCalculable() const;							// Можно ли приступать к расчетам
	bool CanICloseThisCon(const int con_id) const;		// Все ли связи закрыты внутри этой связи
	QPair<double, double> CalcCon(const int id, const double time);
	void ChangeChosenElem(const int id);
	void ChangeChosenCon(const int id);
	void SetActionsActivity();
	void SetDefaultActions();
	void ShowMenu(const bool is_menu, QPoint pos);
	void ResetBuffer();
	void UnchoseAllElems();
	void UnchoseAllCons();
	void ResetConsChecked();
	void ResetElemsChecked();
	QPoint ToGlobal(QPoint pos) const;
	void ReDraw();
	void ShowStatusToolTip(const QString& text);
	void ClearStatusBar();
	QString GetElemVersionsCount(Element *elem) const;

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
	void OnAddNewCon();
	void OnDeleteCon();
	void OnCloseCon();
	void OnAddNewElem();
	void OnChangeElemChars();
	void OnDeleteElem();
	void OnCalc();
	void OnClearScene();
	void OnReserve();
	void OnAboutMe();

private:
    Ui::PromerelClass ui_;
	CustomScene *scene_;
	int elem_ID_;
	int chosen_elem_id_;
	int con_ID_;
	int chosen_con_id_;
	ConVect cons_;				// вектор связей
	ElemVect elements_;			// вектор элементов
	HistoryVect history_;		// вектор пар (<тип объекта(1 - элемент, 2 - связь), ID объекта>) объектов верхнего уровня
	QPointF buffer_pos_;		// буфер для поинтов
	int buffer_con_id_;			// буфер для ID связи
	int buffer_elem_id_;		// буфер для ID элемента
	bool is_closing_con_;		// закрывающая ли следующая связь
	int con_stopping_history_;	// ID связи, на которой остановилась запись в вектор объектов верхнего уровня истории
	XPoint *xp_;
	int time_;
	bool is_chosing_group_;
	HistoryVect chosen_group_;

	QAction *add_new_con_act_;
	QAction *close_con_act_;
	QAction *delete_con_act_;
	QAction *add_new_elem_act_;
	QAction *change_elem_chars_act_;
	QAction *delete_elem_act_;
	QAction *calc_act_;
	QAction *clear_scene_;
	QAction *about_me_;
	QAction *reserve_act_;
};
