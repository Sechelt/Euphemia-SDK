#include "LibInfo.h"
#include "PCanvas.h"

#include "PContext.h"

PCanvas::PCanvas( QWidget *parent, const QSize &size )
    : QWidget( parent )
{
    setAttribute( Qt::WA_StaticContents );
    resize( size );
}

PCanvas::PCanvas( QWidget *parent, const QImage &image )
    : QWidget( parent )
{
    setAttribute( Qt::WA_StaticContents );
    resize( image.size() );
    this->image = image;
}

void PCanvas::setZoom( WZoomWidget::FitTypes nFit, int nZoom )
{
    this->nFit = nFit;
    this->nZoom = nZoom;
}

void PCanvas::setTool( Tools n )
{
    nTool = n;
}

void PCanvas::setAutoCommit( bool b )
{
    if ( isDrawing() ) doDrawCancel();
    bAutoCommit = b;
}

/*!
 * \brief Open/load an image from a file. 
 *  
 * This will prompt for a file name and load it. 
 *  
 * This will return false if; 
 *  
 * - any existing modifications could neither be saved nor discarded 
 * - no file selected 
 * - file is not a viable image 
 *  
 * \author pharvey (11/22/22)
 * 
 * \return bool 
 */
bool PCanvas::doOpen()
{
    // save any changes
    // - not likley to ever be modified here as open will be called from a new state - but just in case
    if ( isModified() )
    {
        // prompt to save, discard or cancel
        QMessageBox::StandardButton nButton = QMessageBox::question( this, tr("Open..."), tr("Discard changes?") );
        if ( nButton == QMessageBox::No ) return false;
        if ( !doSave() ) return false;
        doClear();
    }

    // get a file name
    QString stringFileName = QFileDialog::getOpenFileName( this, tr( "Select Image..." ), QString(), tr("Image (*.png)") );
    if ( stringFileName.isEmpty() ) return false;

    // load the file
    QImageReader reader( stringFileName );
    if ( reader.read( &image ) ) 
    {
        resize( image.size() );
        this->stringFileName = stringFileName;
        setModified( false );
        update();
        return true;
    }

    // error
    QMessageBox::warning( this, tr("Open"), reader.errorString() );

    return false;
}

bool PCanvas::doSave()
{
    // any changes to save?
    if ( !isModified() ) return true;

    // do we need to prompt for a file name?
    if ( stringFileName.isNull() ) return doSaveAs();

    // do it
    QImageWriter writer( stringFileName );
    if ( writer.write( image ) ) 
    {
        setModified( false );
        return true;
    }

    // error
    QMessageBox::warning( this, tr("Save"), writer.errorString() );

    return false;
}

bool PCanvas::doSaveAs()
{
    // get a file name
    QString stringFileName = QFileDialog::getSaveFileName( this, tr( "Save As..." ) );
    if ( stringFileName.isEmpty() ) return false;

    // do it
    QImageWriter writer( stringFileName );
    if ( writer.write( image ) ) 
    {
        this->stringFileName = stringFileName;
        setModified( false );
        return true;
    }

    // error
    QMessageBox::warning( this, tr("Save"), writer.errorString() );

    return false;
}

bool PCanvas::doClose()
{
    // save any changes
    if ( isModified() )
    {
        // prompt to save, discard or cancel
        QMessageBox::StandardButton nButton = QMessageBox::question( this, tr("Close..."), tr("Save changes?") );
        if ( nButton == QMessageBox::Yes ) 
        {
            if ( !doSave() ) return false;
        }
    }

    // do it
    doClear();
    return true;
}

void PCanvas::doCut()
{
}

void PCanvas::doCopy()
{
}

void PCanvas::doPaste()
{
}

void PCanvas::doUndo()
{
    Q_ASSERT( !stackUndo.isEmpty() );
    stackRedo.push( image );
    image = stackUndo.pop();
    update();
    emit signalStateChanged();
}

void PCanvas::doRedo()
{
    Q_ASSERT( !stackRedo.isEmpty() );
    stackUndo.push( image );
    image = stackRedo.pop();
    update();
    emit signalStateChanged();
}

void PCanvas::doDrawCommit()
{
    Q_ASSERT( isDrawing() );

    if ( pShapeBase )
    {
        // update undo/redo
        stackRedo.clear();
        stackUndo.push( image );
        if ( stackUndo.count() >= nMaxUndo ) stackUndo.removeFirst();
        // draw to image
        pShapeBase->doCommit();
        // get rid temp shape widget
        delete pShapeBase;
        pShapeBase = nullptr;
        // we have been modified so...
        setModified( true );
    }
}

void PCanvas::doDrawCancel()
{
    Q_ASSERT( isDrawing() );

/*
    if ( nTool == ToolSelection )                                   
    {                                                               
        PiSelection *pSelection = g_Palette->selection.getCurrent();
        pSelection->doFini();                                       
        emit signalStateChanged();                                  
    }                                                               
*/
    if ( pShapeBase )
    {
        pShapeBase->doCancel();
        delete pShapeBase;
        pShapeBase = nullptr;
    }
}

bool PCanvas::isDrawing()
{
    if ( pShapeBase ) return true;
    return false;
}

bool PCanvas::canCut()
{
//    PiSelection *pSelection = g_Palette->selection.getCurrent();
//    return ( pSelection->getState() == PiSelection::StateManipulating );
    return false;
}

bool PCanvas::canCopy() 
{
//    PiSelection *pSelection = g_Palette->selection.getCurrent();
//    return ( pSelection->getState() == PiSelection::StateManipulating );
    return false;
}

bool PCanvas::canPaste() 
{
    return false;
}

bool PCanvas::canUndo() 
{
    return !stackUndo.isEmpty();
}

bool PCanvas::canRedo() 
{
    return !stackRedo.isEmpty();
}

void PCanvas::mousePressEvent( QMouseEvent *pEvent )
{
    emit signalPos( pEvent->pos() );
    if ( pEvent->button() != Qt::LeftButton ) return;

    // init drawing
    if ( !isDrawing() ) 
    {
        switch ( nTool )
        {
            case ToolSelectRectangle:    
                break;
            case ToolSelectEllipse:      
                break;
            case ToolSelectPolygon:      
                break;
            case ToolDrawFreeHand:               
                break;
            case ToolDrawSpray:               
                break;
            case ToolDrawLine:
                g_Context->setImage( &image );
                pShapeBase = new PDrawLine( this, pEvent->pos() );           
                break;
            case ToolDrawRectangle:      
                g_Context->setImage( &image );
                pShapeBase = new PDrawRectangle( this, pEvent->pos() );           
                break;
            case ToolDrawEllipse:        
                g_Context->setImage( &image );
                pShapeBase = new PDrawEllipse( this, pEvent->pos() );           
                break;
            case ToolDrawPolygon:        
                break;
            case ToolDrawRectangleFilled:
                break;
            case ToolDrawEllipseFilled:  
                break;
            case ToolDrawPolygonFilled:  
                break;
            case ToolDrawText:  
                break;
            case ToolFillFlood:          
                break;
            case ToolFillGradient:       
                break;
        }
    }

    // press
    if ( pShapeBase ) 
    {
        // let shape decide what to do - returns false when its done
        if ( !pShapeBase->doPress( pEvent ) )
            doDrawCommit();
    }
}

void PCanvas::mouseMoveEvent( QMouseEvent *pEvent )
{
    emit signalPos( pEvent->pos() );

    // move
    if ( pShapeBase ) 
    {
        pShapeBase->doMove( pEvent );
    }
}

void PCanvas::mouseReleaseEvent( QMouseEvent *pEvent )
{
    emit signalPos( pEvent->pos() );
    if ( pEvent->button() != Qt::LeftButton ) return;

    // release
    if ( pShapeBase ) 
    {
        pShapeBase->doRelease( pEvent );
        if ( bAutoCommit )
            doDrawCommit();
    }
}

void PCanvas::paintEvent( QPaintEvent *pEvent )
{
    QPainter painter( this );
    QRect dirtyRect = pEvent->rect();
    painter.drawImage( dirtyRect, image, dirtyRect );
}

void PCanvas::resizeEvent( QResizeEvent *pEvent )
{
    if ( width() > image.width() || height() > image.height() )
    {
        int newWidth = qMax( width() + 128, image.width() );
        int newHeight = qMax( height() + 128, image.height() );
        resizeImage( &image, QSize( newWidth, newHeight ) );
        update();
    }
    QWidget::resizeEvent( pEvent );
}

void PCanvas::setModified( bool b )
{
    if ( b == bModified ) return;

    bModified = b;
    emit signalStateChanged();
}

void PCanvas::doClear()
{
    image.fill( qRgb( 255, 255, 255 ) );
    stringFileName = QString();
    setModified( false );
    update();
}
 
void PCanvas::resizeImage( QImage *image, const QSize &newSize )
{
    if ( image->size() == newSize ) return;

    QImage newImage( newSize, QImage::Format_RGB32 );
    newImage.fill( qRgb( 255, 255, 255 ) );
    QPainter painter( &newImage );
    painter.drawImage( QPoint( 0, 0 ), *image );
    *image = newImage;
}

void PCanvas::print()
{
    QPrinter printer( QPrinter::HighResolution );

    QPrintDialog printDialog( &printer, this );
    if ( printDialog.exec() == QDialog::Accepted )
    {
        QPainter painter( &printer );
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale( rect.size(), Qt::KeepAspectRatio );
        painter.setViewport( rect.x(), rect.y(), size.width(), size.height() );
        painter.setWindow( image.rect() );
        painter.drawImage( 0, 0, image );
    }
}


