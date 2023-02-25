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
#include "PDrawFreeHand.h"

#include "PCanvas.h"

PDrawFreeHand::PDrawFreeHand( PCanvas *pCanvas )
    : PFreeBase( pCanvas )
{
}

void PDrawFreeHand::doPress( PMouseEvent *pEvent )
{
    // init/reinit
    t = g_Context->getFreeHand();

    switch ( t.nShape )
    {
        case PContextFreeHand::ShapePen:
            pen = g_Context->getPen();
            brush.setStyle( Qt::NoBrush );
            break;
        case PContextFreeHand::ShapeEllipse:
        case PContextFreeHand::ShapeRectangle:
            pen.setStyle( Qt::NoPen );
            brush = g_Context->getBrush();
            break;
        case PContextFreeHand::ShapeCross:
            pen = g_Context->getPen();
            brush.setStyle( Qt::NoBrush );
            break;
        case PContextFreeHand::ShapeImageScaled:
            if ( t.image.isNull() )
            {
                t.nShape = PContextFreeHand::ShapePen;
                pen = g_Context->getPen();
                brush.setStyle( Qt::NoBrush );
            }
            else
                t.image = t.image.scaled( t.size );
            break;
        case PContextFreeHand::ShapeImage:
            if ( t.image.isNull() )
            {
                t.nShape = PContextFreeHand::ShapePen;
                pen = g_Context->getPen();
                brush.setStyle( Qt::NoBrush );
            }
            else
                t.size = t.image.size();
            break;
    }

    pointLast = pEvent->pos();
}

void PDrawFreeHand::doMove( PMouseEvent *pEvent )
{
    doDrawState( pEvent->pos() );
}

void PDrawFreeHand::doRelease( PMouseEvent *pEvent )
{
    doDrawState( pEvent->pos() );
}

void PDrawFreeHand::doDrawState( const QPoint &point )
{
    switch ( t.nShape )
    {
        case PContextFreeHand::ShapePen:
            return doDrawPen( point );
        case PContextFreeHand::ShapeEllipse:
            return doDrawEllipse( point );
        case PContextFreeHand::ShapeRectangle:
            return doDrawRectangle( point );
        case PContextFreeHand::ShapeCross:
            return doDrawCross( point );
        case PContextFreeHand::ShapeImageScaled:
        case PContextFreeHand::ShapeImage:
            return doDrawImage( point );
    }
    doDrawPen( point );
    pCanvas->update();
}

void PDrawFreeHand::doDrawPen( const QPoint &point )
{
    // draw line
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.drawLine( pointLast, point );
    // ready for next
    pointLast = point;
}

void PDrawFreeHand::doDrawEllipse( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw ellipse
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.setBrush( brush );
    painter.drawEllipse( r );
    // ready for next
    pointLast = point;
}

void PDrawFreeHand::doDrawRectangle( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw rectangle
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.setBrush( brush );
    painter.drawRect( r );
    // ready for next
    pointLast = point;
}

void PDrawFreeHand::doDrawCross( const QPoint &point )
{
    QRect r( point, t.size );
    r.moveCenter( point );

    // draw rectangle
    QPainter painter( g_Context->getImage() );
    painter.setPen( pen );
    painter.drawLine( r.left(), point.y(), r.right(), point.y() );
    painter.drawLine( point.x(), r.top(), point.x(), r.bottom() );
    // ready for next
    pointLast = point;
}

void PDrawFreeHand::doDrawImage( const QPoint &point )
{
    QPainter painter( g_Context->getImage() );
    QRect r( point, t.image.size() );
    r.moveCenter( point );

    painter.drawImage( r.topLeft(), t.image );
    // update last point even though we do not need it in this case
    pointLast = point;
}

//
// PFreeHandToolBar
//
PFreeHandToolBar::PFreeHandToolBar( QWidget *p )
    : QWidget( p )
{
    PContextFreeHand t = g_Context->getFreeHand();
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pShape = new QComboBox( this );
    pShape->addItem( "Pen", PContextFreeHand::ShapePen );
    pShape->addItem( "Ellipse", PContextFreeHand::ShapeEllipse );
    pShape->addItem( "Rectangle", PContextFreeHand::ShapeRectangle );
    pShape->addItem( "Cross", PContextFreeHand::ShapeCross );
    pShape->addItem( "ImageScaled", PContextFreeHand::ShapeImageScaled );
    pShape->addItem( "Image", PContextFreeHand::ShapeImage );
    pShape->setCurrentIndex( pShape->findData( (int)t.nShape ) );
    pShape->setToolTip( tr("shape") );
    pLayout->addWidget( pShape );
    connect( pShape, SIGNAL(currentIndexChanged(int)), SLOT(slotShape(int)) );
    //
    pWidth = new QSpinBox( this );
    pWidth->setMinimum( 1 );
    pWidth->setMaximum( 50 );
    pWidth->setValue( t.size.width() );
    pWidth->setToolTip( tr("width") );
    pLayout->addWidget( pWidth );
    connect( pWidth, SIGNAL(valueChanged(int)), SLOT(slotWidth(int)) );
    //
    pHeight = new QSpinBox( this );
    pHeight->setMinimum( 1 );
    pHeight->setMaximum( 50 );
    pHeight->setValue( t.size.height() );
    pHeight->setToolTip( tr("height") );
    pLayout->addWidget( pHeight );
    connect( pHeight, SIGNAL(valueChanged(int)), SLOT(slotHeight(int)) );

    pImage = new WImageButton( t.image, this );
    pImage->setToolTip( tr("image (double click to change)") );
    pLayout->addWidget( pImage, 10 );
    connect( pImage, SIGNAL(signalClick()), SLOT(slotImage()) );

    pLayout->addStretch( 16 );

    connect( g_Context, SIGNAL(signalModified(const PContextFreeHand &)), SLOT(slotRefresh(const PContextFreeHand &)) );
}

void PFreeHandToolBar::slotRefresh( const PContextFreeHand &t )
{
    pShape->setCurrentIndex( pShape->findData( (int)t.nShape ) );
    pWidth->setValue( t.size.width() );
    pHeight->setValue( t.size.height() );
    pImage->setImage( t.image );
}

void PFreeHandToolBar::slotShape( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.nShape = PContextFreeHand::Shapes(pShape->currentData().toInt());
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotWidth( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.size.setWidth( pWidth->value() );
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotHeight( int )
{
    PContextFreeHand t = g_Context->getFreeHand();
    t.size.setHeight( pHeight->value() );
    g_Context->setFreeHand( t );
}

void PFreeHandToolBar::slotImage()
{
    QImage image;

    QString stringFileName = QFileDialog::getOpenFileName( this, tr("Select image..."), QString(), "Image files (*.png *.xpm *.jpg)" );
    if ( !stringFileName.isEmpty() )
    {
        image.load( stringFileName );
    }

    //
    PContextFreeHand t = g_Context->getFreeHand();
    t.image = image;
    g_Context->setFreeHand( t );
}

