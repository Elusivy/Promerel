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
	QPair<int, QRect> GetFirstNode() const;										// ��� ����������� ������
	bool IsFirst();
	VectOfPoints GetVectCloseRect();											// ���������� start_poses_
	VectOfPoints GetVectCloseRectGlobal();										// ���������� start_poses_
	void SetClosed();
	bool IsClosed();
	void SetOpen();
	bool IsClosing();
	void SetClosePoint(const int br_num, const QPointF& pos);						// ������ � ������ start_poses_ ������� �������� �������� �����
	void DeleteLastClosePoint(const int br_num);
	PairInt GetPrevObject() const;												// ����� ���������� ������
	bool HasPrevObj() const;													// ����� �� ���������� ������
	PairInt GetBelConIDNum() const;												// ����� ����(ID, ����� �����) �����, �� ������� �� �����
	void MakeBranchHistory(const int br_num, const int obj_type, const int id); // ����� ������� ������ �����
	BranchVect GetBranchVect();													// ����� branches_
	bool IsConEmpty() const;													// ����� �� ����� (��� �� ����� ������)
	void RemoveObjOnBranch(const int br_num, const int obj_type, const int id);	// ���������� ���������� ������ �� �����
	bool HasConClosedCons() const;												/* ����� �� ����� ��������� ��������
																				   ����� �� ���� ��������� ������.
																				   �������� �������� ! */
	void SetChosen();															// ����� ������� - ����������
	void SetUnchosen();															// ����� ������ �� ������� - ��������
	void MoveLine(const int br_num, const int delta);							// �������� ����� �� ����� ��������� (���� �������� ��� �������� ������)
	void MoveBranches(const int delta);											// �������� ���� �������� �����
	void Move(const int delta);													// �������� ����������� �����
	VectLines GetLines() const;
	void InitializeLines();
	void InitializeNodes();
	void MakeBeauty();															/* "������� �������" - ����������� ������ � ���.������� 
																					��� ����������� ����� ����� �������� ���� ����� */
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
	QPointF			start_pos_;			// ��������� ������� ��������� ���� �����
	VOfVOfPoints 	start_poses_;		// ������ ������� ����� �� �������� ��������� ������ ����� �����
	QRectF			rect_;				// bounding rect �����
	QRect			main_rect_;			// � ������ ����������� ����� - bounding rect ����� ����� (�� ��������� ����� �� ��������)
	QRect			rect_inside_;		// � ������ ����������� ����� - bounding rect ��� ���������� ����������� �����
	VectOfPairRects nodes_;				// ������ (����� �����, �� ����) ����� �� ����� ����� (���� ����������� �����, �� ���� ���� ����� �����)
	BranchVect		branches_;			// ����� �������� ��� ������ ����� ����� (������: ���, ID)
	VectLines		lines_;				// ����� ����� �� ����� (������ � ������������� ������)
	int				id_;
	bool			is_checked_;		// �������� �� �� ������ �� ������ ����� 
	bool			is_checked_main_;	// �������� �� �� ��������� �� main_rect_ (��� �����������)
	bool			is_checked_inside_;	// �������� �� �� ��������� �� rect_inside_ (��� �����������)
	bool			is_fixed_;			// ���� ����� �������� ���-�� ����, �� �� ��������� � �� ����� ���� ������
	bool			is_new_tree_;		// ������ �� ����� �� ���� ����� (������ �� ��������� ���������� ����)
	bool			is_closing_;		// ����������� �� �����
	bool			is_closed_;			// ������� �� �����
	bool			is_chosen_;			// ������� �� ����� ��� ���������
	bool			is_line_moving_;
	bool			is_ready_to_move_;
	int				branch_count_;		// ���������� ������ �����
	PairInt			prev_object_;		// ���������� �������/����� (<1 - �������/2 - �����, ID>)
	PairInt			bel_con_id_num_;	// ID ����� � ����� ����� �����, �� ������� ������������� ��� �����
	QPointF mutable	myPos_;
	QPoint			old_pos_;			// ������������ ��� ����������� �����
	int				line_moving_;
};