#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPolygonItem>
#include "def.h"

class DiagramItem : public QGraphicsPolygonItem
{
	public:
		enum { Type  = UserType + 1 };
                enum DiagramType { INPUT, OUTPUT, POINT, LINE, RECT, FF, OCTGON };
		DiagramItem(DiagramType diagramType, const Net *_net);
		DiagramItem(DiagramType diagramType, QString str);
    int type() const { return Type; }
		inline DiagramType getPinType() { return m_type; }

	private:
		const Net *m_net;
		DiagramType m_type;
		QPolygonF m_poly;
		QString m_name;
};

#endif // DIAGRAMITEM_H
