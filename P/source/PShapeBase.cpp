#include "LibInfo.h"

#include "PShapeBase.h"
#include "PCanvas.h"

PShapeBase::PShapeBase( PCanvas *pCanvas )
    : QWidget( pCanvas )
{
    this->pCanvas = pCanvas;
    // cover entire canvas - we are a temp layer
    setGeometry( 0, 0, pCanvas->width(), pCanvas->height() );
    setVisible( true );
}

PShapeBase::~PShapeBase()
{
}

void PShapeBase::doCancel()
{
    if ( nState != StateIdle ) doIdle();
}

bool PShapeBase::canCommit()
{
    return nState == StateManipulate;
}

bool PShapeBase::canCancel() 
{
    return nState == StateManipulate;
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
        p->setVisible( b );
    }
}


