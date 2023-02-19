#ifndef H_PFillGradientLinear
#define H_PFillGradientLinear

#include "PShapeBase.h"

/*!
 * \brief Fill using linear gradient. 
 *  
 * Starts in idle mode. 
 *  
 * The first doPress specifies the seed for the fill - the color which will get filled in. 
 * A handle is created - it can be moved. 
 * An out line of the fill area is created as a polygon. 
 * A start and a stop handle is created with a line between. 
 *  
 * Enters manipulation mode. 
 *  
 * The start/stop handles can be moved as desired. 
 *  
 * Returns to idle. 
 *  
 * Clicking other than on the start/stop signifies accept (Esc cancels). In either case; return to idle. 
 * Accept causes the fill to occur. 
 * 
 * \author pharvey (2/16/23)
 */
class PFillGradientLinear : public PShapeBase
{
    Q_OBJECT
public:
    PFillGradientLinear( PCanvas *pCanvas );
    virtual ~PFillGradientLinear();

    virtual QImage getCopy() override;

    virtual void doPaint( QPainter * );
    virtual void doDoubleClick( PMouseEvent * ) override;
    virtual void doPress( PMouseEvent * ) override;
    virtual void doMove( PMouseEvent * ) override; 
    virtual void doRelease( PMouseEvent * ) override;
    virtual void doCommit() override;

protected:
    QPoint      pointSeed;
    QPoint      pointStart;
    QPoint      pointStop;
    QPolygon    polygon;

    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );

    QPolygon getPolygon( const QPoint &point );

    QPoint getLeft( const QPoint &point ) { return QPoint( point.y(), -point.x() ); }
    QPoint getRight( const QPoint &point ) { return QPoint( -point.y(), point.x() ); }

    bool isBoundary( QImage *pImage, const QColor &color, const QPoint &point )
    {
        return ( point.x() < 0 || point.x() >= pImage->size().width() || point.y() < 0 || point.y() >= pImage->size().height() || pImage->pixelColor( point ) != color );
    }
};

#endif
