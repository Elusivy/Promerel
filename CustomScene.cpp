#include "CustomScene.h"

CustomScene::CustomScene()
{
}


CustomScene::~CustomScene()
{
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	myPos = ev->scenePos().toPoint();
	QGraphicsScene::mousePressEvent(ev);
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	QGraphicsScene::mousePressEvent(ev);
}

void CustomScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
	QGraphicsScene::mouseDoubleClickEvent(ev);
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
	QGraphicsScene::mouseMoveEvent(ev);
}

void CustomScene::hoverMoveEvent(QGraphicsSceneHoverEvent *ev)
{
}
