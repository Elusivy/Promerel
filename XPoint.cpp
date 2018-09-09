#include "XPoint.h"


XPoint::XPoint(QWidget *parent, QPoint pos) :
QWidget(parent),
QGraphicsItem{},
pos_{ pos },
rect_{ QRect(pos - QPoint(5, 5), QSize(10, 10)) }
{
	setAcceptHoverEvents(true);
}

XPoint::~XPoint()
{
}

QPoint XPoint::GetPos() const
{
	return pos_;
}

void XPoint::SetPos(QPoint pos)
{
	pos_ = pos;
}

QRectF XPoint::boundingRect() const
{
	return rect_;
}

void XPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setBrush(Qt::black);
	painter->drawLine(pos_ - QPoint(5, 0), pos_ + QPoint(5, 0));
	painter->drawLine(pos_ - QPoint(0, 5), pos_ + QPoint(0, 5));
}