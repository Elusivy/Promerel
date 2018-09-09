#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <QWidget>

class XPoint :
	public QWidget, public QGraphicsItem
{
public:
	XPoint(QWidget *parent, QPoint pos);
	~XPoint();

	QPoint GetPos() const;
	void SetPos(QPoint pos);

private:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

	QPoint pos_;
	QRect rect_;
};

