#include "LibInfo.h"
#include "PPenToolBar.h"

#include "PContext.h"

//
// PPenToolBar
//
PPenToolBar::PPenToolBar( QWidget *pParent )
    : QWidget( pParent )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pColor = new WColorButton( g_Context->getPen().color(), this, WColorButton::Pen );
    pLayout->addWidget( pColor );
    connect( pColor, SIGNAL(signalChanged(const QColor &)), SLOT(slotColor(const QColor &)) );

    pStyle = new WLineStyleComboBox( g_Context->getPen().style(), this );
    pStyle->setToolTip( tr("pen style") );
    pLayout->addWidget( pStyle );
    connect( pStyle, SIGNAL(signalChanged(Qt::PenStyle)), SLOT(slotStyle(Qt::PenStyle)) );

    pWidth = new QSpinBox( this );
    pWidth->setToolTip( tr("pen width") );
    pWidth->setMinimum( 0 );
    pWidth->setMaximum( 100 );
    pWidth->setValue( g_Context->getPen().width() );
    pLayout->addWidget( pWidth );
    connect( pWidth, SIGNAL(valueChanged(int)), SLOT(slotWidth(int)) );

    pMore = new QToolButton( this );
    pMore->setText( ":" );
    pLayout->addWidget( pMore );
    connect( pMore, SIGNAL(clicked()), SLOT(slotMore()) );

    pLayout->addStretch( 11 );

    connect( g_Context, SIGNAL(signalModified(const QPen &)), SLOT(slotRefresh(const QPen &)) );
}

void PPenToolBar::slotRefresh( const QPen &pen )
{
    pColor->setValue( pen.color() );                
    pStyle->setValue( pen.style() );                
    pWidth->setValue( pen.width() );
}

void PPenToolBar::slotColor( const QColor &color )
{
    QPen pen = g_Context->getPen();
    pen.setColor( color );
    g_Context->setPen( pen );
}

void PPenToolBar::slotStyle( Qt::PenStyle n )
{
    QPen pen = g_Context->getPen();
    pen.setStyle( n );
    g_Context->setPen( pen );
}

void PPenToolBar::slotWidth( int n )
{
    QPen pen = g_Context->getPen();
    pen.setWidth( n );
    g_Context->setPen( pen );
}

void PPenToolBar::slotMore()
{
    bool bOk = false;
    QPen pen = PPenDialog::getPen( &bOk, g_Context->getPen(), this );
    if ( bOk ) g_Context->setPen( pen );
}

//
// PPenSwatch
//
PPenSwatch::PPenSwatch( const QPen &pen, QWidget *pParent )
    : QWidget( pParent )
{
    this->pen = pen;
}

void PPenSwatch::setPen( const QPen &pen )
{
    this->pen = pen;
    update();
}

void PPenSwatch::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);

    QPainter painter( this );
    painter.setPen( pen );
    painter.fillRect( 0, 0, width(), height(), Qt::white );
    painter.drawLine( 0, height()/2, width(), height()/2 );
}

QSize PPenSwatch::sizeHint() const
{
    return QSize( 16, 16 );
}

//
// PPenDialog
//
PPenDialog::PPenDialog( const QPen &pen, QWidget *pParent )
    : QDialog( pParent )
{
    setWindowTitle( tr("Pen") );

    this->pen = pen;

    QVBoxLayout *pLayout = new QVBoxLayout( this );
    QWidget *p = new QWidget( this );

    {
        QVBoxLayout *pLayoutTop = new QVBoxLayout( p );

        QGridLayout *pLayout = new QGridLayout();

        pLayout->addWidget( new QLabel( tr("Cap Style:"), p ), 0, 0 );
        pLayout->addWidget( new QLabel( tr("Color:"), p ), 1, 0 );
        pLayout->addWidget( new QLabel( tr("Join Style:"), p ), 2, 0 );
        pLayout->addWidget( new QLabel( tr("Miter Limit:"), p ), 3, 0 );
        pLayout->addWidget( new QLabel( tr("Style:"), p ), 4, 0 );
        pLayout->addWidget( new QLabel( tr("Width:"), p ), 5, 0 );
        pLayout->addWidget( new QLabel( tr("Stroke Fill:"), p ), 6, 0 );

        pCapStyle = new QComboBox( p );
        pCapStyle->addItem( "SquareCap", (int)Qt::SquareCap );
        pCapStyle->addItem( "FlatCap", (int)Qt::FlatCap );
        pCapStyle->addItem( "RoundCap", (int)Qt::RoundCap );
        WSetComboBox( pCapStyle, pen.capStyle() );
        pLayout->addWidget( pCapStyle, 0, 1 );
        connect( pCapStyle, SIGNAL(activated(int)), SLOT(slotCapStyle(int)) );

        pColor = new WColorButton( pen.color(), p, WColorButton::Pen );
        pLayout->addWidget( pColor, 1, 1 );
        connect( pColor, SIGNAL(signalChanged(const QColor &)), SLOT(slotColor(const QColor &)) );

        pJoinStyle = new QComboBox( p );
        pJoinStyle->addItem( "BevelJoin", (int)Qt::BevelJoin );
        pJoinStyle->addItem( "MiterJoin", (int)Qt::MiterJoin );
        pJoinStyle->addItem( "RoundJoin", (int)Qt::RoundJoin );
        WSetComboBox( pJoinStyle, pen.joinStyle() );
        pLayout->addWidget( pJoinStyle, 2, 1 );
        connect( pJoinStyle, SIGNAL(activated(int)), SLOT(slotJoinStyle(int)) );

        pMiterLimit = new QSpinBox( p );
        pMiterLimit->setMinimum( 0 );
        pMiterLimit->setMaximum( 10 );
        pMiterLimit->setValue( pen.miterLimit() );
        pLayout->addWidget( pMiterLimit, 3, 1 );
        connect( pMiterLimit, SIGNAL(valueChanged(int)), SLOT(slotMiterLimit(int)) );

        pStyle = new WLineStyleComboBox( pen.style(), p );
        pLayout->addWidget( pStyle, 4, 1 );
        connect( pStyle, SIGNAL(signalChanged(Qt::PenStyle)), SLOT(slotStyle(Qt::PenStyle)) );

        pWidth = new QSpinBox( p );
        pWidth->setMinimum( 0 );
        pWidth->setMaximum( 100 );
        pWidth->setValue( pen.width() );
        pLayout->addWidget( pWidth, 5, 1 );
        connect( pWidth, SIGNAL(valueChanged(int)), SLOT(slotWidth(int)) );

        // brush
        {
            QHBoxLayout *pLayoutBrush = new QHBoxLayout();

            pColorBrush = new WColorButton( pen.brush().color(), p, WColorButton::Brush );
            pColorBrush->setToolTip( tr("stroke fill color") );
            pLayoutBrush->addWidget( pColorBrush );
            connect( pColorBrush, SIGNAL(signalChanged(const QColor &)), SLOT(slotColorBrush(const QColor &)) );

            pStyleBrush = new WBrushStyleComboBox( this, pen.brush().style() );
            pStyleBrush->setToolTip( tr("stroke fill style (NoBrush for no stroke fill)") );
            pLayoutBrush->addWidget( pStyleBrush );
            connect( pStyleBrush, SIGNAL(signalChanged(Qt::BrushStyle)), SLOT(slotStyleBrush(Qt::BrushStyle)) );

            pMoreBrush = new QToolButton( this );
            pMoreBrush->setText( ":" );
            pLayoutBrush->addWidget( pMoreBrush );
            connect( pMoreBrush, SIGNAL(clicked()), SLOT(slotMoreBrush()) );

            pLayout->addLayout( pLayoutBrush, 6, 1 );
        }

        pLayoutTop->addLayout( pLayout );
        pLayoutTop->addStretch( 10 );
        pSwatch = new PPenSwatch( pen, p );
        pLayoutTop->addWidget( pSwatch );
    }

    pLayout->addWidget( p ); 

    QDialogButtonBox *pButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );
    pLayout->addWidget( pButtonBox ); 

    connect( pButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( pButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject );
}

QPen PPenDialog::getPen( bool *pOk, const QPen &pen, QWidget *pParent )
{
    QPen penOriginal = pen;
    *pOk = true;
    PPenDialog dialog( pen, pParent );
    if ( dialog.exec() == QDialog::Accepted ) return dialog.getPen();
    *pOk = false;
    return penOriginal;
}

void PPenDialog::slotCapStyle( int nIndex )
{
    if ( pCapStyle->itemData( nIndex ).isNull() ) return;
    pen.setCapStyle( (Qt::PenCapStyle)(pCapStyle->itemData( nIndex ).toInt()) );
    pSwatch->setPen( pen );
}

void PPenDialog::slotColor( const QColor &color )
{
    pen.setColor( color );
    pSwatch->setPen( pen );
}

void PPenDialog::slotJoinStyle( int nIndex )
{
    if ( pJoinStyle->itemData( nIndex ).isNull() ) return;
    pen.setJoinStyle( (Qt::PenJoinStyle)(pJoinStyle->itemData( nIndex ).toInt()) );
    pSwatch->setPen( pen );
}

void PPenDialog::slotMiterLimit( int n )
{
    pen.setMiterLimit( n );
    pSwatch->setPen( pen );
}

void PPenDialog::slotStyle( Qt::PenStyle n )
{
    pen.setStyle( n );
    pSwatch->setPen( pen );
}

void PPenDialog::slotWidth( int n )
{
    pen.setWidth( n );
    pSwatch->setPen( pen );
}

void PPenDialog::slotColorBrush( const QColor &color )
{
    QBrush brush = pen.brush();
    brush.setColor( color );
    pen.setBrush( brush );
    pSwatch->setPen( pen );
}

void PPenDialog::slotStyleBrush( Qt::BrushStyle n )
{
    QBrush brush = pen.brush();
    brush.setStyle( n );
    pen.setBrush( brush );
    pSwatch->setPen( pen );
}

void PPenDialog::slotMoreBrush()
{
    bool bOk = false;
    QBrush brush = PBrushDialog::getBrush( &bOk, pen.brush(), this );
    if ( bOk )
    {
        pen.setBrush( brush );
        pColorBrush->setValue( pen.brush().color() );
        pStyleBrush->setValue( pen.brush().style() );
        pSwatch->setPen( pen );
    }
}

