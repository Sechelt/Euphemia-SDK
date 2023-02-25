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

#include "PColorControl.h"

#include <WColorPickerDialog.h>

#include "PContext.h"

PColorControl::PColorControl( QWidget *pParent )
    : QWidget( pParent )
{
    colorForeground = g_Context->getPen().color();
    colorBackground = g_Context->getBrush().color();

    QSize size( 24, 24 );
    rectForeground.setTopLeft( QPoint( 0, 0 ) );
    rectForeground.setSize( size );

    rectBackground.setTopLeft( QPoint( size.width() / 2, size.height() / 2 ) );
    rectBackground.setSize( size );

    connect( g_Context, SIGNAL(signalModified(const QPen &)), SLOT(slotRefresh(const QPen &)) );
    connect( g_Context, SIGNAL(signalModified(const QBrush &)), SLOT(slotRefresh(const QBrush &)) );
}

void PColorControl::slotChange( const QColor &color )
{
    // from widget not aware of g_Context
    // - set in self 
    // - signal to g_Context
    if ( nSwatch == SwatchForeground )
    {
        if ( colorForeground == color ) return;
        colorForeground = color;
        QPen pen = g_Context->getPen();
        pen.setColor( color );
        g_Context->setPen( pen );
        update();
        return;
    }

    if ( colorBackground == color ) return;
    colorBackground = color;
    QBrush brush = g_Context->getBrush();
    brush.setColor( color );
    g_Context->setBrush( brush );
    update();
}

void PColorControl::slotRefresh( const QPen &pen )
{
    // from g_Context
    // - refresh self 
    // - signal to widget not aware of g_Context
    if ( colorForeground == pen.color() ) return;
    colorForeground = pen.color();
    if ( nSwatch == SwatchForeground )
    {
        emit signalChanged( colorForeground );
    }
    update();
}

void PColorControl::slotRefresh( const QBrush &brush )
{
    // from g_Context
    // - refresh self 
    // - signal to widget not aware of g_Context
    if ( colorBackground == brush.color() ) return;
    colorBackground = brush.color();
    if ( nSwatch == SwatchBackground )
    {
        emit signalChanged( colorBackground );
    }
    update();
}

void PColorControl::paintEvent( QPaintEvent * )
{
    QPainter painter( this );

    QPen pen( Qt::darkGray );
    pen.setWidth( 2 );

    if ( nSwatch == SwatchBackground ) pen.setStyle( Qt::DotLine );
    else pen.setStyle( Qt::NoPen );
    painter.setPen( pen );
    painter.setBrush( QBrush( colorBackground ) );
    painter.drawRect( rectBackground );

    if ( nSwatch == SwatchForeground ) pen.setStyle( Qt::DotLine );
    else pen.setStyle( Qt::NoPen );
    painter.setPen( pen );
    painter.setBrush( QBrush( colorForeground ) );
    painter.drawRect( rectForeground );
}

void PColorControl::mouseDoubleClickEvent( QMouseEvent *pEvent )
{
    if ( rectForeground.contains( pEvent->pos() ) )
    {
        nSwatch = SwatchForeground;
        bool bOk = false;
        QColor color = WColorPickerDialog::getColor( &bOk, colorForeground, this, false );
        if ( !bOk ) return;
        if ( colorForeground == color ) return;
        // update context
        colorForeground = color;
        QPen pen = g_Context->getPen();
        pen.setColor( color );
        g_Context->setPen( pen );
        // update other
        emit signalChanged( colorForeground );

    }
    else if ( rectBackground.contains( pEvent->pos() ) )
    {
        nSwatch = SwatchBackground;
        bool bOk = false;
        QColor color = WColorPickerDialog::getColor( &bOk, colorBackground, this, false );
        if ( !bOk ) return;
        if ( colorBackground == color ) return;
        // update context
        colorBackground = color;
        QBrush brush = g_Context->getBrush();
        brush.setColor( color );
        g_Context->setBrush( brush );
        // update other
        emit signalChanged( colorBackground );
    }
}

void PColorControl::mousePressEvent( QMouseEvent *pEvent )
{
    if ( rectForeground.contains( pEvent->pos() ) )
    {
        if ( nSwatch == SwatchForeground ) return;
        nSwatch = SwatchForeground;
        emit signalChanged( nSwatch );
        update();
    }
    else if ( rectBackground.contains( pEvent->pos() ) )
    {
        if ( nSwatch == SwatchBackground ) return;
        nSwatch = SwatchBackground;
        emit signalChanged( nSwatch );
        update();
    }
}

QSize PColorControl::sizeHint() const
{
    return QSize( rectBackground.right() + 4, rectBackground.bottom() + 4 );
}

//
// PColorToolBar
//

PColorToolBar::PColorToolBar( QWidget *pParent )
    : QWidget( pParent )
{
    pLayout = new QHBoxLayout( this );

    pColorControl = new PColorControl( this );
    pLayout->addWidget( pColorControl );

    if ( pColorControl->getSwatch() == PColorControl::SwatchForeground )
    {
        pPenToolBar = new PPenToolBar( this, true );
        pLayout->addWidget( pPenToolBar );
    }
    else
    {
        pBrushToolBar = new PBrushToolBar( this, true );
        pLayout->addWidget( pBrushToolBar );
    }

    connect( pColorControl, SIGNAL(signalChanged(const QColor &)), SIGNAL(signalChanged(const QColor &)) );
    connect( pColorControl, SIGNAL(signalChanged(PColorControl::Swatchs)), SLOT(slotRefresh(PColorControl::Swatchs)) );
}

void PColorToolBar::slotChange( const QColor &color )
{
    pColorControl->slotChange( color );
}

void PColorToolBar::slotRefresh( PColorControl::Swatchs n )
{
    if ( n == PColorControl::SwatchForeground )
    {
        Q_ASSERT( pBrushToolBar );
        delete pBrushToolBar;
        pBrushToolBar = nullptr;
        pPenToolBar = new PPenToolBar( this, true );
        pLayout->addWidget( pPenToolBar );
    }
    else if ( n == PColorControl::SwatchBackground )
    {
        Q_ASSERT( pPenToolBar );
        delete pPenToolBar;
        pPenToolBar = nullptr;
        pBrushToolBar = new PBrushToolBar( this, true );
        pLayout->addWidget( pBrushToolBar );
    }
}


