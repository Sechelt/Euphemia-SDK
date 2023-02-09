#ifndef H_PGraphicsScene
#define H_PGraphicsScene

#include "PLayers.h"

/*!
 * \brief A graphics scene with just one item - a PLayers. 
 *  
 * The layers is added to the scene as a QWidget. 
 * The layers will take up the entire scene. 
 * The viewport (PGraphicsView) may show a larger area than scene - in this case 
 * the extra area will not be usable and will be light grey.
 * 
 * \author pharvey (2/8/23)
 */
class PGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PGraphicsScene( PLayers *pLayers, const QRectF &r, QObject *pObject = nullptr );
    ~PGraphicsScene();

    PLayers *   getLayers() { return pLayers; }
    PCanvas *   getCanvas();

protected:
    PLayers *pLayers;
};

#endif


