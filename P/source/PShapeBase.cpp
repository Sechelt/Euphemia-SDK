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

#include "PShapeBase.h"
#include "PCanvas.h"

PShapeBase::PShapeBase( PCanvas *pCanvas )
    : QGraphicsObject()
{
    nState = StateIdle;
    this->pCanvas = pCanvas;
    pView = pCanvas->getView();
}

PShapeBase::~PShapeBase()
{
}

/*!
 * \brief Scene wants this to paint itself. 
 *  
 * Ignore if this is StateIdle otherwise strip down the call and pass to doPaint. 
 * 
 * \author pharvey (2/10/23)
 * 
 * \param pPainter 
 * \param nOption  
 * \param pWidget  
 */
void PShapeBase::paint( QPainter *pPainter, const QStyleOptionGraphicsItem *nOption, QWidget *pWidget )
{
    Q_UNUSED( nOption );
    Q_UNUSED( pWidget );

    if ( nState == StateIdle ) return;
    doPaint( pPainter, false );
}

/*!
 * \brief Scene wants to know the geometry. 
 *  
 * We are a temp object which always covers the entire; scene/canvas/image. 
 *  
 * Derived classes should not need to change this. 
 * 
 * \author pharvey (2/10/23)
 * 
 * \return QRectF 
 */
QRectF PShapeBase::boundingRect() const
{
    if ( scene() ) return scene()->sceneRect();
    return QRectF();
}

void PShapeBase::doCancel()
{
    if ( nState != StateIdle ) doIdleState();
}

void PShapeBase::doCopy()
{
     QClipboard *clipboard = QGuiApplication::clipboard();
     clipboard->setImage( getCopy() );
}

void PShapeBase::doZoom()
{
    if ( nState != StateManipulate ) return;
    doSyncHandles();
}

bool PShapeBase::canCancel() 
{
    return nState == StateManipulate;
}

PHandle *PShapeBase::getHandle( const QPoint &pointPos )
{
    // search in REVERSE order to reflect z-order
    for ( int n = vectorHandles.count() - 1; n >= 0; n-- )
    {
        PHandle *p = vectorHandles.at( n );
        QRect r = pView->mapToScene( p->geometry() ).boundingRect().toRect();
        if ( r.contains( pointPos ) ) return p;
    }
    return nullptr;
}

/*!
 * \brief Return a handle of given type that is below the given handle.
 * 
 * \author pharvey (2/20/23)
 * 
 * \param pHandle 
 * \param nType   
 * 
 * \return PHandle* 
 */
PHandle *PShapeBase::getHandleUnder( PHandle *pHandle, PHandle::Type nType )
{
    QPoint point = pHandle->geometry().center();

    // search in REVERSE order to reflect z-order
    for ( int n = vectorHandles.count() - 1; n >= 0; n-- )
    {
        PHandle *p = vectorHandles.at( n );
        if ( p->getType() != nType ) continue;          // looking for a handle this is the same type
        if ( p == pHandle ) continue;                   // and not given handle
        if ( p->geometry().contains( point ) ) return p;    // and at same pos (roughly)
    }
    return nullptr;
}

PHandle *PShapeBase::getHandleNext( PHandle *pHandle, PHandle::Type nType )
{
    for ( int n = vectorHandles.indexOf( pHandle ) + 1; n < vectorHandles.count(); n++ )
    {
        PHandle *p = vectorHandles.at( n );
        if ( p->getType() == nType ) return p;
    }
    return nullptr;
}

PHandle *PShapeBase::getHandlePrev( PHandle *pHandle, PHandle::Type nType )
{
    for ( int n = vectorHandles.indexOf( pHandle ) - 1; n >= 0; n-- )
    {
        PHandle *p = vectorHandles.at( n );
        if ( p->getType() == nType ) return p;
    }
    return nullptr;
}

/*!
 * \brief Returns a trimmed version of a copied image. The trimmed area becomes transparent.
 *  
 * This can trim around an ellipse and around a complex polygon with an appropriate mask. 
 * Scans every pixel in imageCopied. 
 *  
 * r 
 *  
 * This is a rectangle, in canvas coordinates, which corresponds to the area copied. It is used 
 * when bErase is true. 
 *  
 * imageCopied 
 *  
 * This is the image copied from the canvas. A trimmed version of this is returned. 
 * Is the same size as r. 
 *  
 * imageMask 
 *  
 * This is a mask used to determine which pixels to trim. It only has two colors for the pixels - black and white. 
 * The black pixels indicate that the corresponding pixel in imageCopied should be left. 
 * The white pixels indicate that the corresponding pixel in imageCopied should be trimmed (set to transparent). 
 * Is the same size as r. 
 *  
 * bErase 
 *  
 * Indicates whether or not the copied image should be erased from the canvas. 
 * true = erase (set transparent) in canvas where mask has black pixels 
 * false = NoOp 
 *  
 * \author pharvey (2/5/23)
 * 
 * \param r bounding rect of copied image (in canvas coordinates)
 * \param imageCopied image copied from canvas
 * \param imageMask mask used to trim (same size as imageCopied with black being pixels to keep and white the pixels to trim)
 * \param bErase true to erase (where mask pixels are black) from canvas 
 * 
 * \return QImage 
 */
QImage PShapeBase::getTrimmed( const QRect &r, QImage &imageCopied, const QImage &imageMask, bool bErase )
{
    Q_ASSERT( imageCopied.size() == imageMask.size() );

    QColor      colorMask( Qt::black );                         // assume black is for the area we want to keep
    QColor      colorBackground( Qt::transparent );             // assume canvas background is transparent
    QImage *    pImageCanvas    = g_Context->getImage();
    int         nWidth          = imageCopied.size().width();
    int         nHeight         = imageCopied.size().height();

    // Seems that creating QColor with Qt::transparent does not properly set alpha channel so...
    colorBackground.setAlpha( 0 );

    for ( int nX = 0; nX < nWidth; nX++ )
    {
        for ( int nY = 0; nY < nHeight; nY++ )
        {
            // trim by setting pixel to mask trim color - probably transparent but definately not colorMask
            if ( imageMask.pixelColor( nX, nY ) != colorMask ) 
                imageCopied.setPixelColor( nX, nY, imageMask.pixelColor( nX, nY ) );
            // not in trim area so consider erasing (ie for a cut operation)
            else if ( bErase ) 
                pImageCanvas->setPixelColor( nX + r.x(), nY + r.y(), colorBackground );
        }
    }

    return imageCopied;
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


