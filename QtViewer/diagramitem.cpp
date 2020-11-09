#include "diagramitem.h"

DiagramItem::DiagramItem(DiagramType diagramType, QString str) {
	m_name = str;
	if (diagramType == OCTGON) {
		m_poly << QPoint(-5, -10) << QPoint(5, -10)  << QPointF(10, -5)
					 << QPoint(10, 5)  << QPoint(5, 10) << QPointF(-5, 10)
					 << QPoint(-10, 5)  << QPoint(-10, -5) << QPoint(-5, -10);
	} 
	else {
		m_poly << QPointF(0, -15) << QPointF(15, 15) << QPointF(-15, 15)
					 << QPointF(0, -15);
	}
	m_type = diagramType;
	m_net = NULL;
	setPolygon(m_poly);
	setFlag(QGraphicsItem::ItemIsSelectable);

}
DiagramItem::DiagramItem(DiagramType diagramType, const Net *_net) {
	// 物件的座標在(0, 0)
	/*
	switch (diagramType) {
		case POINT:

			break;
		case LINE:
			m_poly << QPointF(0, -15) << QPointF(15, 15) << QPointF(-15, 15)
						 << QPointF(0, -15);
			break;
		case RECT:
			m_poly << QPointF(0, -15) << QPointF(15, 15) << QPointF(-15, 15)
						 << QPointF(0, -15);
			break;
		default:
			
	}
	*/
	if (diagramType == POINT) {
		m_poly << QPointF(-15, -15) << QPointF(15, -15) << QPointF(15, 15)
					 << QPointF(-15, 15) << QPointF(-15, -15);
		} 
	else {
		m_poly << QPointF(0, -15) << QPointF(15, 15) << QPointF(-15, 15)
					 << QPointF(0, -15);
	}
	m_type = diagramType;
	m_net = _net;
	m_name = "unnamed";
	setPolygon(m_poly);
	setFlag(QGraphicsItem::ItemIsSelectable);
}
