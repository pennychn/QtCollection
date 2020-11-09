#include "diagramscene.h"

 DiagramScene::DiagramScene(QObject *parent)
     : QGraphicsScene(parent)
 {
     movingItem = 0;
 }

 void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                      event->buttonDownScenePos(Qt::LeftButton).y());
     movingItem = itemAt(mousePos.x(), mousePos.y());

		DiagramItem *_item = qgraphicsitem_cast<DiagramItem*>(movingItem);
		 if (_item != 0) {
				*pLog->logFile() << "Send data from scene" << std::endl;
				emit ItemSelected(_item->getNet(), _item->getPinType());
     }
     clearSelection();
     QGraphicsScene::mousePressEvent(event);
 }

 void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
	 /*
     if (movingItem != 0 && event->button() == Qt::LeftButton) {
             //emit itemMoved(qgraphicsitem_cast<DiagramItem *>(movingItem),
                            //oldPos);
				*pLog->logFile() << "Send data from scene" << std::endl;
				DiagramItem *_item = (DiagramItem *)movingItem;
				emit ItemSelected(_item->getNet());
				movingItem = 0;
     }
		 */
		movingItem = 0;
    QGraphicsScene::mouseReleaseEvent(event);
 }

