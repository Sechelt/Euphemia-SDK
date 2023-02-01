#include "LibInfo.h"

#include "PShapeBase.h"
#include "PCanvas.h"

PShapeBase::PShapeBase( PCanvas *pCanvas )
    : QWidget( pCanvas )
{
    this->pCanvas = pCanvas;
}

PShapeBase::~PShapeBase()
{
    doCancel();
}

void PShapeBase::doCancel()
{
    doDeleteHandles();
}

PHandle *PShapeBase::getHandle( const QPoint &pointPos )
{
    // search in REVERSE order to reflect z-order
    for ( int n = vectorHandles.count() - 1; n >=0; n-- )
    {
        PHandle *p = vectorHandles.at( n );
        if ( p->geometry().contains( pointPos ) ) return p;
    }
    return nullptr;
}

/*!
 * \brief Enlarge rect to account for pen thickness. 
 *  
 * This will prevent lines from being clipped. 
 * 
 * \author pharvey (1/26/23)
 * 
 * \param r 
 * \param nPenWidth 
 * 
 * \return QRect 
 */
QRect PShapeBase::getGeometry( const QRect &r, int nPenWidth )
{
    int n = (nPenWidth + 1) / 2; // add 1 because a bit larger is ok
    return QRect( r.x() - n, r.y() - n, r.width() + n * 2, r.height() + n * 2 );
}

void PShapeBase::doDeleteHandles()
{
    pHandle = nullptr;
    for ( int n = 0; n < vectorHandles.count(); n++ )
    {
        delete vectorHandles.at( n );
    }
    vectorHandles.clear();
}

void PShapeBase::doShowHandles( bool b )
{
    for ( PHandle *p : vectorHandles )
    {
        if ( b ) p->show(); else p->hide();
    }
}


