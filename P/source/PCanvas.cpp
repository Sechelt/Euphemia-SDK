#include "LibInfo.h"
#include "PCanvas.h"

#include "PGraphicsView.h"
#include "PGraphicsScene.h"

#include "PContext.h"

#include "PFillGradient.h"

PCanvas::PCanvas()
{
    setBackground( QColor( Qt::transparent ) );
//    setBackground( QColor( Qt::cyan ) );
}

void PCanvas::setImage( const QImage &i )
{
    // sanity check
    if ( isDrawing() ) doCancel();
    image = i;
    scene()->setSceneRect( QRectF( QPointF( 0, 0 ), image.size() ) );
    update();
    setModified();
}

void PCanvas::setTool( Tools n )
{
    if ( isDrawing() ) doCancel();
    nTool = n;
}

void PCanvas::setAutoCommit( bool b )
{
    PContextGeneral t = g_Context->getGeneral();
    if ( t.bAutoCommit == b ) return;

    if ( isDrawing() ) doCancel();

    t.bAutoCommit = b;
    g_Context->setGeneral( t );
}

/*!
 * \brief Sets the background color. Also sets bool to indicate whether or not its transparent. 
 *  
 * This does not flood the canvas with the color - it simply indicates, to the erase tool, what 
 * color it should erase to. A transparent background will cause the erase tool to reveal PBackground.
 *  
 * This should not be confused with \sa PBackground which indicates transparency and could do 
 * that with the classic checker pattern or a solid color. 
 *  
 * \author pharvey (2/16/23)
 * 
 * \param color  
 */
void PCanvas::setBackground( const QColor &color )      
{                                                       
    colorBackground = color;                            
    if (  colorBackground == QColor( Qt::transparent ) )
    {                                                   
        colorBackground.setAlpha( 0 );  // Qt::transparent does not get the alpha all the way to 0 so we do it here                  
        bBackgroundTransparent = true;                  
    }                                                   
    else                                                
        bBackgroundTransparent = false;                 
}                                                       
                                                        
PGraphicsView *PCanvas::getView()
{
    // get view with focus 
    QList <QGraphicsView *> l = scene()->views();
    for ( QGraphicsView *p : l )
    {
        if ( p->hasFocus() ) return (PGraphicsView *)p;
    }

    return nullptr;
}

PGraphicsScene *PCanvas::getScene() 
{ 
    return (PGraphicsScene*)scene(); 
}

QImage PCanvas::getCopy()
{
    if ( !pShapeBase ) return QImage();
    if ( !pShapeBase->canCopy() ) return QImage();
    return pShapeBase->getCopy();
}

bool PCanvas::getAutoCommit()
{
    return g_Context->getGeneral().bAutoCommit;
}

void PCanvas::doDoubleClickEvent( QGraphicsSceneMouseEvent *pEvent )
{
    // ignore if outside of scene (happens when outside of scene but within view)
    if ( !scene()->sceneRect().contains( pEvent->scenePos() ) ) return;
    // convert to our simplified mouse event
    PMouseEvent mouseEvent( pEvent );

    // only makes sense if we are drawing because we need a 'tool' to pass the event to
    if ( !isDrawing() ) return; 

    if ( pFreeBase )
        ;
    else if ( pShapeBase )
        pShapeBase->doDoubleClick( &mouseEvent );
}

void PCanvas::doPressEvent( QGraphicsSceneMouseEvent *pEvent )
{
    // ignore if outside of scene (happens when outside of scene but within view)
    if ( !scene()->sceneRect().contains( pEvent->scenePos() ) ) return;
    // convert to our simplified mouse event

    PMouseEvent mouseEvent( pEvent );

    // is a 'tool' active?
    if ( !isDrawing() ) 
    {
        // init 'tool'
        switch ( nTool )
        {
            case ToolSelectRectangle:    
                g_Context->setImage( &image );
                pShapeBase = new PSelectRectangle( this );           
                break;
            case ToolSelectEllipse:      
                g_Context->setImage( &image );
                pShapeBase = new PSelectEllipse( this );           
                break;
            case ToolSelectPolygon:      
                g_Context->setImage( &image );
                pShapeBase = new PSelectPolygon( this );           
                break;
            case ToolDrawFreeHand:               
                g_Context->setImage( &image );
                pFreeBase = new PDrawFreeHand( this );           
                break;
            case ToolDrawSpray:               
                g_Context->setImage( &image );
                pFreeBase = new PDrawSpray( this );           
                break;
            case ToolDrawErase:               
                g_Context->setImage( &image );
                pFreeBase = new PDrawErase( this );           
                break;
            case ToolDrawLine:
                g_Context->setImage( &image );
                pShapeBase = new PDrawLine( this );
                break;
            case ToolDrawRectangle:      
                g_Context->setImage( &image );
                pShapeBase = new PDrawRectangle( this );           
                break;
            case ToolDrawEllipse:        
                g_Context->setImage( &image );
                pShapeBase = new PDrawEllipse( this );           
                break;
            case ToolDrawPolygon:        
                g_Context->setImage( &image );
                pShapeBase = new PDrawPolygon( this );           
                break;
            case ToolDrawPolyline:        
                g_Context->setImage( &image );
                pShapeBase = new PDrawPolyline( this );           
                break;
            case ToolDrawRectangleFilled:
                g_Context->setImage( &image );
                pShapeBase = new PDrawRectangleFilled( this );           
                break;
            case ToolDrawEllipseFilled:  
                g_Context->setImage( &image );
                pShapeBase = new PDrawEllipseFilled( this );           
                break;
            case ToolDrawPolygonFilled:  
                g_Context->setImage( &image );
                pShapeBase = new PDrawPolygonFilled( this );           
                break;
            case ToolDrawText:  
                g_Context->setImage( &image );
                pShapeBase = new PDrawText( this );           
                break;
            case ToolFillFlood:          
                doFillFlood( mouseEvent.pos() );
                break;
            case ToolFillGradient:       
                g_Context->setImage( &image );
                pShapeBase = new PFillGradient( this );           
                break;
        }
        if ( pShapeBase ) 
        {
            scene()->addItem( pShapeBase );
            imagePreCommit = image;
            connect( pShapeBase, SIGNAL(signalChangedState()), SIGNAL(signalChangedState()) );
            connect( pShapeBase, SIGNAL(signalCommitted()), SLOT(slotCommitted()) );
        }
        emit signalChangedState();
    }

    // pass event to 'tool'
    if ( pFreeBase )
        pFreeBase->doPress( &mouseEvent );
    else if ( pShapeBase ) 
    {
        pShapeBase->doPress( &mouseEvent );
        if ( pShapeBase->inherits("PPasteRectangle") && pShapeBase->getState() == PShapeBase::StateIdle ) 
        {
            bPaste = false;
            doCancel();
        }
    }
}

void PCanvas::doMoveEvent( QGraphicsSceneMouseEvent *pEvent )
{
    // ignore if outside of scene (happens when outside of scene but within view)
    if ( !scene()->sceneRect().contains( pEvent->scenePos() ) ) return;
    // convert to our simplified mouse event
    PMouseEvent mouseEvent( pEvent );

    emit signalPos( mouseEvent.pos() );

    // move
    if ( pFreeBase )
        pFreeBase->doMove( &mouseEvent );
    else if ( pShapeBase )
        pShapeBase->doMove( &mouseEvent );
}

void PCanvas::doReleaseEvent( QGraphicsSceneMouseEvent *pEvent )
{
    // release is outside of scene is not a problem (prevented/handled in the move)
    // if ( !scene()->sceneRect().contains( pEvent->scenePos() ) ) return;

    // convert to our simplified mouse event
    PMouseEvent mouseEvent( pEvent );

    // pass event to 'tool'
    if ( pFreeBase )
    {
        pFreeBase->doRelease( &mouseEvent );
        // tools based upon PFreeBase can not be manipulated after drawn so we are done here
        delete pFreeBase;
        pFreeBase = nullptr;
        setModified();
        emit signalChangedState(); // setModified may - or may not - have done this so we do it here to ensure it happens
    }
    else if ( pShapeBase ) 
    {
        pShapeBase->doRelease( &mouseEvent );
    }
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
bool PCanvas::doOpen( const QString &s )
{
    // save any changes
    // - not likley to ever be modified here as open will be called from a new state - but just in case
    if ( isModified() )
    {
        // prompt to save, discard or cancel
        QMessageBox::StandardButton nButton = QMessageBox::question( qApp->activeWindow(), tr("Open..."), tr("Discard changes?") );
        if ( nButton == QMessageBox::No ) return false;
        if ( !doSave() ) return false;
        doClear();
    }

    // get a file name
    QString stringFileName = s;
    if ( stringFileName.isEmpty() )
        stringFileName = QFileDialog::getOpenFileName(qApp->activeWindow(), tr("Select Image..."), QString(), tr("Image (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"));
    if ( stringFileName.isEmpty() ) return false;

    // Qt supports svg but not via QImage

    // load the file
    QImageReader reader( stringFileName );
    if ( reader.read( &image ) ) 
    {
        image = image.convertToFormat( QImage::Format_ARGB32 );
        if ( scene() ) 
        {
            QRectF r = scene()->sceneRect();
            r.setSize( image.size() );
            scene()->setSceneRect( r );
        }

        this->stringFileName = stringFileName;
        emit signalChangedFileName( stringFileName );
        setModified( false );
        update();
        return true;
    }

    // error
    QMessageBox::warning( qApp->activeWindow(), tr("Open"), reader.errorString() );

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
    QMessageBox::warning( qApp->activeWindow(), tr("Save"), writer.errorString() );

    return false;
}

bool PCanvas::doSaveAs()
{
    // get a file name
    QString stringFileName = QFileDialog::getSaveFileName( qApp->activeWindow(), tr( "Save As..." ) );
    if ( stringFileName.isEmpty() ) return false;

    // Qt will use file extension to set format.

    // do it
    QImageWriter writer( stringFileName );
    if ( writer.write( image ) ) 
    {
        this->stringFileName = stringFileName;
        emit signalChangedFileName( stringFileName );
        setModified( false );
        return true;
    }

    // error
    QMessageBox::warning( qApp->activeWindow(), tr("Save"), writer.errorString() );

    return false;
}

bool PCanvas::doClose()
{
    // save any changes
    if ( isModified() )
    {
        // prompt to save, discard or cancel
        QMessageBox::StandardButton nButton = QMessageBox::question( qApp->activeWindow(), tr("Close..."), tr("Save changes?") );
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
    if ( pShapeBase  && pShapeBase->canCopy() ) 
    {
        pShapeBase->doCut();
        update();
        doPaste();
    }
}

void PCanvas::doCopy()
{
    if ( pShapeBase && pShapeBase->canCopy() ) 
    {
        pShapeBase->doCopy();
        doPaste();
    }
}

void PCanvas::doPaste()
{
    if ( isDrawing() ) doCancel();
    g_Context->setImage( &image );
    pShapeBase = new PPasteRectangle( this );
    scene()->addItem( pShapeBase );
    imagePreCommit = image;
    connect( pShapeBase, SIGNAL(signalChangedState()), SIGNAL(signalChangedState()) );
    connect( pShapeBase, SIGNAL(signalCommitted()), SLOT(slotCommitted()) );
    bPaste = true;
    emit signalChangedState();
}

void PCanvas::doPaste( const QImage &i )
{
    if ( isDrawing() ) doCancel();
    g_Context->setImage( &image );
    pShapeBase = new PPasteRectangle( this, i );
    scene()->addItem( pShapeBase );
    imagePreCommit = image;
    connect( pShapeBase, SIGNAL(signalChangedState()), SIGNAL(signalChangedState()) );
    connect( pShapeBase, SIGNAL(signalCommitted()), SLOT(slotCommitted()) );
    bPaste = true;
    emit signalChangedState();
}

void PCanvas::doUndo()
{
    Q_ASSERT( !stackUndo.isEmpty() );
    stackRedo.push( image );
    image = stackUndo.pop();
    update();
    emit signalChangedState();
}

void PCanvas::doRedo()
{
    Q_ASSERT( !stackRedo.isEmpty() );
    stackUndo.push( image );
    image = stackRedo.pop();
    update();
    emit signalChangedState();
}

void PCanvas::doSelectAll()
{
    if ( isDrawing() ) doCancel();
    g_Context->setImage( &image );
    pShapeBase = new PSelectRectangle( this, true );
    imagePreCommit = image;
    connect( pShapeBase, SIGNAL(signalChangedState()), SIGNAL(signalChangedState()) );
    connect( pShapeBase, SIGNAL(signalCommitted()), SLOT(slotCommitted()) );
    emit signalChangedState();
}

void PCanvas::doSelectNone()
{
    if ( !pShapeBase ) return;
    if ( !pShapeBase->isSelector() ) return;
    doCancel();
}

void PCanvas::doCrop()
{
    if ( !canCopy() ) return;

    QImage i = pShapeBase->getCopy();
    doCancel();
    image = i;
    scene()->setSceneRect( QRectF( 0, 0, image.size().width(), image.size().height() ) );
    update();
}

void PCanvas::doScale( int nX, int nY )
{
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << nX << nY;
}

void PCanvas::doFlipX()
{
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]";
}

void PCanvas::doFlipY()
{
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]";
}

void PCanvas::doRotate( int nDegrees )
{
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << nDegrees;
}

// app calls here ie tool button clicked
void PCanvas::doCommit()
{
    if ( !pShapeBase ) return;

    if ( pShapeBase->canCommit() ) 
        pShapeBase->doCommit(); // this should trigger PCanvas::slotCommitted()

    delete pShapeBase;
    pShapeBase = nullptr;
    imagePreCommit = QImage();
}

void PCanvas::doCancel()
{
    if ( !pShapeBase ) return;

    if ( pShapeBase->canCancel() ) 
        pShapeBase->doCancel();

    delete pShapeBase;
    pShapeBase = nullptr;
    imagePreCommit = QImage();
    emit signalChangedState();
}

bool PCanvas::isDrawing()
{
    if ( pFreeBase ) return true;
    if ( pShapeBase ) return true;
    return false;
}

bool PCanvas::hasSelection()
{
    if ( pShapeBase ) return pShapeBase->isSelector();
    return false;
}

bool PCanvas::canCut()
{
    if ( pShapeBase ) return pShapeBase->canCut();
    return false;
}

bool PCanvas::canCopy() 
{
    if ( pShapeBase ) return pShapeBase->canCopy();
    return false;
}

bool PCanvas::canPaste() 
{
    QClipboard *pClipboard = QGuiApplication::clipboard();
    const QMimeData *pMimeData = pClipboard->mimeData();

    if ( pMimeData->hasImage() ) return true;

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

bool PCanvas::canCommit() 
{
    if ( pShapeBase ) return pShapeBase->canCommit();
    return false;
}

bool PCanvas::canCancel() 
{
    if ( pShapeBase ) return pShapeBase->canCancel();
    return false;
}

// shape has just called its doCommit
// - shape may have decided to do it in which case shape has gone idle - it will get deleted when dif tool selected
//   isDrawing remains true
// - app may have called our doCommit in which case shape will get deleted
//   isDrawing becomes false
void PCanvas::slotCommitted()
{
    // undo/redo
    stackRedo.clear();
    stackUndo.push( imagePreCommit );
    if ( stackUndo.count() >= nMaxUndo ) stackUndo.removeFirst();
    imagePreCommit = image;
    // we have been modified
    setModified();
    // setModified may - or may not - have done this so we do it here to ensure it happens
    emit signalChangedState(); 
}

void PCanvas::slotZoomChanged()
{
    if ( pShapeBase ) pShapeBase->doZoom();
}

void PCanvas::paint( QPainter *pPainter, const QStyleOptionGraphicsItem *nOption, QWidget *pWidget )
{
    Q_UNUSED( nOption );
    Q_UNUSED( pWidget );

    // has scene been resized?
    if ( boundingRect().size().toSize() != image.size() )
    {
        QImage i( boundingRect().size().toSize(), QImage::Format_ARGB32 );
        i.fill( Qt::transparent );
        QPainter p( &i );
        p.drawImage( 0, 0, image );
        image = i;
    }

    pPainter->drawImage( 0, 0, image );
}

QRectF PCanvas::boundingRect() const
{
    if ( scene() ) return scene()->sceneRect();
    return QRectF();
}

void PCanvas::setModified( bool b )
{
    if ( b == bModified ) return;

    bModified = b;
    emit signalChangedState();
}

void PCanvas::doFillFlood( const QPoint &pointSeed )
{
    if ( g_Context->getBrush().style() == Qt::SolidPattern )
        doFillFloodColor( pointSeed );
    else if ( !g_Context->getBrush().textureImage().isNull() )
        doFillFloodTiled( pointSeed );
    else if ( g_Context->getBrush().style() != Qt::NoBrush )
        doFillFloodTiled( pointSeed );
    else
        QMessageBox::warning( qApp->activeWindow(), tr("Flood Fill"), tr("Please select a brush.") );
    update();
}

/*!
 * \brief Flood fill with current brush color.
 *  
 * This is not super efficient (can check pixel 4x) but it is a super simple 
 * bit of code. 
 *  
 * Lets Qt worry about color format. 
 *  
 * \author pharvey (1/27/23)
 * 
 * \param pointSeed 
 */
void PCanvas::doFillFloodColor( const QPoint &pointSeed )
{
    int nWidth          = image.size().width();
    int nHeight         = image.size().height();
    QColor colorNew     = g_Context->getBrush().color();            // fill color
    QColor colorSeed    = image.pixelColor( pointSeed );            // color at seed point - color we are replacing
    QStack<QPoint> stackPointsToProcess;

    // prime stack with seed point
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        int nX = point.x();
        int nY = point.y();
        if ( image.pixelColor( point ) == colorNew ) continue;      // we already changed this pixel or we have hit a boundary (which happens to be fill color) so do nothing
        if ( image.pixelColor( point ) != colorSeed ) continue;     // not the color we want to fill over so do nothing
        image.setPixelColor( point, colorNew );                     // fill
        // check N
        if ( nY - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( nX, nY - 1 ) );
        }
        // check S
        if ( nY + 1 < nHeight )
        {
            stackPointsToProcess.push( QPoint( nX, nY + 1 ) );
        }
        // check E
        if ( nX + 1 < nWidth )
        {
            stackPointsToProcess.push( QPoint( nX + 1, nY ) );
        }
        // check W
        if ( nX - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( nX - 1, nY ) );
        }
    }
}

/*!
 * \brief Flood fill with current brush pattern or texture.
 *  
 * This is based upon doFillFloodColor() but addresses the fact that 
 * a pattern/texture fill does not always result in a target pixel being 
 * changed. 
 *  
 * So just checking the color of the pixel does not tell us if 
 * that pixel has already been processed. 
 *  
 * We add a 2d array indicating whether or not a pixel has been 
 * processed. 
 *  
 * This also; 
 *  - reduces use of the stack
 *  - reduces full checks for a given point to 0-1
 *  
 * \author pharvey (1/27/23)
 * 
 * \param point 
 */
void PCanvas::doFillFloodTiled( const QPoint &pointSeed )
{
    int nWidth              = image.size().width();
    int nHeight             = image.size().height();
    bool tableProcessed[nWidth][nHeight];                               // cell == true if point processed (auto inits to 0)
    QColor colorSeed        = image.pixelColor( pointSeed );            // we will use to determine the boundary
    QImage imageSource      = QImage( image.size(), image.format() );   // we will use to get our new pixel colors

    // Initialize the source image.
    {
        // We will use a pattern with a transparent background so fill with the seed color.
        imageSource.fill( colorSeed );
        // Fill with pattern.
        QPainter painter( &imageSource );
        painter.fillRect( 0, 0, nWidth, nHeight, g_Context->getBrush() );
    }

    // prime stack with seed point
    QStack<QPoint> stackPointsToProcess;
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        int nX = point.x();
        int nY = point.y();
        tableProcessed[nX][nY] = true;                                      // first and last time we will process this point 
        if ( image.pixelColor( point ) != colorSeed ) continue;             // not the color we want to fill over so do nothing
        image.setPixelColor( point, imageSource.pixelColor( point ) );      // fill
        // look N
        if ( nY - 1 >= 0 && tableProcessed[nX][nY-1] == false )
        {
            stackPointsToProcess.push( QPoint( nX, nY - 1 ) );
        }
        // look S
        if ( nY + 1 < nHeight && tableProcessed[nX][nY+1] == false )
        {
            stackPointsToProcess.push( QPoint( nX, nY + 1 ) );
        }
        // look E
        if ( nX + 1 < nWidth && tableProcessed[nX+1][nY] == false )
        {
            stackPointsToProcess.push( QPoint( nX + 1, nY ) );
        }
        // look W
        if ( nX - 1 >= 0 && tableProcessed[nX-1][nY] == false )
        {
            stackPointsToProcess.push( QPoint( nX - 1, nY ) );
        }
    }
}

void PCanvas::doFillGradient( const QPoint &pointSeed )
{
    int nWidth              = image.size().width();
    int nHeight             = image.size().height();
    bool tableProcessed[nWidth][nHeight];                               // cell == true if point processed (auto inits to 0)
    QColor colorSeed        = image.pixelColor( pointSeed );            // we will use to determine the boundary
    QImage imageSource      = QImage( image.size(), image.format() );   // we will use to get our new pixel colors

    // Initialize the source image.
    {
        // We will use a pattern with a transparent background so fill with the seed color.
        imageSource.fill( colorSeed );
        // Fill with pattern.
        QPainter painter( &imageSource );
        // QBrush brush = g_Context->getBrush();
        // QGradient gradient = g_Context->getGradient();
        // QGradient gradient;

        // example 1
        QRadialGradient radialGrad( pointSeed, 100 );
        radialGrad.setColorAt( 0, Qt::red);
        radialGrad.setColorAt( 0.5, Qt::blue);
        radialGrad.setColorAt( 1, Qt::green);

        // example 2 (preset)
        QGradient gradient( QGradient::WarmFlame );
        gradient.setColorAt( 0, Qt::red);
        gradient.setColorAt( 0.5, Qt::blue);
        gradient.setColorAt( 1, Qt::green);

        painter.fillRect( 0, 0, nWidth, nHeight, QBrush( gradient ) );
return;
    }

    // prime stack with seed point
    QStack<QPoint> stackPointsToProcess;
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        int nX = point.x();
        int nY = point.y();
        tableProcessed[nX][nY] = true;                                      // first and last time we will process this point 
        if ( image.pixelColor( point ) != colorSeed ) continue;             // not the color we want to fill over so do nothing
        image.setPixelColor( point, imageSource.pixelColor( point ) );      // fill
        // look N
        if ( nY - 1 >= 0 && tableProcessed[nX][nY-1] == false )
        {
            stackPointsToProcess.push( QPoint( nX, nY - 1 ) );
        }
        // look S
        if ( nY + 1 < nHeight && tableProcessed[nX][nY+1] == false )
        {
            stackPointsToProcess.push( QPoint( nX, nY + 1 ) );
        }
        // look E
        if ( nX + 1 < nWidth && tableProcessed[nX+1][nY] == false )
        {
            stackPointsToProcess.push( QPoint( nX + 1, nY ) );
        }
        // look W
        if ( nX - 1 >= 0 && tableProcessed[nX-1][nY] == false )
        {
            stackPointsToProcess.push( QPoint( nX - 1, nY ) );
        }
    }
}

void PCanvas::doClear()
{
    image.fill( colorBackground );
    stringFileName = QString();
    emit signalChangedFileName( stringFileName );
    setModified( false );
    update();
}
 
void PCanvas::print()
{
    QPrinter printer( QPrinter::HighResolution );

    QPrintDialog printDialog( &printer, qApp->activeWindow() );
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


