#include <QPainter>
#include <QRect>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

#include "Connection.h"

Connection::Connection(Promerel *prom, const int id, const bool is_new_tree, const PairInt& prev_obj, const PairInt& bel_con_is_num, const bool is_closing, const VectOfPoints& points, const int branch_count) :
QObject{},
QGraphicsItem{},
prom_{ prom },
start_pos_{ points.front() },
start_poses_{},
rect_{ !is_closing ? QRect(QPoint(0, -8 * (2 * branch_count - 2) - 8), QSize(32 + (is_new_tree ? 16 : 0), 16 * (2 * branch_count - 2) + 16)) : QRect() },
main_rect_{},
rect_inside_{},
nodes_{},
branches_{},
lines_{},
id_{ id },
is_checked_{},
is_checked_main_{},
is_checked_inside_{},
is_fixed_{},
is_new_tree_{ is_new_tree },
is_closing_{ is_closing },
is_closed_{},
is_chosen_{},
is_line_moving_{},
is_ready_to_move_{},
branch_count_{ branch_count },
prev_object_{ prev_obj },
bel_con_id_num_{ bel_con_is_num },
myPos_{},
old_pos_{},
line_moving_{}
{
	if (!is_closing_)
	{
		InitializeLines();
		InitializeNodes();
		branches_.resize(branch_count_);
	}
	setAcceptHoverEvents(true);
}

Connection::~Connection()
{
}

void Connection::InitializeLines()
{
	lines_.append(QLine(QPoint(rect_.x() + 8 + (is_new_tree_ ? 16 : 0), rect_.y() + 8), QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.y() + 8))); // 1-я
	for (int i = 1; i < branch_count_ - 1; ++i)
	{
		lines_.append(QLine(QPoint(rect_.x() + 8 + (is_new_tree_ ? 16 : 0), rect_.y() + 2 * 16 * i + 8), QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.y() + 2 * 16 * i + 8))); // между
	}
	lines_.append(QLine(QPoint(rect_.x() + 8 + (is_new_tree_ ? 16 : 0), rect_.bottom() - 8), QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.bottom() - 8))); // последняя
	lines_.append(QLine(QPoint(is_new_tree_ ? 16 : 0, 0), QPoint(8 + (is_new_tree_ ? 16 : 0), 0))); // входная горизонтальная
	lines_.append(QLine(QPoint(8 + (is_new_tree_ ? 16 : 0), rect_.top() + 8), QPoint(8 + (is_new_tree_ ? 16 : 0), rect_.bottom() - 8))); // вертикальная
}

void Connection::InitializeNodes()
{
	const QRect first_node{ QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.y()), QSize(16, 16) };
	nodes_.push_back(QPair<int, QRect>(1, first_node)); // 1-й узел
	for (int i = 1; i < branch_count_ - 1; ++i)
	{
		const QRect node{ QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.y() + 2 * 16 * i), QSize(16, 16) };
		nodes_.push_back(QPair<int, QRect>(i + 1, node)); // i-е узлы
	}
	const QRect last_node{ QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.bottom() - 16), QSize(16, 16) };
	nodes_.push_back(QPair<int, QRect>(branch_count_, last_node)); // последний

	start_poses_.resize(nodes_.size());
	for (size_t i = 0; i < nodes_.size(); ++i)
	{
		start_poses_[i].append(nodes_[i].second.topRight() + QPoint(0, 8));
	}
}

VectLines Connection::GetLines() const
{
	return lines_;
}

void Connection::MoveLine(const int br_num, const int delta)
{
	if (br_num == 1)
	{
		if (!is_closing_)
		{
			if (delta > 0)
			{
				if (lines_.at(br_num - 1).y1() >= (lines_.at(br_num).y1() - 16))
				{
					QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(br_num).p1()).toPoint()).y() - 16 + 82);
					update();
					return;
				}
				else if (lines_.at(br_num - 1).y1() >= (lines_.at(branch_count_).y1() - 16))
				{
					QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(branch_count_).p1()).toPoint()).y() - 16 + 82);
					update();
					return;
				}
			}
			const auto point = lines_.back().p1();
			lines_[branch_count_ + 1].setP1(QPoint(point.x(), point.y() + delta));
		}
		else
		{
			main_rect_.setTop(rect_.top() + delta);
			rect_inside_.setTop(rect_.top() + delta);
		}
		rect_.setTop(rect_.top() + delta);
	}
	else if (br_num == branch_count_)
	{
		if (!is_closing_)
		{
			if (delta < 0)
			{
				if (lines_.at(br_num - 1).y1() <= (lines_.at(br_num - 2).y1() + 16))
				{
					QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(br_num).p1()).toPoint()).y() + 16 + 82);
					update();
					return;
				}
				else if (lines_.at(br_num - 1).y1() <= (lines_.at(branch_count_).y1() + 16))
				{
					QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(branch_count_).p1()).toPoint()).y() + 16 + 82);
					update();
					return;
				}
			}
			const auto point = lines_.back().p2();
			lines_[branch_count_ + 1].setP2(QPoint(point.x(), point.y() + delta));
		}
		else
		{
			main_rect_.setBottom(main_rect_.bottom() + delta);
			rect_inside_.setBottom(rect_inside_.bottom() + delta);
		}
		rect_.setBottom(rect_.bottom() + delta);
	}
	else
	{
		if (delta > 0)
		{
			if (!is_closing_ && lines_.at(br_num - 1).y1() >= (lines_.at(br_num).y1() - 16))
			{
				QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(br_num).p1()).toPoint()).y() - 16 + 82);
				update();
				return;
			}
		}
		else
		{
			if (!is_closing_ && lines_.at(br_num - 1).y1() <= (lines_.at(br_num - 2).y1() + 16))
			{
				QCursor::setPos(QCursor::pos().x(), prom_->ToGlobal(mapToScene(lines_.at(br_num - 2).p1()).toPoint()).y() + 16 + 82);
				update();
				return;
			}
		}
	}
	if (is_closed_)
		prom_->GetConById(id_ * 100)->MoveLine(br_num, delta);
	if (is_closing_)
	{
		for (auto& pos : start_poses_[br_num - 1])
		{
			pos.ry() += delta;
		}
	}
	else
	{
		lines_[br_num - 1].translate(0, delta);
		nodes_[br_num - 1].second.translate(0, delta);
		for (auto& pos : start_poses_[br_num - 1])
		{
			pos.ry() += delta;
		}
		for (auto obj : branches_.at(br_num - 1))
		{
			if (obj.first == 1)
				prom_->GetElemById(obj.second)->MoveIt(delta);
			else
				prom_->GetConById(obj.second)->MoveBranches(delta);
		}
		prom_->ReDraw();
	}
}

void Connection::MoveBranches(const int delta)
{
	for (auto& line : lines_)
	{
		line.translate(QPoint(0, delta));
	}
	for (auto& node : nodes_)
	{
		node.second.translate(QPoint(0, delta));
	}
	for (auto& poss : start_poses_)
	{
		for (auto& pos : poss)
		{
			pos.ry() += delta;
		}
	}
	for (auto branch : branches_)
	{
		for (auto obj : branch)
		{
			if (obj.first == 1)
				prom_->GetElemById(obj.second)->MoveIt(delta);
			else
				prom_->GetConById(obj.second)->MoveBranches(delta);
		}
	}
	rect_.translate(QPoint(0, delta));
	if (is_closed_)
		prom_->GetConById(id_ * 100)->Move(delta);
}

void Connection::Move(const int delta)
{
	rect_inside_.translate(QPoint(0, delta));
	main_rect_.translate(QPoint(0, delta));
	rect_.translate(QPoint(0, delta));
	for (auto& poss : start_poses_)
	{
		for (auto& pos : poss)
		{
			pos.ry() += delta;
		}
	}
}

bool Connection::IsChecked() const
{
	return is_checked_;
}

void Connection::ResetFlag()
{
	is_checked_ = false;
}

int Connection::GetCheckedPartly() const
{
	if (is_checked_inside_)
		return 1;
	else if (is_checked_main_)
		return 2;
	return 0;
}

void Connection::ResetPartlyFlags()
{
	is_checked_main_ = false;
	is_checked_inside_ = false;
}

bool Connection::IsFixed() const
{
	return is_fixed_;
}

void Connection::SetFixed()
{
	is_fixed_ = true;
}

void Connection::SetUnfixed()
{
	is_fixed_ = false;
}

int Connection::GetId() const
{
	return id_;
}

int Connection::GetBranchCount() const
{
	return branch_count_;
}

QPointF Connection::GetStartPos() const
{
	return start_pos_;
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!is_ready_to_move_)
	{
		if (is_closing_)
		{
			is_checked_ = true;
			if (main_rect_.contains(event->pos().toPoint()))
				is_checked_main_ = true;
			if (rect_inside_.contains(event->pos().toPoint()))
				is_checked_inside_ = true;
		}
		else
		{
			is_checked_ = true;
		}
		is_chosen_ = true;
	}
	else
	{
		is_line_moving_ = true;
	}

	myPos_ = event->pos();
	QGraphicsItem::mousePressEvent(event);
}

void Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (prom_->cursor().shape() != Qt::ArrowCursor && is_line_moving_)
	{
		prom_->unsetCursor();
		is_line_moving_ = false;
		is_ready_to_move_ = false;
	}
	QGraphicsItem::mouseReleaseEvent(event);
}

void Connection::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if (is_closing_)
		return;
	const auto new_pos = event->pos().toPoint();
	int line{};
	for (int i = 0; i < lines_.size() - 2; ++i)
	{
		const auto line_rect = QRect(lines_.at(i).p1() - QPoint(0, 1), lines_.at(i).p2() + QPoint(0, 1));
		if (line_rect.contains(event->pos().toPoint()))
		{
			line = i + 1;
			break;
		}
	}
	if (line || is_line_moving_)
	{
		line_moving_ = line > 0 ? line : line_moving_;
		is_ready_to_move_ = true;
		if (prom_->cursor().shape() != Qt::SizeVerCursor)
			prom_->setCursor(Qt::SizeVerCursor);
		if (is_line_moving_)
		{
			MoveLine(line_moving_, new_pos.y() - old_pos_.y());
		}
	}
	else
	{
		prom_->unsetCursor();
		is_ready_to_move_ = false;
	}
	old_pos_ = new_pos;
	QGraphicsItem::hoverMoveEvent(event);
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//const auto new_pos = event->pos().toPoint();
	//
	//if (is_line_moving_)
	//{
	//	MoveLine(line_moving_, new_pos.y() - old_pos_.y());
	//}
	//old_pos_ = new_pos;
}

QRectF Connection::boundingRect() const
{
	return rect_;
}

QPair<int, QRect> Connection::GetConsNodeRect() const
{
	for (auto node : nodes_)
	{
		if (node.second.contains(myPos_.toPoint()))
		{
			return{ node.first, QRectF(mapToScene(node.second.topLeft()), mapToScene(node.second.bottomRight())).toRect() };
		}
	}
	return QPair<int, QRect>();
}

QPair<int, QRect> Connection::GetFirstNode() const
{
	return{ nodes_.front().first, QRectF(mapToScene(nodes_.front().second.topLeft()), mapToScene(nodes_.front().second.bottomRight())).toRect() };
}

bool Connection::IsFirst()
{
	return is_new_tree_;
}

void Connection::SetClosed()
{
	is_closed_ = true;
}

bool Connection::IsClosed()
{
	return is_closed_;
}

void Connection::SetOpen()
{
	is_closed_ = false;
}

bool Connection::IsClosing()
{
	return is_closing_;
}

bool Connection::IsLineMoving() const
{
	return is_line_moving_;
}

void Connection::SetClosePoint(const int br_num, const QPointF& point)
{
	start_poses_[br_num - 1].append(mapFromScene(point));
}

void Connection::DeleteLastClosePoint(const int br_num)
{
	if (!start_poses_.at(br_num - 1).isEmpty())
		start_poses_[br_num - 1].removeLast();
}

VectOfPoints Connection::GetVectCloseRect()
{
	VectOfPoints vect{};
	for (auto branch : start_poses_)
	{
		vect.append(branch.back());
	}
	return vect;
}

VectOfPoints Connection::GetVectCloseRectGlobal()
{
	VectOfPoints vect{};
	for (auto branch : start_poses_)
	{
		vect.append(mapToScene(branch.back()));
	}
	return vect;
}

PairInt Connection::GetPrevObject() const
{
	return prev_object_;
}

bool Connection::HasPrevObj() const
{
	return (prev_object_.first > 0 && prev_object_.second > 0);
}

PairInt Connection::GetBelConIDNum() const
{
	return bel_con_id_num_;
}

void Connection::MakeBranchHistory(const int br_num, const int obj_type, const int id)
{
	branches_[br_num - 1].push_back(PairInt(obj_type, id));
}

void Connection::RemoveObjOnBranch(const int br_num, const int obj_type, const int id)
{
	for (auto branch : branches_)
	{
		if (branch == branches_.at(br_num - 1))
		{
			for (auto obj : branch)
			{
				if (obj.first == obj_type && obj.second == id)
				{
					branches_[br_num - 1].removeOne(obj);
				}
			}
		}
	}
}

bool Connection::HasConClosedCons() const
{
	bool success{ true };
	if (!is_closed_)
		return false;
	for (auto branch : branches_)
	{
		for (auto obj : branch)
		{
			if (obj.first == 2)
			{
				auto *con = prom_->GetConById(obj.second);
				if (con->IsClosed())
				{
					success = con->HasConClosedCons();
					if (!success)
						return false;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return success;
}

void Connection::SetChosen()
{
	is_chosen_ = true;
}

void Connection::SetUnchosen()
{
	is_chosen_ = false;
}


BranchVect Connection::GetBranchVect()
{
	return branches_;
}

bool Connection::IsConEmpty() const
{
	for (const auto branch : branches_)
	{
		if (!branch.isEmpty())
		{
			return false;
		}
	}
	return true;
}

void Connection::DrawNode(QPainter *painter, QRect node_rect)
{
	painter->setBrush(Qt::black);
	painter->drawEllipse(node_rect);
	painter->setBrush(Qt::white);
	painter->drawEllipse(node_rect.marginsRemoved(QMargins(4, 4, 4, 4)));
}

void Connection::MakeBeauty()
{
	auto* con = prom_->GetConById(id_ / 100);
	QVector<QPointF> vect = con->GetVectCloseRect();
	start_poses_.clear();
	start_poses_.resize(vect.size());
	QVector<QPointF> new_vect{};
	new_vect.resize(vect.size());
	for (size_t i = 0; i < vect.size(); ++i)
	{
		start_poses_[i].append(mapFromItem(con, vect.at(i)));
		new_vect.append(mapFromItem(con, vect.at(i)));
	}
	int min_x = std::numeric_limits<int>::max();
	int min_y = std::numeric_limits<int>::max();
	int max_x = std::numeric_limits<int>::min();
	int max_y = std::numeric_limits<int>::min();
	for (auto point : new_vect)
	{
		if (point.x() > max_x)
			max_x = point.x();
		if (point.x() < min_x)
			min_x = point.x();
		if (point.y() > max_y)
			max_y = point.y();
		if (point.y() < min_y)
			min_y = point.y();
	}
	min_y -= 8;
	max_y += 8;
	max_x += 32;
	const int width = max_x - min_x + 16;
	const int height = max_y - min_y;
	rect_ = QRect(min_x - 16, min_y, width, height);
	main_rect_ = QRect(QPoint(rect_.right() - 32, rect_.top()), rect_.bottomRight().toPoint());
	rect_inside_ = QRect(rect_.topLeft().toPoint(), QPoint(rect_.right() - 32, rect_.bottom()));
	update();
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//painter->setBrush(Qt::red);
	//painter->drawRect(rect_);
	//painter->setBrush(Qt::yellow);
	//painter->drawRect(main_rect_);
	//painter->setBrush(Qt::cyan);
	//painter->drawRect(rect_inside_);

	if (!is_closing_)
	{
		//--------------------- Связь ---------------------
		const auto hor_line = lines_.at(lines_.size() - 2);
		const auto ver_line = lines_.last();
		const auto first_hor_line = lines_.front();
		const auto last_hor_line = lines_.at(lines_.size() - 3);

		if (is_chosen_)
		{
			painter->setBrush(Qt::red);
			painter->drawRect(QRect(hor_line.p1() - QPoint(0, 3), hor_line.p2() + QPoint(0, 2)));
			painter->drawRect(QRect(ver_line.p1() - QPoint(3, 3), ver_line.p2() + QPoint(3, 2)));
			painter->drawRect(QRect(first_hor_line.p1() - QPoint(-3, 3), first_hor_line.p2() + QPoint(0, 2)));
			painter->drawRect(QRect(last_hor_line.p1() - QPoint(0, 3), last_hor_line.p2() + QPoint(0, 2)));
		}

		painter->setBrush(Qt::black);
		painter->drawLine(hor_line);
		painter->drawLine(ver_line);
		painter->drawLine(first_hor_line);
		painter->drawLine(last_hor_line);

		DrawNode(painter, nodes_.front().second); // 1-й узел

		for (int i = 1; i < branch_count_ - 1; ++i)
		{
			const auto line = lines_.at(i);
			if (is_chosen_)
			{
				painter->setBrush(Qt::red);
				painter->drawRect(QRect(line.p1() - QPoint(0, 3), line.p2() + QPoint(0, 2)));
				painter->setBrush(Qt::black);
			}
			painter->drawLine(line);

			//--------------------- Узлы ---------------------
			DrawNode(painter, nodes_.at(i).second); // i-е узлы
		}
		DrawNode(painter, nodes_.back().second); // Последний узел

		//--------------------- Самый первый узел ---------------------

		if (is_new_tree_)
		{
			const QRect node_rect{ QPoint(rect_.x(), -8), QSize(16, 16) };
			DrawNode(painter, node_rect);
		}
	}
	else //--------------------Если связь является закрывающей-------------------------
	{
		if (start_poses_.isEmpty())
			return;
		auto* con = prom_->GetConById(id_ / 100);
		auto lines = con->GetLines();
		for (auto& line : lines)
		{
			line = QLineF(mapFromItem(con, line.p1()), mapFromItem(con, line.p2())).toLine();
		}
		const QLine hor_line(QPoint(rect_.right() - 24, lines.at(lines.size() - 2).p2().y()), QPoint(rect_.right() - 16, lines.at(lines.size() - 2).p2().y()));
		const QLine ver_line(QPoint(rect_.right() - 24, lines.at(lines.size() - 1).p1().y()), QPoint(rect_.right() - 24, lines.at(lines.size() - 1).p2().y()));
		if (is_chosen_)
		{
			painter->setBrush(Qt::red);
			painter->drawRect(QRect(hor_line.p1() - QPoint(0, 3), hor_line.p2() + QPoint(0, 2)));
			painter->drawRect(QRect(ver_line.p1() - QPoint(3, 3), ver_line.p2() + QPoint(3, 2)));
			painter->setBrush(Qt::black);
		}
		painter->drawLine(hor_line);
		painter->drawLine(ver_line);

		for (auto branch : start_poses_)
		{
			auto point = branch.back();
			if (rect_.right() - 48 > point.x()) // Прямые до узлов
				painter->drawLine(point, QPoint(rect_.right() - 48, point.y()));

			const QLine line(QPoint(rect_.right() - 32, point.y()), QPoint(rect_.right() - 24, point.y()));
			if (is_chosen_)
			{
				painter->setBrush(Qt::red);
				painter->drawRect(QRect(line.p1() - QPoint(0, 3), line.p2() + QPoint(0, 2)));
				painter->setBrush(Qt::black);
			}
			painter->drawLine(line); // Черты от узлов до вертикальной черты

			const QRect node_rect{ QPoint(rect_.right() - 48, point.y() - 8), QSize(16, 16) };
			DrawNode(painter, node_rect); // Узлы связи
		}
		const QRect node_rect{ QPoint(rect_.right() - 16, lines.at(lines.size() - 2).p2().y() - 8), QSize(16, 16) };
		nodes_.append(QPair<int, QRect>(1, node_rect));
		DrawNode(painter, node_rect); // Крайний узел
	}

}