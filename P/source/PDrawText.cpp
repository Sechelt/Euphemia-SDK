#include "LibInfo.h"
#include "PDrawText.h"

#include <WColorButton.h>
#include <WTextHAlignComboBox.h>
#include <WTextVAlignComboBox.h>

#include "PCanvas.h"
#include "PFontToolBar.h"

PDrawText::PDrawText( PCanvas *pCanvas, const QPoint &pointBegin )
    : PDrawRectangle( pCanvas, pointBegin )
{
}

void PDrawText::doCommit()
{
    QPainter painter( g_Context->getImage() );

    QRect r( pointBegin, pointEnd );
    r = r.normalized();

    // apply context
    painter.setPen( g_Context->getPen() );
    painter.setFont( g_Context->getFont() );

    // paint
    painter.drawText( r, g_Context->getText().nHAlign | g_Context->getText().nVAlign, g_Context->getText().stringText );

    doDeleteHandles();
}

void PDrawText::doPaint( QPainter *pPainter, const QPoint &pointBegin, const QPoint &pointEnd )
{
    QRect r( pointBegin, pointEnd );
    r = r.normalized();

    // temp outline of rect area
    pPainter->setPen( QPen( Qt::DotLine ) );
    pPainter->drawRect( r );

    // apply context
    pPainter->setPen( g_Context->getPen() );
    pPainter->setFont( g_Context->getFont() );

    // paint
    pPainter->drawText( r, g_Context->getText().nHAlign | g_Context->getText().nVAlign, g_Context->getText().stringText );
}

//
// PTextToolBar
//
PTextToolBar::PTextToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    // we only want the pen (foreground) color - not the entire pen
    pColor = new WColorButton( g_Context->getPen().color(), this, WColorButton::Pen );
    pLayout->addWidget( pColor );
    connect( pColor, SIGNAL(signalChanged(const QColor &)), SLOT(slotColor(const QColor &)) );
    connect( g_Context, SIGNAL(signalModified(const QPen &)), SLOT(slotRefresh(const QPen &)) );
    // font - of course
    pLayout->addWidget( new PFontToolBar( this ) );
    // text
    pLineEdit = new QLineEdit( g_Context->getText().stringText, this );
    pLayout->addWidget( pLineEdit );
    connect( pLineEdit, SIGNAL(textChanged(const QString &)), SLOT(slotText(const QString &)) );
    //
    pHAlign = new WTextHAlignComboBox( g_Context->getText().nHAlign, this );
    pLayout->addWidget( pHAlign );
    connect( pHAlign, SIGNAL(signalChanged(Qt::AlignmentFlag)), SLOT(slotHAlign(Qt::AlignmentFlag)) );
    //
    pVAlign = new WTextVAlignComboBox( g_Context->getText().nVAlign, this );
    pLayout->addWidget( pVAlign );
    connect( pVAlign, SIGNAL(signalChanged(Qt::AlignmentFlag)), SLOT(slotVAlign(Qt::AlignmentFlag)) );
    //
    pMore = new QToolButton( this );
    pMore->setText( ":" );
    pLayout->addWidget( pMore );
    connect( pMore, SIGNAL(clicked()), SLOT(slotMore()) );
    //
    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextText &)), SLOT(slotRefresh(const PContextText &)) );
}

void PTextToolBar::slotRefresh( const QPen &t )
{
    pColor->setValue( t.color() );
}

void PTextToolBar::slotRefresh( const PContextText &t )
{
    pLineEdit->setText( t.stringText );
    pHAlign->setValue( t.nHAlign );
    pVAlign->setValue( t.nVAlign );
}

void PTextToolBar::slotColor( const QColor &t )
{
    QPen pen = g_Context->getPen();
    pen.setColor( t );
    g_Context->setPen( pen );
}

void PTextToolBar::slotText( const QString &t )
{
    PContextText text = g_Context->getText();
    text.stringText = t;
    g_Context->setText( text );
}

void PTextToolBar::slotHAlign( Qt::AlignmentFlag n )
{
    PContextText text = g_Context->getText();
    text.nHAlign = n;
    g_Context->setText( text );
}

void PTextToolBar::slotVAlign( Qt::AlignmentFlag n )
{
    PContextText text = g_Context->getText();
    text.nVAlign = n;
    g_Context->setText( text );
}

void PTextToolBar::slotMore()
{
    // shadow effects etc
}



