#include <QPainter>
#include <QRect>
#include <QMessageBox>
#include <qgraphicsscene.h>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <qmath.h>

#include "Element.h"
#include "Promerel.h"
#include "SetElementSettingsView.h"

Element::Element(Promerel *prom, const int id, const bool is_new_tree, const PairInt& bel_con_id_num, const PairInt& prev_body, const QPointF& node_pos, const QString& title, const VectFloat& data) :
QObject{},
QGraphicsItem{},
rect_{ QRectF(QPointF(0, -8), QSizeF(80 + (is_new_tree ? 16 : 0), 16)) },
exit_point_{ QPointF(node_pos.x() + 80 + (is_new_tree ? 16 : 0), node_pos.y()) },
id_{ id },
is_checked_{},
is_fixed_{},
is_chosen_{},
is_new_tree_{ is_new_tree },
bel_con_id_num_{ bel_con_id_num },
prev_object_{ prev_body },
prom_{ prom },
elem_title_{ title },
intensity_{},
time_{},
units_{},
reliability_{},
timeT_{}
{
	ExtractData(data);
	setAcceptHoverEvents(true);
}

Element::Element(Element *el, const int id, const QString& ver) :
QObject{},
QGraphicsItem{},
id_{ id },
rect_{ el->rect_ },
exit_point_{ el->exit_point_ },
is_checked_{},
is_fixed_{},
is_chosen_{},
is_new_tree_{},
bel_con_id_num_{}, //
prev_object_{},    //
prom_{ el->prom_ },
elem_title_{ ver },
intensity_{ el->intensity_ },
time_{ el->time_ },
units_{ el->units_ },
reliability_{ el->reliability_ }
{

}

Element::~Element()
{
}

void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

void Element::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	auto text = QString::fromLocal8Bit("%1").arg(elem_title_);
	int count{};
	if (intensity_)
		++count;
	if (reliability_)
		++count;
	if (timeT_)
		++count;
	if (count)
	{
		--count;
		text += QString("[ ");
		if (GetIntensity())
		{
			text += QString::fromLocal8Bit("Интенсивность отказов: %1").arg(intensity_) + (count ? QString(", ") : QString());
			--count;
		}
		if (GetReliability())
		{
			text += QString::fromLocal8Bit("Надежность: %1").arg(reliability_) + (count ? QString(", ") : QString());
			--count;
		}
		if (GetTimeT())
		{
			text += QString::fromLocal8Bit("Ср. вр. безотк. работы: %1").arg(timeT_) + (count ? QString(", ") : QString());
			--count;
		}
		text += QString(" ]");
	}
	prom_->ShowStatusToolTip(text);
}

void Element::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	prom_->ClearStatusBar();
}

void Element::ExtractData(const VectFloat& data)
{
	if (!data.isEmpty())
	{
		intensity_ = data.at(0);
		time_ = data.at(1);
		units_ = data.at(2);
		reliability_ = data.at(3);
		timeT_ = data.at(4);
		if (timeT_)
			intensity_ = 1 / timeT_;
		if (reliability_)
			intensity_ = log(reliability_) / (-time_);
	}
}

bool Element::IsChecked() const
{
	return is_checked_;
}

void Element::ResetFlag()
{
	is_checked_ = false;
}

bool Element::IsFixed() const
{
	return is_fixed_;
}

void Element::SetFixed()
{
	is_fixed_ = true;
}

void Element::SetUnfixed()
{
	is_fixed_ = false;
}

int Element::GetId() const
{
	return id_;
}

void Element::SetChosen()
{
	is_chosen_ = true;
}

void Element::SetUnchosen()
{
	is_chosen_ = false;
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	is_checked_ = true;
	QGraphicsItem::mousePressEvent(event);
}

void Element::MoveIt(const int delta)
{
	rect_.translate(QPoint(0, delta));
	exit_point_.ry() += delta;
}

QRectF Element::boundingRect() const
{
	return rect_;
}

const QPointF &Element::GetElemsExitPoint() const
{
	return exit_point_;
}

PairInt Element::GetBelConIDNum() const
{
	return bel_con_id_num_;
}

PairInt Element::GetPrevObject() const
{
	return prev_object_;
}

bool Element::HasPrevObj() const
{
	return (prev_object_.first > 0 && prev_object_.second > 0);
}

const QString& Element::GetTitle() const
{
	return elem_title_;
}

double Element::GetIntensity() const
{
	return intensity_;
}

double Element::GetReliability() const
{
	return reliability_;
}

double Element::GetTimeT() const
{
	return timeT_;
}

void Element::ChangeChars()
{
	SetElementSettingsView set(QTEXT("Изменение свойств элемента"));
	set.SetTitle(elem_title_);
	
	VectFloat v{};
	v.push_back(intensity_);
	v.push_back(time_);
	v.push_back(units_);
	v.push_back(reliability_);
	v.push_back(timeT_);
	
	set.SetData(v);
	if (!set.exec())
		return;
	elem_title_ = set.GetElemTitle();
	ExtractData(set.GetData());
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//--------------------- Элемент ------------------

	const QLine left_line(QPoint(rect_.x() + (is_new_tree_ ? 16 : 0), rect_.y() + 8), QPoint(rect_.x() + 16 + (is_new_tree_ ? 16 : 0), rect_.y() + 8));
	const QLine right_line(QPoint(rect_.x() + 48 + (is_new_tree_ ? 16 : 0), rect_.y() + 8), QPoint(rect_.right() - 16, rect_.y() + 8));
	const QRect rect(QRect(QPoint(rect_.x() + 8 + (is_new_tree_ ? 16 : 0), rect_.y()), QSize(48, 16)));
	if (is_chosen_)
	{
		painter->setBrush(Qt::red);
		painter->drawRoundedRect(rect.marginsAdded(QMargins(3, 3, 3, 3)), 20.0, 20.0, Qt::RelativeSize);
		painter->drawRect(QRect(left_line.p1() - QPoint(0, 3), left_line.p2() + QPoint(-3, 2)));
		painter->drawRect(QRect(right_line.p1() - QPoint(-3, 3), right_line.p2() + QPoint(0, 2)));
	}
	painter->setBrush(Qt::black);
	painter->drawLine(left_line);
	painter->drawLine(right_line);
	painter->setBrush(Qt::blue);
	painter->drawRoundedRect(rect, 20.0, 20.0, Qt::RelativeSize);

	//--------------------- Узел ---------------------

	const QRect node_rect{ QPoint(rect_.right() - 16, rect_.y()), QSize(16, 16) };
	painter->setBrush(Qt::black);
	painter->drawEllipse(node_rect);
	painter->setBrush(Qt::white);
	painter->drawEllipse(node_rect.marginsRemoved(QMargins(4, 4, 4, 4)));

	//--------------------- Первый узел ---------------------

	if (is_new_tree_)
	{
		const QRect node_rect{ QPoint(rect_.x(), rect_.y()), QSize(16, 16) };
		painter->setBrush(Qt::black);
		painter->drawEllipse(node_rect);
		painter->setBrush(Qt::white);
		painter->drawEllipse(node_rect.marginsRemoved(QMargins(4, 4, 4, 4)));
	}
}