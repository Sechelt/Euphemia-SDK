#include "LibInfo.h"
#include "PBrushToolBar.h"

#include "PContext.h"

PBrushToolBar::PBrushToolBar( QWidget *pParent )
    : QWidget( pParent )
{
    setObjectName( "PBrushToolBar" );

    QHBoxLayout *pLayout = new QHBoxLayout( this );

    pColor = new WColorButton( g_Context->getBrush().color(), this, WColorButton::Fill );
    pLayout->addWidget( pColor );
    connect( pColor, SIGNAL(signalChanged(const QColor &)), SLOT(slotColor(const QColor &)) );

    pStyle = new WBrushStyleComboBox( this, g_Context->getBrush().style() );
    pStyle->setToolTip( tr("brush style") );
    // pStyle->setValue( g_Context->getBrush().style() );
    pLayout->addWidget( pStyle );
    connect( pStyle, SIGNAL(signalChanged(Qt::BrushStyle)), SLOT(slotStyle(Qt::BrushStyle)) );

    pImage = new WImageButton( g_Context->getBrush().textureImage(), this );
    pLayout->addWidget( pImage, 10 );
    connect( pImage, SIGNAL(signalClick()), SLOT(slotImage()) );

    pMore = new QToolButton( this );
    pMore->setText( ":" );
    pLayout->addWidget( pMore );
    connect( pMore, SIGNAL(clicked()), SLOT(slotMore()) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const QBrush &)), SLOT(slotRefresh(const QBrush &)) );
}

void PBrushToolBar::slotRefresh( const QBrush &t )
{
    pColor->setValue( t.color() );
    pStyle->setValue( t.style() );
    pImage->setImage( t.textureImage() );
}

void PBrushToolBar::slotColor( const QColor &color )
{
    QBrush brush = g_Context->getBrush();
    brush.setColor( color );
    g_Context->setBrush( brush );
}

void PBrushToolBar::slotStyle( Qt::BrushStyle n )
{
    QBrush brush = g_Context->getBrush();
    brush.setStyle( n );
    g_Context->setBrush( brush );
}

void PBrushToolBar::slotImage()
{
    QImage image;

    QString stringFileName = QFileDialog::getOpenFileName( this, tr("Select image..."), QString(), "Image files (*.png *.xpm *.jpg)" );
    if ( !stringFileName.isEmpty() )
    {
        image.load( stringFileName );
    }

    // accept null or loaded image
    QBrush brush = g_Context->getBrush();
    brush.setTextureImage( image );
    g_Context->setBrush( brush );
}

void PBrushToolBar::slotMore()
{
    bool bOk = false;
    QBrush brush = PBrushDialog::getBrush( &bOk, g_Context->getBrush(), this );
    if ( bOk ) g_Context->setBrush( brush );
}

//
// PBrushSwatch
//
PBrushSwatch::PBrushSwatch( const QBrush &brush, QWidget *pParent )
    : QWidget( pParent )
{
    this->brush = brush;
}

void PBrushSwatch::setBrush( const QBrush &brush )
{
    this->brush = brush;
    update();
}

void PBrushSwatch::paintEvent( QPaintEvent *pEvent )
{
    Q_UNUSED(pEvent);

    QPainter painter(this);
    painter.fillRect( 0, 0, width(), height(), brush );
}

QSize PBrushSwatch::sizeHint() const
{
    return QSize( 16, 16 );
}


//
// PBrushDialog
//
PBrushDialog::PBrushDialog( const QBrush &brush, QWidget *pParent )
    : QDialog( pParent )
{
    setWindowTitle( tr("Brush") );
    this->brush = brush;

    QVBoxLayout *pLayout = new QVBoxLayout( this );
    QWidget *p = new QWidget( this );

    {
        QVBoxLayout *pLayoutTop = new QVBoxLayout( p );

        QFormLayout *pLayout = new QFormLayout();

        pColor = new WColorButton( brush.color(), p, WColorButton::Brush );
        pLayout->addRow( tr("Color"), pColor );
        connect( pColor, SIGNAL(signalChanged(QColor)), SLOT(slotColor(QColor)) );

        pStyleComboBox = new WBrushStyleComboBox( p, brush.style() );
        pStyleComboBox->setValue( brush.style() );
        pLayout->addRow( tr("Style"), pStyleComboBox );
        connect( pStyleComboBox, SIGNAL(signalChanged(Qt::BrushStyle)), SLOT(slotStyle(Qt::BrushStyle)) );

        pImage = new WImageButton( brush.textureImage(), p );
        pLayout->addRow( tr("Image"), pImage );
        connect( pImage, SIGNAL(signalClick()), SLOT(slotImage()) );

        pLayoutTop->addLayout( pLayout );
        pLayoutTop->addStretch( 10 );
        pSwatch = new PBrushSwatch( brush, p );
        pLayoutTop->addWidget( pSwatch );
    }

    pLayout->addWidget( p ); 

    QDialogButtonBox *pButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );
    pLayout->addWidget( pButtonBox ); 

    connect( pButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( pButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject );
}

QBrush PBrushDialog::getBrush( bool *pOk, const QBrush &brush, QWidget *pParent )
{
    QBrush brushOriginal = brush;
    *pOk = true;
    PBrushDialog dialog( brush, pParent );
    if ( dialog.exec() == QDialog::Accepted ) return dialog.getBrush();
    *pOk = false;
    return brushOriginal;
}

void PBrushDialog::slotColor( QColor color )
{
    brush.setColor( color );
    pSwatch->setBrush( brush );
}

void PBrushDialog::slotStyle( Qt::BrushStyle n )
{
    brush.setStyle( n );
    pSwatch->setBrush( brush );
}

void PBrushDialog::slotImage()
{
    QImage image;
    QString stringFileName = QFileDialog::getOpenFileName( this, tr("Select image..."), QString(), "*.png" );
    if ( !stringFileName.isEmpty() ) image.load( stringFileName );

    pImage->setImage( image );
    brush.setTextureImage( image );
    pSwatch->setBrush( brush );
}

