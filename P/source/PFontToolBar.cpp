#include "LibInfo.h"
#include "PFontToolBar.h"

#include "PContext.h"

PFontToolBar::PFontToolBar( QWidget *pParent )
    : QWidget( pParent )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pFont = new QFontComboBox( this );
    pFont->setCurrentFont( g_Context->getFont() );
    connect( pFont, SIGNAL(currentFontChanged(const QFont &)), SLOT(slotFont(const QFont &)) );
    pLayout->addWidget( pFont );

    pSize = new QSpinBox( this );
    pSize->setMinimum( 2 );
    pSize->setMaximum( 100 );
    pSize->setValue( g_Context->getFont().pointSize() );
    connect( pSize, SIGNAL(valueChanged(int)), SLOT(slotSize(int)) );
    pLayout->addWidget( pSize );

    pMore = new QToolButton( this );
    pMore->setText( ":" );
    pLayout->addWidget( pMore );
    connect( pMore, SIGNAL(clicked()), SLOT(slotMore()) );

    pLayout->addStretch( 11 );

    connect( g_Context, SIGNAL(signalModified(const QFont &)), SLOT(slotRefresh(const QFont &)) );
}

void PFontToolBar::slotRefresh( const QFont &t )
{
    pFont->setCurrentFont( t );
    pSize->setValue( t.pointSize() );
}

void PFontToolBar::slotFont( const QFont &t )
{
    g_Context->setFont( t );
}

void PFontToolBar::slotSize( int n )
{
    QFont font = g_Context->getFont();
    font.setPointSize( n );
    g_Context->setFont( font );
}

void PFontToolBar::slotMore()
{
    bool bOk = false;
    QFont font = QFontDialog::getFont( &bOk, g_Context->getFont(), this );
    if ( bOk ) g_Context->setFont( font ); 
}


