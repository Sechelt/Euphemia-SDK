#include "LibInfo.h"
#include "PDrawText.h"

#include <WColorButton.h>
#include <WTextHAlignComboBox.h>
#include <WTextVAlignComboBox.h>

#include "PCanvas.h"
#include "PFontToolBar.h"

PDrawText::PDrawText( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
}

// paint - but without the rect
QRect PDrawText::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QRect rectUpdate = r;
    QPainter painter( g_Context->getImage());

    // apply context
    painter.setPen( g_Context->getPen() );
    painter.setFont( g_Context->getFont() );

    // paint
    painter.drawText( r, g_Context->getText().nHAlign | g_Context->getText().nVAlign, g_Context->getText().stringText );

    emit signalCommitted();

    //
    doIdle();

    return rectUpdate;
}

void PDrawText::doPaint( QPainter *pPainter )
{
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
PTextToolBar::PTextToolBar( QWidget *p, bool bLineEdit )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    // font - of course
    pLayout->addWidget( new PFontToolBar( this ) );
    // text
    if ( bLineEdit )
    {
        pLineEdit = new QLineEdit(g_Context->getText().stringText, this);
        pLayout->addWidget( pLineEdit );
        connect( pLineEdit, SIGNAL(textChanged(const QString &)), SLOT(slotText(const QString &)) );
    }
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

void PTextToolBar::slotRefresh( const PContextText &t )
{
    if ( pLineEdit ) pLineEdit->setText(t.stringText);
    pHAlign->setValue( t.nHAlign );
    pVAlign->setValue( t.nVAlign );
}

void PTextToolBar::slotText( const QString &t )
{
    if ( !pLineEdit ) return;
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



