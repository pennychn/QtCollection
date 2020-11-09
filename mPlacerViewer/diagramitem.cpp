#include "diagramitem.h"

DiagramItem::DiagramItem(DiagramType diagramType, const Net *_net) {
	// ���󪺮y�Цb(0, 0)
	if (diagramType == FF) {
		m_poly << QPointF(-15, -15) << QPointF(15, -15) << QPointF(15, 15)
					 << QPointF(-15, 15) << QPointF(-15, -15);
		} 
	else {
		m_poly << QPointF(0, -15) << QPointF(15, 15) << QPointF(-15, 15)
					 << QPointF(0, -15);
	}
	m_net = _net;
	m_type = diagramType;
	setPolygon(m_poly);
	setFlag(QGraphicsItem::ItemIsSelectable);
}
