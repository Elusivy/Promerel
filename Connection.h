#pragma once
#include <QGraphicsItem.h>

#include "MyDataTypes.h"
#include "Promerel.h"

class Promerel;

class Connection :
	public QObject, public QGraphicsItem
{
public:
	Connection(Promerel *prom, const int id, const bool is_new_tree, const PairInt& prev_obj, const PairInt& bel_con_is_num, const bool is_closing, const VectOfPoints& node_pos, const int branch_count);
	~Connection();

	bool IsChecked() const;
	void ResetFlag();
	int GetCheckedPartly() const;
	void ResetPartlyFlags();
	bool IsFixed() const;
	void SetFixed();
	void SetUnfixed();
	int GetId() const;
	int GetBranchCount() const;
	QPointF GetStartPos() const;
	QPair<int, QRect> GetConsNodeRect() const;
	QPair<int, QRect> GetFirstNode() const;										// Для закрывающих связей
	bool IsFirst();
	VectOfPoints GetVectCloseRect();											// Возвращает start_poses_
	VectOfPoints GetVectCloseRectGlobal();										// Возвращает start_poses_
	void SetClosed();
	bool IsClosed();
	void SetOpen();
	bool IsClosing();
	void SetClosePoint(const int br_num, const QPointF& pos);						// Запись в вектор start_poses_ позиции крайнего элемента ветки
	void DeleteLastClosePoint(const int br_num);
	PairInt GetPrevObject() const;												// Отдаёт предыдущий объект
	bool HasPrevObj() const;													// Имеет ли предыдущий объект
	PairInt GetBelConIDNum() const;												// Отдаёт пару(ID, номер ветки) связи, на которой мы висим
	void MakeBranchHistory(const int br_num, const int obj_type, const int id); // Пишет историю каждой ветки
	BranchVect GetBranchVect();													// Отдаёт branches_
	bool IsConEmpty() const;													// Пуста ли связь (все ли ветки пустые)
	void RemoveObjOnBranch(const int br_num, const int obj_type, const int id);	// Унижтожает конкретный объект из ветки
	bool HasConClosedCons() const;												/* Имеет ли связь полностью закрытые
																				   связи на всех глубинных ветках.
																				   Работает РЕКУРСИЯ ! */
	void SetChosen();															// Связь выбрана - подсветить
	void SetUnchosen();															// Связь теперь не выбрана - потушить
	void MoveLine(const int br_num, const int delta);							// Смещение ветки со всеми объектами (есть РЕКУРСИЯ для закрытых связей)
	void MoveBranches(const int delta);											// Смещение всех объектов веток
	void Move(const int delta);													// Смещение закрывающей связи
	VectLines GetLines() const;
	void InitializeLines();
	void InitializeNodes();
	void MakeBeauty();															/* "Навести красоту" - определение ректов и нач.позиций 
																					для закрывающей связи после создания этой связи */
	bool IsLineMoving() const;

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
	void DrawNode(QPainter *painter, QRect rect);

	Promerel		*prom_;
	QPointF			start_pos_;			// начальная позиция рисования всей связи
	VOfVOfPoints 	start_poses_;		// вектор поинтов узлов на КОНЕЧНЫХ элементах каждой ветки связи
	QRectF			rect_;				// bounding rect общий
	QRect			main_rect_;			// в случае закрывающей связи - bounding rect самой связи (от начальных узлов до конечных)
	QRect			rect_inside_;		// в случае закрывающей связи - bounding rect для соединений закрывающей связи
	VectOfPairRects nodes_;				// Вектор (номер ветки, ее рект) узлов НА САМОЙ связи (если закрывающая связь, то один узел после связи)
	BranchVect		branches_;			// набор объектов для каждой ветки связи (объект: ТИП, ID)
	VectLines		lines_;				// линии веток до узлов (только у незакрывающих связей)
	int				id_;
	bool			is_checked_;		// кликнули ли мы ВООБЩЕ по данной связи 
	bool			is_checked_main_;	// кликнули ли мы КОНКРЕТНО по main_rect_ (для закрывающей)
	bool			is_checked_inside_;	// кликнули ли мы КОНКРЕТНО по rect_inside_ (для закрывающей)
	bool			is_fixed_;			// если после элемента что-то есть, то он закреплен и не может быть удален
	bool			is_new_tree_;		// первая ли связь на всей сцене (влияет на отрисовку начального узла)
	bool			is_closing_;		// закрывающая ли связь
	bool			is_closed_;			// закрыта ли связь
	bool			is_chosen_;			// выбрана ли связь для подсветки
	bool			is_line_moving_;
	bool			is_ready_to_move_;
	int				branch_count_;		// количество ветвей связи
	PairInt			prev_object_;		// предыдущий элемент/связь (<1 - элемент/2 - связь, ID>)
	PairInt			bel_con_id_num_;	// ID связи и номер ветки связи, на которой располагается эта связь
	QPointF mutable	myPos_;
	QPoint			old_pos_;			// используется при перемещении ветки
	int				line_moving_;
};