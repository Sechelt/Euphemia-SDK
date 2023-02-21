#include "LibInfo.h"
#include "PPasteRectangle.h"

//
// PPasteRectangle
//
PPasteRectangle::PPasteRectangle( PCanvas *pCanvas )
    : PDrawRectangle( pCanvas )
{
    image = QGuiApplication::clipboard()->image();
    doDrawState( QPoint( 10, 10 ) );
    doManipulateState();
}

PPasteRectangle::PPasteRectangle( PCanvas *pCanvas, const QImage &i )
    : PDrawRectangle( pCanvas )
{
    image = i;
    doDrawState( QPoint( 10, 10 ) );
    doManipulateState();
}

void PPasteRectangle::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

void PPasteRectangle::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdleState();
}

bool PPasteRectangle::canCommit()
{
    return !image.isNull();
}

void PPasteRectangle::doPaint( QPainter *pPainter, bool )
{
    if ( g_Context->getPaste().nCompositionMode != QPainter::CompositionMode_SourceOver )
    {
        // dest
        QImage imageDestination = g_Context->getImage()->copy( r );
        int nWidth = r.width();
        int nHeight = r.height();

        for ( int nX = 0; nX < nWidth; nX++ )                                                                                  
        {                                                                                                                           
            for ( int nY = 0; nY < nHeight; nY++ )                                                                              
            {                                                                                                                       
                if ( image.pixelColor( nX, nY ).alpha() == 0 ) imageDestination.setPixelColor( nX, nY, image.pixelColor( nX, nY ) );
            }                                                                                                                       
        }                                                                                                                           

        pPainter->drawImage( r, imageDestination );
        pPainter->setRenderHint(QPainter::Antialiasing, false);               // probably not relevant for drawImage but will not hurt
        pPainter->setCompositionMode( g_Context->getPaste().nCompositionMode );
    }
    // src
    pPainter->drawImage( r, image );
}

void PPasteRectangle::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );
    r.setTopLeft( point );
    r.setSize( image.size() );
    nState = StateDraw;
    update();
    emit signalChangedState();
}

//
// PPasteToolBar
//
PPasteToolBar::PPasteToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pComposition = new QComboBox( this );
    pComposition->addItem( "SourceOver", QPainter::CompositionMode_SourceOver );
    pComposition->addItem( "DestinationOver", QPainter::CompositionMode_DestinationOver );
    pComposition->addItem( "Clear", QPainter::CompositionMode_Clear );
    pComposition->addItem( "Source", QPainter::CompositionMode_Source );
    pComposition->addItem( "Destination", QPainter::CompositionMode_Destination );
    pComposition->addItem( "SourceIn", QPainter::CompositionMode_SourceIn );
    pComposition->addItem( "DestinationIn", QPainter::CompositionMode_DestinationIn );
    pComposition->addItem( "SourceOut", QPainter::CompositionMode_SourceOut );
    pComposition->addItem( "DestinationOut", QPainter::CompositionMode_DestinationOut );
    pComposition->addItem( "SourceAtop", QPainter::CompositionMode_SourceAtop );
    pComposition->addItem( "DestinationAtop", QPainter::CompositionMode_DestinationAtop );
    pComposition->addItem( "Xor", QPainter::CompositionMode_Xor );
    //svg 1.2 blend modes
    pComposition->addItem( "Plus", QPainter::CompositionMode_Plus );
    pComposition->addItem( "Multiply", QPainter::CompositionMode_Multiply );
    pComposition->addItem( "Screen", QPainter::CompositionMode_Screen );
    pComposition->addItem( "Overlay", QPainter::CompositionMode_Overlay );
    pComposition->addItem( "Darken", QPainter::CompositionMode_Darken );
    pComposition->addItem( "Lighten", QPainter::CompositionMode_Lighten );
    pComposition->addItem( "ColorDodge", QPainter::CompositionMode_ColorDodge );
    pComposition->addItem( "ColorBurn", QPainter::CompositionMode_ColorBurn );
    pComposition->addItem( "HardLight", QPainter::CompositionMode_HardLight );
    pComposition->addItem( "SoftLight", QPainter::CompositionMode_SoftLight );
    pComposition->addItem( "Difference", QPainter::CompositionMode_Difference );
    pComposition->addItem( "Exclusion", QPainter::CompositionMode_Exclusion );
    // ROPs (X11)
    pComposition->addItem( "SourceOrDestination", QPainter::RasterOp_SourceOrDestination );
    pComposition->addItem( "SourceAndDestination", QPainter::RasterOp_SourceAndDestination );
    pComposition->addItem( "SourceXorDestination", QPainter::RasterOp_SourceXorDestination );
    pComposition->addItem( "NotSourceAndNotDestination", QPainter::RasterOp_NotSourceAndNotDestination );
    pComposition->addItem( "NotSourceOrNotDestination", QPainter::RasterOp_NotSourceOrNotDestination );
    pComposition->addItem( "NotSourceXorDestination", QPainter::RasterOp_NotSourceXorDestination );
    pComposition->addItem( "NotSource", QPainter::RasterOp_NotSource );
    pComposition->addItem( "NotSourceAndDestination", QPainter::RasterOp_NotSourceAndDestination );
    pComposition->addItem( "SourceAndNotDestination", QPainter::RasterOp_SourceAndNotDestination );
    pComposition->addItem( "NotSourceOrDestination", QPainter::RasterOp_NotSourceOrDestination );
    pComposition->addItem( "SourceOrNotDestination", QPainter::RasterOp_SourceOrNotDestination );
    pComposition->addItem( "ClearDestination", QPainter::RasterOp_ClearDestination );
    pComposition->addItem( "SetDestination", QPainter::RasterOp_SetDestination );
    pComposition->addItem( "NotDestination", QPainter::RasterOp_NotDestination );
    pComposition->setCurrentIndex( pComposition->findData( (int)g_Context->getPaste().nCompositionMode ) );
    connect( pComposition, SIGNAL(currentIndexChanged(int)), SLOT(slotComposition(int)) );
    pLayout->addWidget( pComposition );

    pStamp = new QCheckBox( tr("Stamp"), this );
    pStamp->setChecked( g_Context->getPaste().bStamp );
    connect( pStamp, SIGNAL(stateChanged(int)), SLOT(slotStamp(int)) );
    pLayout->addWidget( pStamp );

    pLayout->addStretch( 10 );
}

void PPasteToolBar::slotRefresh( const PContextPaste &t )
{
    pComposition->setCurrentIndex( pComposition->findData( (int)t.nCompositionMode ) );
    pStamp->setChecked( t.bStamp );
}

void PPasteToolBar::slotComposition( int )
{
    PContextPaste paste = g_Context->getPaste();
    paste.nCompositionMode = QPainter::CompositionMode(pComposition->currentData().toInt());
    g_Context->setPaste( paste );
}

void PPasteToolBar::slotStamp( int n )
{
    PContextPaste paste = g_Context->getPaste();
    paste.bStamp = n;
    g_Context->setPaste( paste );
}

