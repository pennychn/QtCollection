#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "diagramitem.h"
#include "def.h"



class DiagramItem;
class QGraphicsSceneDragDropEvent;
class QGraphicsViewItem;

class DiagramScene : public QGraphicsScene
{
	 Q_OBJECT
public:
	 DiagramScene(QObject *parent = 0);
	// QGraphicItem

signals:
	 void ItemSelected(const Net *_net, DiagramItem::DiagramType _type);

protected:
	 void mousePressEvent(QGraphicsSceneMouseEvent *event);
	 void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	 QGraphicsItem *movingItem;
	 QPointF oldPos;
};

#endif // DIAGRAMSCENE_H
