#include "LibInfo.h"
#include "PDrawPolygon.h"

#include "PCanvas.h"
#include "PPenToolBar.h"

PDrawPolygon::PDrawPolygon( PCanvas *pCanvas, const QPoint &pointBegin )
    : PShapeBase( pCanvas )
{
    // We start of with a single point. 
    // We assume that doPress is called right after we are instantiated and
    // this will result in a second point being appended.
    polygon.append( pointBegin );   // first point

    // set our geometry
    // - make ourself the same size as the canvas to avoid;
    //      - having to map coordinates for many points
    //      - having to call setGeometry too frequently
    setGeometry( 0, 0, pCanvas->geometry().width(), pCanvas->geometry().height() );
    setVisible( true );
}

/*!
 * \brief Switch from drawing mode to manipulating mode.
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
bool PDrawPolygon::doDoubleClick( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( bManipulating ) return false;
    bManipulating = true;
    doCreateHandles();
    update();
    return true;
}

/*!
 * \brief Create a new point (drawing mode) or start to move a point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
bool PDrawPolygon::doPress( QMouseEvent *pEvent )
{
// qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]";
    // manipulating mode
    if ( bManipulating )
    {
        pHandle = getHandle( pEvent->pos() );
        if ( pHandle )
        {
            // hide handles during manipulation
            // doShowHandles( false );
            return true;
        }
        // pressed on the shape (but not a handle)
        if ( geometry().contains( pEvent->pos() ) ) return true;
        // nothing to do here so return false
        // app will probably tell this to doCommit() then delete this
        return false;
    }

    // drawing mode
    // - append a point 
    // - doMove will always be moving the last point so this is the one we will start working with
    polygon.append( pEvent->pos() );
    update();
    return true;
}

/*!
 * \brief Move the last point (drawing mode) or the handle/point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
bool PDrawPolygon::doMove( QMouseEvent *pEvent ) 
{
// qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]";
    // manipulating mode
    if ( bManipulating )
    {
        // IF no handle moving THEN no manipulation
        if ( !pHandle ) return true;
        // move handle and its corresponding point
        doMoveHandle( pEvent->pos() );
        return true;
    }

    // drawing mode
    // - update the last point
    // - adjust our size to size of the polygon
    polygon.setPoint( polygon.count() - 1, pEvent->pos() );
    update();
    return true;
}

bool PDrawPolygon::doRelease( QMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
// qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]";

    // Only relevant when manipulating
    if ( !bManipulating ) return true;
    // IF no handle moving THEN no manipulation
    if ( !pHandle ) return true;

    // update handle/point position
    pHandle = nullptr;

    return true;
}

/*!
 * \brief Commit the polygon to canvas. 
 *  
 * The canvas will probably delete this object after this call. 
 * 
 * \author pharvey (2/1/23)
 */
void PDrawPolygon::doCommit()
{
    QPainter painter( g_Context->getImage() );
    doPaint( &painter, polygon );
}

/*!
 * \brief Paint polygon on self.
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 */
void PDrawPolygon::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED( pEvent );
    QPainter painter( this );
    doPaint( &painter, polygon );
}

/*!
 * \brief Paint the polygon. 
 *  
 * \author pharvey (2/1/23)
 * 
 * \param pPainter 
 * \param polygon 
 */
void PDrawPolygon::doPaint( QPainter *pPainter, const QPolygon &polygon )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setBrush( g_Context->getBrush() );
    pPainter->setFont( g_Context->getFont() );
    // paint
    pPainter->drawPolyline( polygon );                         
//    pPainter->drawPolygon( polygon );                         
}

/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 * It would be easiest to append the move handle at the end as this would mean there would 
 * be a direct correspondence between point indexs and handle indexs - but this would make 
 * the move handle the default when all handles happened to be stacked upon each other. 
 * This would make resizing a very small polygon difficult - so we have the move handle 
 * first (at bottom). 
 *  
 * \author pharvey (2/1/23)
 */
void PDrawPolygon::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    PHandle *pHandle;

    // move handle is always vectorHandles[0]
    pHandle = new PHandle( pCanvas, PHandle::TypeDrag, polygon.boundingRect().center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // add a handle for each point
    for ( QPoint point : polygon )
    {
        pHandle = new PHandle( pCanvas, PHandle::TypeMovePoint, point );
        vectorHandles.append( pHandle );
        pHandle->show();
    }
}

/*!
 * \brief Move current handle.
 *  
 * Only used during manipulation mode. 
 *  
 * The polygon will be moved if move handle otherwise we will move handle/point and 
 * adjust move handle so its center. 
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pointPos 
 */
void PDrawPolygon::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );
    Q_ASSERT( bManipulating );

    // move handle?
    if ( pHandle == vectorHandles[0] )
    {
        QPoint pointDelta = pHandle->getCenter() - pointPos;
        // move points
        for ( int n = 0; n < polygon.count(); n++ )
        {
            polygon.setPoint( n, polygon.at( n ) + pointDelta );
            vectorHandles.at( n + 1 )->doMoveBy( pointDelta.x(), pointDelta.y() );
        }
        // move self
        vectorHandles.at( 0 )->doMoveBy( pointDelta.x(), pointDelta.y() );
        return;
    }

    // point handle
    pHandle->setCenter( pointPos );
    polygon.setPoint( vectorHandles.indexOf( pHandle ) - 1, pointPos );
    vectorHandles.at( 0 )->setCenter( polygon.boundingRect().center() );
    update();
}

//
// PPolygonToolBar
//
PPolygonToolBar::PPolygonToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pLayout->addWidget( new PPenToolBar( this ) );
    pLayout->addStretch( 10 );
}

