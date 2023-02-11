#ifndef H_PGraphicsScene
#define H_PGraphicsScene

#include "PBackground.h"
#include "PCanvas.h"

/*!
 * \brief A graphics scene.
 *  
 * \author pharvey (2/8/23)
 */
class PGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PGraphicsScene( const QRectF &r, QObject *pObject = nullptr );
    ~PGraphicsScene();

    PBackground *   getBackground() { return pBackground;   }
    PCanvas *       getCanvas()     { return pCanvas;       }

protected:
    PBackground *   pBackground;
    PCanvas *       pCanvas;

    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *pEvent );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *pEvent );
};

#endif


