/*
 * Copyright 2022-2023, Peter Harvey <pharvey@codebydesign.com>
 *
 * This file is part of Euphemia-SDK.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "LibInfo.h"
#include "PFillGradient.h"

#include <WColorPickerDialog.h>

#include "PContext.h"
#include "PCanvas.h"

// constant index values for vectorHandles
#define PFillGradientFactory 0                      /*!< indicates fill area, can create/delete intermediate handles, is center for radial gradient */

#define PFillGradientLinearStart 1
#define PFillGradientLinearStop 2
#define PFillGradientLinearIntermediates 3          /*!< index of first (if any) intermediate stop point handle                                     */

#define PFillGradientRadialFocal 1
#define PFillGradientRadialRadius 2
#define PFillGradientRadialIntermediates 3          /*!< index of first (if any) intermediate stop point handle                                     */

#define PFillGradientConicalAngle 1
#define PFillGradientConicalIntermediates 2         /*!< index of first (if any) intermediate stop point handle                                     */

//
//
//
PFillGradient::PFillGradient( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
    connect( g_Context, SIGNAL(signalModified(const PContextGradient &)), SLOT(slotRefresh(const PContextGradient &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientLinear &)), SLOT(slotRefresh(const PContextGradientLinear &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientRadial &)), SLOT(slotRefresh(const PContextGradientRadial &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientConical &)), SLOT(slotRefresh(const PContextGradientConical &)) );
}

PFillGradient::~PFillGradient()
{
    doCancel();
}

/*!
 * \brief Return a copy of the CURRENT SHAPE.
 *  
 * This is done by getting doPaint to draw on an empty, temp, canvas and then 
 * copying from that canvas. 
 *  
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PFillGradient::getCopy()
{
    QImage image( g_Context->getImage()->size(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    doPaint( &painter );

    return image.copy( polygon.boundingRect() );
}

/*!
 * \brief Switch from drawing mode to manipulating mode.
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return QRect 
 */
void PFillGradient::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
    if ( nState != StateManipulate ) return;                                // does not make sense unless we are in manipulate state as there would be no handles
    if ( nType >= 0 ) return;                                               // not relevant for a preset
    PHandle *pHandle = getHandle( pEvent->pos() );
    if ( !pHandle ) return;                                                 // we are only going to process a double click on a handle

    if ( nType == PContextGradient::StandardGradientLinear )
    {
        if ( pHandle == vectorHandles[PFillGradientLinearStart] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStart.second, 0, true );
            if ( !bOk ) return;
            stopStart.second = color;
            update();
            return;
        }
        if ( pHandle == vectorHandles[PFillGradientLinearStop] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStop.second, 0, true );
            if ( !bOk ) return;
            stopStop.second = color;
            update();
            return;
        }
        // must be a stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientLinearIntermediates;
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stops[n].second, 0, true );
            if ( !bOk ) return;
            stops[n].second = color;
            update();
            return;
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        if ( pHandle == vectorHandles[PFillGradientFactory] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStart.second, 0, true );
            if ( !bOk ) return;
            stopStart.second = color;
            update();
            return;
        }

        if ( pHandle == vectorHandles[PFillGradientRadialRadius] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStop.second, 0, true );
            if ( !bOk ) return;
            stopStop.second = color;
            update();
            return;
        }
        if ( pHandle == vectorHandles[PFillGradientRadialFocal] )
        {
            return;
        }
        // must be a stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientLinearIntermediates;
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stops[n].second, 0, true );
            if ( !bOk ) return;
            stops[n].second = color;
            update();
            return;
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        if ( pHandle == vectorHandles[PFillGradientFactory] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStart.second, 0, true );
            if ( !bOk ) return;
            stopStart.second = color;
            update();
            return;
        }

        if ( pHandle == vectorHandles[PFillGradientConicalAngle] )
        {
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stopStop.second, 0, true );
            if ( !bOk ) return;
            stopStop.second = color;
            update();
            return;
        }
        // must be a stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientConicalIntermediates;
            bool bOk = false;
            QColor color = WColorPickerDialog::getColor( &bOk, stops[n].second, 0, true );
            if ( !bOk ) return;
            stops[n].second = color;
            update();
            return;
        }
        return;
    }
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
void PFillGradient::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

void PFillGradient::doMove( PMouseEvent *pEvent ) 
{
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PFillGradient::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle )
        {
            if ( shouldRemoveStop() ) 
            {
                doRemoveStop();
                update();
            }
        }
        pHandle = nullptr;
        break;
    }
}

void PFillGradient::doCommit()
{
    Q_ASSERT( nState == StateManipulate );

    QPainter painter( g_Context->getImage() );
    doPaint( &painter );

    emit signalCommitted();
    doIdleState();
}

void PFillGradient::slotRefresh( const PContextGradient &t )
{
    // this should not happen but...
    if ( nState != StateManipulate ) return;

    // changed gradient type so...
    if ( t.nType != nType )
    {
        doCancel();
        return;
    }
}

void PFillGradient::slotRefresh( const PContextGradientLinear &t )
{
}

void PFillGradient::slotRefresh( const PContextGradientRadial &t )
{
}

void PFillGradient::slotRefresh( const PContextGradientConical &t )
{
}


void PFillGradient::doPaint( QPainter *pPainter, bool bCommit )
{
    if ( nState != StateManipulate ) return;

    if ( nType >= 0 )
    {
        QGradient   gradient( (QGradient::Preset)nType );
        gradient.setSpread( g_Context->getGradient().nSpread );
        pPainter->setBrush( QBrush( gradient ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
        return;
    }

    // re. intermediate stops
    // - we do not use gradient.setStops( stops ); because that would have to be in asc order by qreal value
    // - our stops are not sorted - their index position is important for our use in other areas
    // - so we add them one-at-a-time.


    if ( nType == PContextGradient::StandardGradientLinear )
    {
        QLinearGradient gradient( linear.pointStart, linear.pointStop );
        gradient.setSpread( g_Context->getGradient().nSpread );

        // add intermediate stops
        gradient.setColorAt( stopStart.first, stopStart.second );
        for ( QGradientStop stop : stops )
        {
            gradient.setColorAt( stop.first, stop.second );
        }
        gradient.setColorAt( stopStop.first, stopStop.second );

        pPainter->setBrush( QBrush( gradient ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );

        if ( !bCommit )
        {
            QPen pen( Qt::darkGray );
            pen.setStyle( Qt::DashLine );
            pPainter->setPen( pen );
            pPainter->drawLine( linear.pointStart, linear.pointStop );
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        qreal nRadius = qAbs( pointFactory.x() - radialSimple.pointCenterRadius.x() );
        QRadialGradient gradient( radialSimple.pointCenter, nRadius, radialSimple.pointFocal );
        gradient.setSpread( g_Context->getGradient().nSpread );

        // add intermediate stops
        gradient.setColorAt( stopStart.first, stopStart.second );       // center
        for ( QGradientStop stop : stops )
        {
            gradient.setColorAt( stop.first, stop.second );
        }
        gradient.setColorAt( stopStop.first, stopStop.second );         // focal point

        pPainter->setBrush( QBrush( gradient ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );

        if ( !bCommit )
        {
            QPen pen( Qt::darkGray );
            pen.setStyle( Qt::DashLine );
            pPainter->setPen( pen );
            pPainter->drawLine( radialSimple.pointCenter, radialSimple.pointCenterRadius );
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        // the origin of the polar coordinates is pointCenter.
        // get pointAngle relative to polar origin
        QPointF pointAngle = conical.pointAngle - conical.pointCenter;
        // get polar coordinate to get radians and then convert radians to degrees
        Polar polar = getCartesianToPolar( pointAngle.x(), pointAngle.y() );
        qreal nDegrees = getRadiansToDegrees( polar.angle );
        // init gradient
        QConicalGradient gradient( conical.pointCenter, nDegrees );

        // add intermediate stops
        gradient.setColorAt( stopStart.first, stopStart.second );
        for ( QGradientStop stop : stops )
        {
            gradient.setColorAt( stop.first, stop.second );
        }
        gradient.setColorAt( stopStop.first, stopStop.second );  

        pPainter->setBrush( QBrush( gradient ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );

        if ( !bCommit )
        {
            QPen pen( Qt::darkGray );
            pen.setStyle( Qt::DashLine );
            pPainter->setPen( pen );
            pPainter->drawLine( conical.pointCenter, conical.pointAngle );
        }
        return;
    }
}

void PFillGradient::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    // pointFactory 
    // - indicates the fill area. 
    // - its handle can not be moved.
    // - can produce intermediate 'stop' points when an attempt is made to drag it.
    pointFactory = point;

    nType = g_Context->getGradient().nType;
    if ( nType >= 0 )
    {
    }
    else if ( nType == PContextGradient::StandardGradientLinear )
    {
        // pointStart is the [Start]
        // pointStop is the [Stop]
        // intermediate points will be between [Start] and [Stop]
        stopStart.first = 0;
        stopStart.second = QColor( Qt::black );
        stopStop.first = 1;
        stopStop.second = QColor( Qt::white );

        linear.pointStart = linear.pointStop = point;
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        // pointCenter is set but can not be moved. it is the [Start] 
        // pointCenterRadius can be moved. is is the [Stop]
        // intermediate points will be between [Start] and [Stop]
        // pointFocal can be moved (should stay within the 'center circle' and has no color associated with it
        stopStart.first = 0;
        stopStart.second = QColor( Qt::black );
        stopStop.first = 1;
        stopStop.second = QColor( Qt::white );

        radialSimple.pointCenter = point;
        radialSimple.pointCenterRadius = point;
        radialSimple.pointFocal = point;
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        // pointCenter is set but can not be moved. it is the [Start] 
        // pointAngle can be moved. is is the [Stop]
        // intermediate points will be between [Start] and [Stop]
        stopStart.first = 0;
        stopStart.second = QColor( Qt::black );
        stopStop.first = 1;
        stopStop.second = QColor( Qt::white );

        conical.pointCenter = point;
        conical.pointAngle  = point;
    }
    else
    {
        Q_ASSERT( 1==0 );
    }

    // polygon
    // - this is an outline of the area to be filled
    polygon = getPolygon( pointFactory );

    nState = StateDraw;
    doManipulateState();
}

void PFillGradient::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PFillGradient::doIdleState()
{
    Q_ASSERT( nState == StateManipulate );

    doDeleteHandles();
    stops.clear();
    vectorStopPoints.clear();
    polygon.clear();
    nType = 0;
    polygon.clear();
    nState = StateIdle;
    update();
    emit signalChangedState();
}


/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 */
void PFillGradient::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    if ( nType == PContextGradient::StandardGradientLinear )
    {
        pHandle = new PHandle( pView, PHandle::TypePointFactory, pView->mapFromScene( pointFactory ), PHandle::ShapeSquare, QColor( Qt::white ) );
        pHandle->setToolTip( tr("Drag to create a new point.") );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeGradientStart, pView->mapFromScene( linear.pointStart ), PHandle::ShapeCircle, QColor( Qt::darkGreen ) );
        pHandle->setToolTip( tr("Start point.\nDouble-Click to set color.\nDrop point here to remove.") );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeGradientStop, pView->mapFromScene( linear.pointStop ), PHandle::ShapeCircle, QColor( Qt::darkGreen )  );
        pHandle->setToolTip( tr("Stop point.\nDouble-Click to set color.\nDrop point here to remove.") );
        vectorHandles.append( pHandle );
        pHandle->show();
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        pHandle = new PHandle( pView, PHandle::TypePointFactory, pView->mapFromScene( pointFactory ), PHandle::ShapeSquare, QColor( Qt::darkGreen ) );
        pHandle->setToolTip( tr("Center\nDouble-Click to set color.\nDrag to create a new point.\nDrop point here to remove.") );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeGradientFocal, pView->mapFromScene( radialSimple.pointFocal ), PHandle::ShapeCircle, QColor( Qt::darkGray ) );
        pHandle->setToolTip( tr("Focal point") );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeGradientRadius, pView->mapFromScene( radialSimple.pointCenterRadius ), PHandle::ShapeCircle, QColor( Qt::darkGreen ) );
        pHandle->setToolTip( tr("Radius\nDouble-Click to set color.\nDrop point here to remove.") );
        vectorHandles.append( pHandle );
        pHandle->show();
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        pHandle = new PHandle( pView, PHandle::TypePointFactory, pView->mapFromScene( conical.pointCenter ), PHandle::ShapeSquare, QColor( Qt::darkGreen ) );
        // pHandle->setToolTip( tr("Drag to create a new point.\nDrop point here to remove.") );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeGradientAngle, pView->mapFromScene( conical.pointAngle ), PHandle::ShapeCircle, QColor( Qt::darkGreen ) );
        pHandle->setToolTip( tr("Start/stop angle and radius") );
        vectorHandles.append( pHandle );
        pHandle->show();
        return;
    }
}


// react to zoom
void PFillGradient::doSyncHandles()
{
    vectorHandles[PFillGradientFactory]->setCenter( pView->mapFromScene( pointFactory ) );

    if ( nType == PContextGradient::StandardGradientLinear )
    {
        vectorHandles[PFillGradientLinearStart]->setCenter( pView->mapFromScene( linear.pointStart ) );
        vectorHandles[PFillGradientLinearStop]->setCenter( pView->mapFromScene( linear.pointStop ) );

        for ( int n = 0; n < vectorStopPoints.count(); n++ )
        {
            vectorHandles[PFillGradientLinearIntermediates + n]->setCenter( pView->mapFromScene( vectorStopPoints[n] ) );
        }
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        vectorHandles[PFillGradientRadialRadius]->setCenter( pView->mapFromScene( radialSimple.pointCenterRadius ) );
        vectorHandles[PFillGradientRadialFocal]->setCenter( pView->mapFromScene( radialSimple.pointFocal ) );

        for ( int n = 0; n < vectorStopPoints.count(); n++ )
        {
            vectorHandles[PFillGradientRadialIntermediates + n]->setCenter( pView->mapFromScene( vectorStopPoints[n] ) );
        }
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        vectorHandles[PFillGradientConicalAngle]->setCenter( pView->mapFromScene( conical.pointAngle ) );
    }
}

void PFillGradient::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    QPoint pointViewPos = pView->mapFromScene( pointPos );

    // standard handles
    if ( nType == PContextGradient::StandardGradientLinear )
    {
        if ( pHandle == vectorHandles[PFillGradientLinearStart] )
        {
            pHandle->setCenter( pointViewPos );
            linear.pointStart = pointPos;
            doUpdateStops();
            update();
            return;
        }
        if ( pHandle == vectorHandles[PFillGradientLinearStop] )
        {
            pHandle->setCenter( pointViewPos );
            linear.pointStop = pointPos;
            doUpdateStops();
            update();
            return;
        }
        // create intermediate stop point
        if ( pHandle == vectorHandles[PFillGradientFactory] )
        {
            // create handle
            pHandle = new PHandle( pView, PHandle::TypeGradientIntermediate, pView->mapFromScene( pointFactory ), PHandle::ShapeCircle, QColor( Qt::darkYellow ) );
            pHandle->setToolTip( tr("Color point.\nDouble-Click to set color.\nDrop on Start/Stop handle to delete.") );
            pHandle->show();
            // add point
            vectorStopPoints.append( pointFactory );
            // add stop
            stops.append( QGradientStop() );
            // add handle
            vectorHandles.append( pHandle );
            // ensure point and handle on line
            // set qreal in stop
            doUpdateStop( vectorStopPoints.count() - 1 );
            update();
            return;
        }
        // move intermediate stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientLinearIntermediates;
            vectorStopPoints[n] = pointPos;
            doUpdateStop( n );
            update();
            return;
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        if ( pHandle == vectorHandles[PFillGradientRadialRadius] )
        {
            pHandle->setCenter( pointViewPos );
            radialSimple.pointCenterRadius = pointPos;
            doUpdateStops();
            update();
            return;
        }
        if ( pHandle == vectorHandles[PFillGradientRadialFocal] )
        {
            pHandle->setCenter( pointViewPos );
            radialSimple.pointFocal = pointPos;
            doUpdateStops();
            update();
            return;
        }
        // create intermediate stop point
        if ( pHandle == vectorHandles[PFillGradientFactory] )
        {
            // create handle
            pHandle = new PHandle( pView, PHandle::TypeGradientIntermediate, pView->mapFromScene( pointFactory ), PHandle::ShapeCircle, QColor( Qt::darkYellow ) );
            pHandle->setToolTip( tr("Color point.\nDouble-Click to set color.\nDrop on Start/Stop handle to delete.") );
            pHandle->show();
            // add point
            vectorStopPoints.append( pointFactory );
            // add stop
            stops.append( QGradientStop() );
            // add handle
            vectorHandles.append( pHandle );
            // ensure point and handle on line
            // set qreal in stop
            doUpdateStop( vectorStopPoints.count() - 1 );
            update();
            return;
        }
        // move intermediate stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientRadialIntermediates;
            vectorStopPoints[n] = pointPos;
            doUpdateStop( n );
            update();
            return;
        }
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        if ( pHandle == vectorHandles[PFillGradientConicalAngle] )
        {
            pHandle->setCenter( pointViewPos );
            conical.pointAngle = pointPos;
            doUpdateStops();
            update();
            return;
        }
        // create intermediate stop point
        if ( pHandle == vectorHandles[PFillGradientFactory] )
        {
            // create handle
            pHandle = new PHandle( pView, PHandle::TypeGradientIntermediate, pView->mapFromScene( pointFactory ), PHandle::ShapeCircle, QColor( Qt::darkYellow ) );
            pHandle->setToolTip( tr("Color point.\nDouble-Click to set color.\nDrop on Start/Stop handle to delete.") );
            pHandle->show();
            // add point
            vectorStopPoints.append( pointFactory );
            // add stop
            stops.append( QGradientStop() );
            // add handle
            vectorHandles.append( pHandle );
            // ensure point and handle on line
            // set qreal in stop
            doUpdateStop( vectorStopPoints.count() - 1 );
            update();
            return;
        }
        // move intermediate stop point
        {
            int n = vectorHandles.indexOf( pHandle ) - PFillGradientConicalIntermediates;
            vectorStopPoints[n] = pointPos;
            doUpdateStop( n );
            update();
            return;
        }
    }
}

// assumes the vectorHandles, stops, and vectorStopPoints have point represented
void PFillGradient::doUpdateStop( int n )
{
    // ensure that all of our vectors are in harmony
    Q_ASSERT( n >= 0 );
    Q_ASSERT( n < vectorStopPoints.count() );
    Q_ASSERT( n < stops.count() );
    Q_ASSERT( vectorStopPoints.count() == stops.count() );


    if ( nType == PContextGradient::StandardGradientLinear )
    {
        Q_ASSERT( vectorHandles.count() - PFillGradientLinearIntermediates == stops.count() ); 

        QPointF pointOnLine = getNearestPointOnLine( vectorStopPoints[n], linear.pointStart, linear.pointStop );

        qreal nDistance             = getDistance( linear.pointStart, linear.pointStop );
        qreal nDistanceFromStart    = getDistance( linear.pointStart, pointOnLine );
        stops[n].first = nDistanceFromStart / nDistance; // value should always be 0-1
        vectorStopPoints[n] = pointOnLine.toPoint();
        vectorHandles[PFillGradientLinearIntermediates + n]->setCenter( pView->mapFromScene( pointOnLine ) );
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        Q_ASSERT( vectorHandles.count() - PFillGradientRadialIntermediates == stops.count() ); 

        QPointF pointOnLine = getNearestPointOnLine( vectorStopPoints[n], radialSimple.pointCenter, radialSimple.pointCenterRadius );

        qreal nDistance             = getDistance( radialSimple.pointCenter, radialSimple.pointCenterRadius );
        qreal nDistanceFromStart    = getDistance( radialSimple.pointCenter, pointOnLine );
        stops[n].first = nDistanceFromStart / nDistance; // value should always be 0-1
        vectorStopPoints[n] = pointOnLine.toPoint();
        vectorHandles[PFillGradientRadialIntermediates + n]->setCenter( pView->mapFromScene( pointOnLine ) );
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        Q_ASSERT( vectorHandles.count() - PFillGradientConicalIntermediates == stops.count() ); 

        QPointF pointOnLine = getNearestPointOnLine( vectorStopPoints[n], conical.pointCenter, conical.pointAngle );

        qreal nDistance             = getDistance( conical.pointCenter, conical.pointAngle );
        qreal nDistanceFromStart    = getDistance( conical.pointCenter, pointOnLine );
        stops[n].first = nDistanceFromStart / nDistance; // value should always be 0-1
        vectorStopPoints[n] = pointOnLine.toPoint();
        vectorHandles[PFillGradientConicalIntermediates + n]->setCenter( pView->mapFromScene( pointOnLine ) );
        return;
    }
}

// start or 'final' stop have moved so...
// - assumes we are only doing linear at the moment
void PFillGradient::doUpdateStops()
{
    for ( int n = 0; n < vectorStopPoints.count(); n++ )
    {
        doUpdateStop( n );
    }
}

// call shouldRemoveStop() before calling here
void PFillGradient::doRemoveStop()
{
    if ( nType == PContextGradient::StandardGradientLinear )
    {
        // this is based off of a handle being dropped so...
        if ( !pHandle ) return;
        // we can only remove TypeGradientIntermediate handles
        if ( pHandle->getType() != PHandle::TypeGradientIntermediate ) return;

        // remove handle
        int nHandle = vectorHandles.indexOf( pHandle );
        delete vectorHandles.takeAt( nHandle );
        // remove from stops
        stops.remove( nHandle - PFillGradientLinearIntermediates );
        vectorStopPoints.remove( nHandle - PFillGradientLinearIntermediates );
        return;
    }

    if ( nType == PContextGradient::StandardGradientRadial )
    {
        // this is based off of a handle being dropped so...
        if ( !pHandle ) return;
        // we can only remove TypeGradientIntermediate handles
        if ( pHandle->getType() != PHandle::TypeGradientIntermediate ) return;

        // remove handle
        int nHandle = vectorHandles.indexOf( pHandle );
        delete vectorHandles.takeAt( nHandle );
        // remove from stops
        stops.remove( nHandle - PFillGradientRadialIntermediates );
        vectorStopPoints.remove( nHandle - PFillGradientRadialIntermediates );
        return;
    }

    if ( nType == PContextGradient::StandardGradientConical )
    {
        // this is based off of a handle being dropped so...
        if ( !pHandle ) return;
        // we can only remove TypeGradientIntermediate handles
        if ( pHandle->getType() != PHandle::TypeGradientIntermediate ) return;

        // remove handle
        int nHandle = vectorHandles.indexOf( pHandle );
        delete vectorHandles.takeAt( nHandle );
        // remove from stops
        stops.remove( nHandle - PFillGradientConicalIntermediates );
        vectorStopPoints.remove( nHandle - PFillGradientConicalIntermediates );
        return;
    }
}

bool PFillGradient::shouldRemoveStop()
{
    // this is based off of a handle being dropped so...
    if ( !pHandle ) return false;
    // we can only remove TypeGradientIntermediate handles
    if ( pHandle->getType() != PHandle::TypeGradientIntermediate ) return false;
    // deleted by dropping handle on a TypePointFactory handle (same place it was created)
    if ( nType == PContextGradient::StandardGradientLinear )
    {
        if ( getHandleUnder( pHandle, PHandle::TypeGradientStart ) ) return true;
        if ( getHandleUnder( pHandle, PHandle::TypeGradientStop ) ) return true;
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        if ( getHandleUnder( pHandle, PHandle::TypeGradientRadius ) ) return true;
        if ( getHandleUnder( pHandle, PHandle::TypePointFactory ) ) return true;
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        if ( getHandleUnder( pHandle, PHandle::TypeGradientAngle ) ) return true;
        if ( getHandleUnder( pHandle, PHandle::TypePointFactory ) ) return true;
    }

    return false;
}

/*!
 * \brief Returns a polygon representing a boundary. 
 *  
 * The boundary is defined by not being the color at pointFactory starting at pointFactory. 
 *  
 * Uses the 'Square tracing algorithm'. 
 *  
 * \author pharvey (2/17/23)
 * 
 * \param point  
 * 
 * \return QPolygon The inside of the polygon is the area outlined.
 */
QPolygon PFillGradient::getPolygon( const QPoint &pointFactory )
{
    QImage *    pImage      = g_Context->getImage();
    int         nX          = pointFactory.x();
    int         nY          = pointFactory.y();
    QColor      colorSeed   = pImage->pixelColor( pointFactory );
    QPolygon    polygon;

    // go west until we hit a boundary (or go off image)
    do
    {
        nX--;
        if ( isBoundary( pImage, colorSeed, QPoint( nX, nY ) ) ) break; 
    } while ( 1 );

    // start point
    QPoint pointStart( nX, nY );
    polygon.append( pointStart );

    //
    QPoint pointStep = getLeft( QPoint( 0, 1 ) );               
    QPoint point = pointStart + pointStep;
    while ( point != pointStart )
    {
        if ( isBoundary( pImage, colorSeed, point ) )
        {
            polygon.append( point );
            pointStep = getLeft( pointStep );
            point = point + pointStep;
        }
        else
        {
            point = point - pointStep;
            pointStep = getRight( pointStep );
            point = point + pointStep;
        }
    }

    return polygon;
}

QPointF PFillGradient::getPolarToCartesian( qreal nRadius, qreal nAngle )
{
    return QPointF( nRadius * qCos( nAngle ), nRadius * qSin( nAngle ) );
}

PFillGradient::Polar PFillGradient::getCartesianToPolar( qreal x, qreal y )
{
    Polar p;

    // θ = tan-1 ( y / x )
    p.angle = qAtan2( y, x );
    if ( p.angle < 0 )
    {
        p.angle += (2* M_PI);
    }

    // r = √ ( x2 + y2 )
    p.radius = qSqrt( (x*x) + (y*y) );

    return p;
}

QPointF PFillGradient::getNearestPointOnLine( const QPointF &P, const QPointF &A, const QPointF &B )
{
    QPointF a_to_p;
    QPointF a_to_b;

    a_to_p.setX( P.x() - A.x() );
    a_to_p.setY( P.y() - A.y() ); //     # Storing vector A->P  
    a_to_b.setX( B.x() - A.x() );
    a_to_b.setY( B.y() - A.y() ); //     # Storing vector A->B

    qreal atb2 = a_to_b.x() * a_to_b.x() + a_to_b.y() * a_to_b.y();
    qreal atp_dot_atb = a_to_p.x() * a_to_b.x() + a_to_p.y() * a_to_b.y(); // The dot product of a_to_p and a_to_b
    qreal t = atp_dot_atb / atb2;  //  # The normalized "distance" from a to the closest point

    qreal nX = A.x() + a_to_b.x() * t;
    qreal nY = A.y() + a_to_b.y() * t;

    // on an infinite line so lets restrict to between A and B on that line
    if (  A.x() >= B.x() )
    {
         if ( nX > A.x() ) nX = A.x();
         if ( nX < B.x() ) nX = B.x();
    }
    if (  A.y() >= B.y() )
    {
         if ( nY > A.y() ) nY = A.y();
         if ( nY < B.y() ) nY = B.y();
    }
     
    return QPointF( nX, nY );
}

qreal PFillGradient::getDistance( const QPointF &pointA, const QPointF &pointB )
{
    return sqrt(pow(pointB.x() - pointA.x(), 2) + pow(pointB.y() - pointA.y(), 2) * 1.0);
}

qreal PFillGradient::getRadiansToDegrees( qreal nRadian )
{
    return ( nRadian * ( 180 / M_PI ) );
}

//
// PFillGradientToolBar
//
PFillGradientToolBar::PFillGradientToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pType = new QComboBox( this );
    doAddType( "Linear",           PContextGradient::StandardGradientLinear );
    doAddType( "Radial",           PContextGradient::StandardGradientRadial );
    doAddType( "Conical",          PContextGradient::StandardGradientConical );
    doAddType( "WarmFlame",        QGradient::WarmFlame );                          
    doAddType( "NightFade",        QGradient::NightFade );                          
    doAddType( "SpringWarmth",     QGradient::SpringWarmth );                       
    doAddType( "JuicyPeach",       QGradient::JuicyPeach );                         
    doAddType( "YoungPassion",     QGradient::YoungPassion );                       
    doAddType( "LadyLips",         QGradient::LadyLips );                           
    doAddType( "SunnyMorning",     QGradient::SunnyMorning );                       
    doAddType( "RainyAshville",    QGradient::RainyAshville );                      
    doAddType( "FrozenDreams",     QGradient::FrozenDreams );                       
    doAddType( "WinterNeva",       QGradient::WinterNeva );                         
    doAddType( "DustyGrass",       QGradient::DustyGrass );                         
    doAddType( "TemptingAzure",    QGradient::TemptingAzure );                      
    doAddType( "HeavyRain",        QGradient::HeavyRain );                          
    doAddType( "AmyCrisp",         QGradient::AmyCrisp );                           
    doAddType( "MeanFruit",        QGradient::MeanFruit );                          
    doAddType( "DeepBlue",         QGradient::DeepBlue );                           
    doAddType( "RipeMalinka",      QGradient::RipeMalinka );                        
    doAddType( "CloudyKnoxville",  QGradient::CloudyKnoxville );                    
    doAddType( "MalibuBeach",      QGradient::MalibuBeach );                        
    doAddType( "NewLife",          QGradient::NewLife );                            
    doAddType( "TrueSunset",       QGradient::TrueSunset );                         
    doAddType( "MorpheusDen",      QGradient::MorpheusDen );                        
    doAddType( "RareWind",         QGradient::RareWind );                           
    doAddType( "NearMoon",         QGradient::NearMoon );                           
    doAddType( "WildApple",        QGradient::WildApple );                          
    doAddType( "SaintPetersburg",  QGradient::SaintPetersburg );                    
    doAddType( "PlumPlate",        QGradient::PlumPlate );                          
    doAddType( "EverlastingSky",   QGradient::EverlastingSky );                     
    doAddType( "HappyFisher",      QGradient::HappyFisher );                        
    doAddType( "Blessing",         QGradient::Blessing );                           
    doAddType( "SharpeyeEagle",    QGradient::SharpeyeEagle );                      
    doAddType( "LadogaBottom",     QGradient::LadogaBottom );                       
    doAddType( "LemonGate",        QGradient::LemonGate );                          
    doAddType( "ItmeoBranding",    QGradient::ItmeoBranding );                      
    doAddType( "ZeusMiracle",      QGradient::ZeusMiracle );                        
    doAddType( "OldHat",           QGradient::OldHat );                             
    doAddType( "StarWine",         QGradient::StarWine );                           
    doAddType( "HappyAcid",        QGradient::HappyAcid );                          
    doAddType( "AwesomePine",      QGradient::AwesomePine );                        
    doAddType( "NewYork",          QGradient::NewYork );                            
    doAddType( "ShyRainbow",       QGradient::ShyRainbow );                         
    doAddType( "MixedHopes",       QGradient::MixedHopes );                         
    doAddType( "FlyHigh",          QGradient::FlyHigh );                            
    doAddType( "StrongBliss",      QGradient::StrongBliss );                        
    doAddType( "FreshMilk",        QGradient::FreshMilk );                          
    doAddType( "SnowAgain",        QGradient::SnowAgain );                          
    doAddType( "FebruaryInk",      QGradient::FebruaryInk );                        
    doAddType( "KindSteel",        QGradient::KindSteel );                          
    doAddType( "SoftGrass",        QGradient::SoftGrass );                          
    doAddType( "GrownEarly",       QGradient::GrownEarly );                         
    doAddType( "SharpBlues",       QGradient::SharpBlues );                         
    doAddType( "ShadyWater",       QGradient::ShadyWater );                         
    doAddType( "DirtyBeauty",      QGradient::DirtyBeauty );                        
    doAddType( "GreatWhale",       QGradient::GreatWhale );                         
    doAddType( "TeenNotebook",     QGradient::TeenNotebook );                       
    doAddType( "PoliteRumors",     QGradient::PoliteRumors );                       
    doAddType( "SweetPeriod",      QGradient::SweetPeriod );                        
    doAddType( "WideMatrix",       QGradient::WideMatrix );                         
    doAddType( "SoftCherish",      QGradient::SoftCherish );                        
    doAddType( "RedSalvation",     QGradient::RedSalvation );                       
    doAddType( "BurningSpring",    QGradient::BurningSpring );                      
    doAddType( "NightParty",       QGradient::NightParty );                         
    doAddType( "SkyGlider",        QGradient::SkyGlider );                          
    doAddType( "HeavenPeach",      QGradient::HeavenPeach );                        
    doAddType( "PurpleDivision",   QGradient::PurpleDivision );                     
    doAddType( "AquaSplash",       QGradient::AquaSplash );                         
    doAddType( "SpikyNaga",        QGradient::SpikyNaga );                          
    doAddType( "LoveKiss",         QGradient::LoveKiss );                           
    doAddType( "CleanMirror",      QGradient::CleanMirror );                        
    doAddType( "PremiumDark",      QGradient::PremiumDark );                        
    doAddType( "ColdEvening",      QGradient::ColdEvening );                        
    doAddType( "CochitiLake",      QGradient::CochitiLake );                        
    doAddType( "SummerGames",      QGradient::SummerGames );                        
    doAddType( "PassionateBed",    QGradient::PassionateBed );                      
    doAddType( "MountainRock",     QGradient::MountainRock );                       
    doAddType( "DesertHump",       QGradient::DesertHump );                         
    doAddType( "JungleDay",        QGradient::JungleDay );                          
    doAddType( "PhoenixStart",     QGradient::PhoenixStart );                       
    doAddType( "OctoberSilence",   QGradient::OctoberSilence );                     
    doAddType( "FarawayRiver",     QGradient::FarawayRiver );                       
    doAddType( "AlchemistLab",     QGradient::AlchemistLab );                       
    doAddType( "OverSun",          QGradient::OverSun );                            
    doAddType( "PremiumWhite",     QGradient::PremiumWhite );                       
    doAddType( "MarsParty",        QGradient::MarsParty );                          
    doAddType( "EternalConstance", QGradient::EternalConstance );                   
    doAddType( "JapanBlush",       QGradient::JapanBlush );                         
    doAddType( "SmilingRain",      QGradient::SmilingRain );                        
    doAddType( "CloudyApple",      QGradient::CloudyApple );                        
    doAddType( "BigMango",         QGradient::BigMango );                           
    doAddType( "HealthyWater",     QGradient::HealthyWater );                       
    doAddType( "AmourAmour",       QGradient::AmourAmour );                         
    doAddType( "RiskyConcrete",    QGradient::RiskyConcrete );                      
    doAddType( "StrongStick",      QGradient::StrongStick );                        
    doAddType( "ViciousStance",    QGradient::ViciousStance );                      
    doAddType( "PaloAlto",         QGradient::PaloAlto );                           
    doAddType( "HappyMemories",    QGradient::HappyMemories );                      
    doAddType( "MidnightBloom",    QGradient::MidnightBloom );                      
    doAddType( "Crystalline",      QGradient::Crystalline );                        
    doAddType( "PartyBliss",       QGradient::PartyBliss );                         
    doAddType( "ConfidentCloud",   QGradient::ConfidentCloud );                     
    doAddType( "LeCocktail",       QGradient::LeCocktail );                         
    doAddType( "RiverCity",        QGradient::RiverCity );                          
    doAddType( "FrozenBerry",      QGradient::FrozenBerry );                        
    doAddType( "ChildCare",        QGradient::ChildCare );                          
    doAddType( "FlyingLemon",      QGradient::FlyingLemon );                        
    doAddType( "NewRetrowave",     QGradient::NewRetrowave );                       
    doAddType( "HiddenJaguar",     QGradient::HiddenJaguar );                       
    doAddType( "AboveTheSky",      QGradient::AboveTheSky );                        
    doAddType( "Nega",             QGradient::Nega );                               
    doAddType( "DenseWater",       QGradient::DenseWater );                         
    doAddType( "Seashore",         QGradient::Seashore );                           
    doAddType( "MarbleWall",       QGradient::MarbleWall );                         
    doAddType( "CheerfulCaramel",  QGradient::CheerfulCaramel );                    
    doAddType( "NightSky",         QGradient::NightSky );                           
    doAddType( "MagicLake",        QGradient::MagicLake );                          
    doAddType( "YoungGrass",       QGradient::YoungGrass );                         
    doAddType( "ColorfulPeach",    QGradient::ColorfulPeach );                      
    doAddType( "GentleCare",       QGradient::GentleCare );                         
    doAddType( "PlumBath",         QGradient::PlumBath );                           
    doAddType( "HappyUnicorn",     QGradient::HappyUnicorn );                       
    doAddType( "AfricanField",     QGradient::AfricanField );                       
    doAddType( "SolidStone",       QGradient::SolidStone );                         
    doAddType( "OrangeJuice",      QGradient::OrangeJuice );                        
    doAddType( "GlassWater",       QGradient::GlassWater );                         
    doAddType( "NorthMiracle",     QGradient::NorthMiracle );                       
    doAddType( "FruitBlend",       QGradient::FruitBlend );                         
    doAddType( "MillenniumPine",   QGradient::MillenniumPine );                     
    doAddType( "HighFlight",       QGradient::HighFlight );                         
    doAddType( "MoleHall",         QGradient::MoleHall );                           
    doAddType( "SpaceShift",       QGradient::SpaceShift );                         
    doAddType( "ForestInei",       QGradient::ForestInei );                         
    doAddType( "RoyalGarden",      QGradient::RoyalGarden );                        
    doAddType( "RichMetal",        QGradient::RichMetal );                          
    doAddType( "JuicyCake",        QGradient::JuicyCake );                          
    doAddType( "SmartIndigo",      QGradient::SmartIndigo );                        
    doAddType( "SandStrike",       QGradient::SandStrike );                         
    doAddType( "NorseBeauty",      QGradient::NorseBeauty );                        
    doAddType( "AquaGuidance",     QGradient::AquaGuidance );                       
    doAddType( "SunVeggie",        QGradient::SunVeggie );                          
    doAddType( "SeaLord",          QGradient::SeaLord );                            
    doAddType( "BlackSea",         QGradient::BlackSea );                           
    doAddType( "GrassShampoo",     QGradient::GrassShampoo );                       
    doAddType( "LandingAircraft",  QGradient::LandingAircraft );                    
    doAddType( "WitchDance",       QGradient::WitchDance );                         
    doAddType( "SleeplessNight",   QGradient::SleeplessNight );                     
    doAddType( "AngelCare",        QGradient::AngelCare );                          
    doAddType( "CrystalRiver",     QGradient::CrystalRiver );                       
    doAddType( "SoftLipstick",     QGradient::SoftLipstick );                       
    doAddType( "SaltMountain",     QGradient::SaltMountain );                       
    doAddType( "PerfectWhite",     QGradient::PerfectWhite );                       
    doAddType( "FreshOasis",       QGradient::FreshOasis );                         
    doAddType( "StrictNovember",   QGradient::StrictNovember );                     
    doAddType( "MorningSalad",     QGradient::MorningSalad );                       
    doAddType( "DeepRelief",       QGradient::DeepRelief );                         
    doAddType( "SeaStrike",        QGradient::SeaStrike );                          
    doAddType( "NightCall",        QGradient::NightCall );                          
    doAddType( "SupremeSky",       QGradient::SupremeSky );                         
    doAddType( "LightBlue",        QGradient::LightBlue );                          
    doAddType( "MindCrawl",        QGradient::MindCrawl );                          
    doAddType( "LilyMeadow",       QGradient::LilyMeadow );                         
    doAddType( "SugarLollipop",    QGradient::SugarLollipop );                      
    doAddType( "SweetDessert",     QGradient::SweetDessert );                       
    doAddType( "MagicRay",         QGradient::MagicRay );                           
    doAddType( "TeenParty",        QGradient::TeenParty );                          
    doAddType( "FrozenHeat",       QGradient::FrozenHeat );                         
    doAddType( "GagarinView",      QGradient::GagarinView );                        
    doAddType( "FabledSunset",     QGradient::FabledSunset );                       
    doAddType( "PerfectBlue",      QGradient::PerfectBlue );                        
    pType->setCurrentIndex( pType->findData( g_Context->getGradient().nType ) );
    pLayout->addWidget( pType );
    connect( pType, SIGNAL(activated(int)), SLOT(slotType(int)) );

    pSpread = new QComboBox( this );
    pSpread->addItem( "PadSpread", QGradient::PadSpread ); 
    pSpread->addItem( "RepeatSpread", QGradient::RepeatSpread ); 
    pSpread->addItem( "ReflectSpread", QGradient::ReflectSpread ); 
    pSpread->setCurrentIndex( pSpread->findData( g_Context->getGradient().nSpread ) );
    pLayout->addWidget( pSpread );
    connect( pSpread, SIGNAL(activated(int)), SLOT(slotSpread(int)) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextGradient &)), SLOT(slotRefresh(const PContextGradient &)) );
}

void PFillGradientToolBar::slotRefresh( const PContextGradient &t )
{
    pType->setCurrentIndex( pType->findData( t.nType ) );
}

void PFillGradientToolBar::slotType( int n )
{
    PContextGradient t = g_Context->getGradient();
    t.nType = pType->itemData( n ).toInt();
    g_Context->setGradient( t );
}

void PFillGradientToolBar::slotSpread( int n )
{
    PContextGradient t = g_Context->getGradient();
    t.nSpread = QGradient::Spread(pSpread->itemData( n ).toInt());
    g_Context->setGradient( t );
}

QPixmap PFillGradientToolBar::getSwatch( const QSize size, int nType )
{
    QPixmap pixmap( size );
    QRect r( 0, 0, size.width(), size.height() );
    pixmap.fill( Qt::white );
    QPainter painter( &pixmap );

    if ( nType >= 0 )
    {
        QGradient gradient( (QGradient::Preset)nType );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientLinear )
    {
        QLinearGradient gradient( QPoint( r.left(), r.center().y() ), QPoint( r.right(),  r.center().y() )  );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        QRadialGradient gradient( r.center(), r.width() / 2 );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        QConicalGradient gradient( r.center(), 0.0 );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }

    return pixmap;
}

void PFillGradientToolBar::doAddType( const QString &stringText, int nType )
{
    pType->addItem( QIcon( getSwatch( QSize( 48, 48 ), nType ) ), stringText, nType );
}


