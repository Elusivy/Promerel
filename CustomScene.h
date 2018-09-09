#pragma once
#include "qgraphicsscene.h"
#include "QGraphicsSceneMouseEvent"

class CustomScene :
	public QGraphicsScene
{
public:
	CustomScene();
	~CustomScene();

	void mousePressEvent(QGraphicsSceneMouseEvent *ev);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	QPoint myPos;
};

