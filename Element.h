#pragma once
#include <qgraphicsitem.h>
#include "MyDataTypes.h"

class Promerel;

class Element :
	public QObject, public QGraphicsItem
{
public:
	Element(Promerel *prom, const int id, const bool is_new_tree, const PairInt& bel_con_id_num, const PairInt& prev_body, const QPointF& node_pos, const QString& title, const VectFloat& data);
	Element(Element *el, const int id, const QString& ver);
	~Element();

	bool IsChecked() const;
	void ResetFlag();
	bool IsFixed() const;
	void SetFixed();
	void SetUnfixed();
	int GetId() const;
	const QPointF &GetElemsExitPoint() const;
	PairInt GetBelConIDNum() const;					// ���������� ID � ����� �����, �� ������� �������
	PairInt GetPrevObject() const;
	bool HasPrevObj() const;
	void ChangeChars();
	void ExtractData(const VectFloat& data);
	void SetChosen();								// ������� ������� - ����������
	void SetUnchosen();								// ������� ������ �� ������� - ��������
	void MoveIt(const int delta);

	const QString& GetTitle() const;
	double GetIntensity() const;
	double GetReliability() const;
	double GetTimeT() const;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	//void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

	QRectF rect_;
	QPointF exit_point_;
	int id_;
	bool is_checked_;
	bool is_fixed_;				// ���� ����� �������� ���-�� ����, �� �� ��������� � �� ����� ���� ������
	bool is_chosen_;			// ������ �� ������� ��� ���������
	bool is_new_tree_;
	PairInt bel_con_id_num_;	// ID ����� � ����� ����� �����, �� ������� ������������� ���� �������
	PairInt prev_object_;		// ���������� �������/����� (<1 - �������/2 - �����, ID>)
	Promerel *prom_;

	QString elem_title_;		// �������� ��������
	double intensity_;			// ������������� �������
	double time_;
	int units_;
	double reliability_;		// ����������� ����������� ������
	double timeT_;				// ������� ����� ����������� ������
};