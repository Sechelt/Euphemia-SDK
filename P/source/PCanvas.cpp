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
                g_Context->setImage( &image );
                pShapeBase = new PSelectRectangle( this, pEvent->pos() );           
                break;
            case ToolSelectEllipse:      
                g_Context->setImage( &image );
                pShapeBase = new PSelectEllipse( this, pEvent->pos() );           
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
                g_Context->setImage( &image );
                pShapeBase = new PDrawRectangleFilled( this, pEvent->pos() );           
                break;
            case ToolDrawEllipseFilled:  
                g_Context->setImage( &image );
                pShapeBase = new PDrawEllipseFilled( this, pEvent->pos() );           
                break;
            case ToolDrawPolygonFilled:  
                break;
            case ToolDrawText:  
                break;
            case ToolFillFlood:          
                doFillFlood( pEvent->pos() );
                break;
            case ToolFillGradient:       
                doFillGradient( pEvent->pos() );
                break;
        }
    }

    // IF shape THEN pass press event to shape
    if ( pShapeBase ) 
    {
        // IF not used by shape THEN cancel/commit
        if ( !pShapeBase->doPress( pEvent ) )
        {
            // IF selection THEN cancel ELSE commit
            // - in both cases the shape is deleted
            if ( pShapeBase->inherits( "PSelectRectangle" ) ||
                 pShapeBase->inherits( "PSelectEllipse" ) ||
                 pShapeBase->inherits( "PSelectPolygon" ) )
            {
                doDrawCancel();
            }
            else
            {
                doDrawCommit();
            }
        }
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

    // shapes can be manipulated after draw when !bAutoCommit
    if ( pShapeBase ) 
    {
        pShapeBase->doRelease( pEvent );
        // a commit does not make sense for selection shapes  
        if ( bAutoCommit && 
             !( pShapeBase->inherits( "PSelectRectangle" ) ||
                pShapeBase->inherits( "PSelectEllipse" ) ||
                pShapeBase->inherits( "PSelectPolygon" ) ) )
        {
            doDrawCommit();
        }
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

void PCanvas::doFillFlood( const QPoint &pointSeed )
{
    if ( !g_Context->getBrush().textureImage().isNull() )
        doFillFloodTexture2( pointSeed );
    else if ( g_Context->getBrush().style() == Qt::SolidPattern )
        doFillFloodColor( pointSeed );
    else if ( g_Context->getBrush().style() != Qt::NoBrush )
        doFillFloodPattern2( pointSeed );

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
    QColor colorNew = g_Context->getBrush().color();            // fill color
    QColor colorOld = image.pixelColor( pointSeed );            // color at seed point - color we are replacing
    QStack<QPoint> stackPointsToProcess;

    // nothing to do if new color is same as color at seed point
    if ( colorNew == colorOld ) return;
    // prime stack with seed point
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        if ( image.pixelColor( point ) == colorNew ) continue;
        if ( image.pixelColor( point ) != colorOld ) continue;
        image.setPixelColor( point, colorNew );
        // check N
        if ( point.y() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() - 1 ) );
        }
        // check S
        if ( point.y() + 1 < image.height() )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() + 1 ) );
        }
        // check E
        if ( point.x() + 1 < image.width() )
        {
            stackPointsToProcess.push( QPoint( point.x() + 1, point.y() ) );
        }
        // check W
        if ( point.x() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x() - 1, point.y() ) );
        }
    }
}

/*!
 * \brief Flood fill with current brush color.
 *  
 * Code inspired but public code. 
 *  
 * Colors need to be RGB and does not factor in alpha channel. 
 *  
 * \author pharvey (1/27/23)
 * 
 * \param point 
 */
void PCanvas::doFillFloodColor2( const QPoint &point )
{
    int     XPos        = point.x();
    int     YPos        = point.y();
    int     NewR, NewG, NewB;

    QColor color = g_Context->getBrush().color();

    // sanity check
    if ( color == image.pixelColor( point ) ) return;

    color.getRgb( &NewR, &NewG, &NewB );

    image = image.convertToFormat(QImage::Format_RGBA8888);

    uchar *bits = image.bits();

    int origR, origG, origB;
    int begPixel = XPos * 4 + YPos * 4 * image.width();

    origR = bits[ begPixel ];
    origG = bits[ begPixel + 1 ];
    origB = bits[ begPixel + 2 ];

    bool reachLeft( false );
    bool reachRight( false );

    QList< QPair<int, int> > pixelStack;
    pixelStack.push_back( QPair< int, int>( XPos, YPos ) );
    while( pixelStack.length() > 0 )
    {
        reachLeft = false;
        reachRight = false;

        QPair<int, int> currentPixel = pixelStack[0];
        pixelStack.pop_front();
        begPixel = currentPixel.first * 4 + currentPixel.second * 4 * image.width();

        while( currentPixel.second - 1 >= 0 )
        {
            begPixel = currentPixel.first * 4 + currentPixel.second * 4 * image.width();
            if( !( ( bits[ begPixel ] == origR ) && ( bits[ begPixel + 1 ] == origG ) && ( bits[ begPixel + 2] == origB) ) )
            {
                break;
            }
            currentPixel.second--;
        }

        currentPixel.second++;
        begPixel = currentPixel.first * 4 + currentPixel.second * 4 * image.width();
        bits[ begPixel ] = NewR;
        bits[ begPixel + 1 ] = NewG;
        bits[ begPixel + 2 ] = NewB;
        bits[ begPixel + 3 ] = 255;

        while( currentPixel.second + 1 < image.height() )
        {
            currentPixel.second++;
            begPixel = currentPixel.first * 4 + currentPixel.second * 4 * image.width();
            if( ( ( bits[ begPixel ] == origR ) && ( bits[ begPixel + 1 ] == origG ) && ( bits[ begPixel + 2] == origB) ) )
            {
                bits[ begPixel ] = NewR;
                bits[ begPixel + 1 ] = NewG;
                bits[ begPixel + 2 ] = NewB;
                bits[ begPixel + 3 ] = 255;

                if( currentPixel.first > 1 )
                {
                    int leftPixel = ( currentPixel.first - 1 ) * 4 + currentPixel.second * 4 * image.width();
                    if( ( ( bits[ leftPixel ] == origR ) && ( bits[ leftPixel + 1 ] == origG ) && ( bits[ leftPixel + 2] == origB) ) )
                    {
                        if( !reachLeft )
                        {
                            pixelStack.push_back(QPair< int, int> ( currentPixel.first - 1, currentPixel.second ) );
                            reachLeft = true;
                        }
                    }
                    else
                    {
                        reachLeft = false;
                    }
                }

                if ( currentPixel.first < image.width() )
                {
                    int rightPixel = ( currentPixel.first + 1 ) * 4 + currentPixel.second * 4 * image.width();
                    if( ( ( bits[ rightPixel ] == origR ) && ( bits[ rightPixel + 1 ] == origG ) && ( bits[ rightPixel + 2] == origB) ) )
                    {
                        if( !reachRight )
                        {
                            pixelStack.push_back(QPair< int, int> ( currentPixel.first + 1, currentPixel.second ) );
                            reachRight = true;
                        }
                    }
                    else
                    {
                        reachRight = false;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
}

/*!
 * \brief Flood fill with current brush pattern.
 *  
 * Uses three images; 
 *          1) original image(to be modified) 
 *          2) copy of image to be used to determine fill boundary
 *          3) patterned filled as source for new pixel color
 *  
 * Fill is otherwise based upon doFillFloodColor().
 *  
 * \author pharvey (1/27/23)
 * 
 * \param point 
 */
void PCanvas::doFillFloodPattern( const QPoint &pointSeed )
{
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ <<"]";
    // fill entire image to get our new pixel colors 
    QImage imageFilled = QImage( image.size(), image.format() );   // we will use to get our new pixel colors
    {
        QPainter painter( &imageFilled );
        // painter.setPen( g_Context->getPen() /* Qt::NoPen */ );
        // painter.setBrush( g_Context->getBrush() );
        painter.fillRect( 0, 0, image.size().width(), image.size().height(), g_Context->getBrush() );
    }

    // get the color we are filling over
    QColor colorOld = image.pixelColor( pointSeed );

    // prime stack with seed point
    QStack<QPoint> stackPointsToProcess;
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        if ( image.pixelColor( point ) != colorOld ) continue;
        image.setPixelColor( point, imageFilled.pixelColor( point ) );
        // check N
        if ( point.y() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() - 1 ) );
        }
        // check S
        if ( point.y() + 1 < image.height() )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() + 1 ) );
        }
        // check E
        if ( point.x() + 1 < image.width() )
        {
            stackPointsToProcess.push( QPoint( point.x() + 1, point.y() ) );
        }
        // check W
        if ( point.x() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x() - 1, point.y() ) );
        }
    }
}

/*!
 * \brief Flood fill with current brush pattern.
 *  
 * Uses three images; 
 *          1) original image(to be modified) 
 *          2) copy of image to be used to determine fill boundary
 *          3) patterned filled as source for new pixel color
 *  
 * Fill is otherwise based upon doFillFloodColor2().
 *  
 * \author pharvey (1/27/23)
 * 
 * \param point 
 */
void PCanvas::doFillFloodPattern2( const QPoint &point )
{
    QPair<int,int> pointSeed( point.x(), point.y() );                           // seed point (QPair works as a map key - QPoint does not)
    QColor colorSeed        = image.pixel( point );                           // seed color (color clicked on for fill)
    QImage imageOriginal    = image;                                          // we will use to determine the boundary
    QImage imageFilled      = QImage( image.size(), image.format() );       // we will use to get our new pixel colors
    bool bReachWest         = false;                                            // we do not always need to process point to the West
    bool bReachEast         = false;                                            // we do not always need to process point to the East
    QList<QPair<int,int>>       stackPointsToProcess;                           // points to process (based upon peeking East and West)
    QMap<QPair<int,int>,int>    mapPointsChanged;                               // points to ignore because we have already processed them

    QBrush brush = g_Context->getBrush();

    // fill entire image to get our new pixel colors 
    // - for now; fill is just a brush (color/pattern/texture) but this could be an algo in the future
    {
        QPainter painter( &imageFilled );
        painter.setPen( Qt::NoPen );
        painter.setBrush( brush );
        painter.fillRect( 0, 0, image.size().width(), image.size().height(), brush );
    }

    // start with seed point
    stackPointsToProcess.push_back( pointSeed );

    // process until point stack is empty
    // - we fill by working our way down the y-axis
    // - we move along the x-axis by alternating between West and East
    while ( stackPointsToProcess.length() > 0 )
    {
        bReachWest = false;
        bReachEast = false;

        // process first point on stack
        QPair<int,int> pointCurrent = stackPointsToProcess[0];
        stackPointsToProcess.pop_front();

        // go North until we hit; edge of image, fill boundary, or processed point
        // - no processing - just move
        while ( pointCurrent.second - 1 >= 0 )
        {
            // have we hit the boundary?
            QColor colorOriginal = imageOriginal.pixel( pointCurrent.first, pointCurrent.second );
            if ( colorOriginal != colorSeed || mapPointsChanged.contains( pointCurrent )) break;
            pointCurrent.second--;
        }
//        pointCurrent.second++;

        // work our way South until we hit edge of image or fill boundary
        // - process each point along the way
        while ( pointCurrent.second + 1 < image.height() )
        {
            pointCurrent.second++;

            // have we hit the boundary?
            QColor colorOriginal = imageOriginal.pixel( pointCurrent.first, pointCurrent.second );
            if ( colorOriginal != colorSeed || mapPointsChanged.contains( pointCurrent ) ) break;

            // apply fill at current point
            image.setPixelColor( pointCurrent.first, pointCurrent.second, imageFilled.pixel( pointCurrent.first, pointCurrent.second ) );
            mapPointsChanged.insert( pointCurrent, 0 );

            // is there a point to the West?
            if ( pointCurrent.first > 1 )
            {
                // does the point need to be processed?
                QPair<int, int> pointWest( pointCurrent.first - 1, pointCurrent.second );
                QColor colorOriginal = imageOriginal.pixel( pointWest.first, pointWest.second );
                if ( colorOriginal == colorSeed && !mapPointsChanged.contains( pointWest ) )
                {
                    if ( !bReachWest )
                    {
                        stackPointsToProcess.push_back( pointWest );
                        bReachWest = true;
                    }
                }
                else
                    bReachWest = false;
            }

            // is there a point to the East?
            if ( pointCurrent.first < image.width() )
            {
                // does the point need to be processed?
                QPair<int, int> pointEast( pointCurrent.first + 1, pointCurrent.second );
                QColor colorOriginal = imageOriginal.pixel( pointEast.first, pointEast.second );
                if ( colorOriginal == colorSeed && !mapPointsChanged.contains( pointEast ) )
                {
                    if ( !bReachEast )
                    {
                        stackPointsToProcess.push_back( pointEast );
                        bReachEast = true;
                    }
                }
                else
                    bReachEast = false;
            }
        } // while
    } // while
}

void PCanvas::doFillFloodTexture( const QPoint &pointSeed )
{
    // fill entire image to get our new pixel colors 
    QImage imageFilled = QImage( image.size(), image.format() );   // we will use to get our new pixel colors
    {
        QPainter painter( &imageFilled );
        painter.setPen( Qt::NoPen );
        painter.setBrush( g_Context->getBrush() );
        painter.fillRect( 0, 0, image.size().width(), image.size().height(), g_Context->getBrush() );
    }

    // get the color we are filling over
    QColor colorOld = image.pixelColor( pointSeed );

    // prime stack with seed point
    QStack<QPoint> stackPointsToProcess;
    stackPointsToProcess.push( pointSeed );
    // process until stack is empty
    while ( !stackPointsToProcess.isEmpty() )
    {
        QPoint point = stackPointsToProcess.pop();
        if ( image.pixelColor( point ) != colorOld ) continue;
        image.setPixelColor( point, imageFilled.pixelColor( point ) );
        // check N
        if ( point.y() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() - 1 ) );
        }
        // check S
        if ( point.y() + 1 < image.height() )
        {
            stackPointsToProcess.push( QPoint( point.x(), point.y() + 1 ) );
        }
        // check E
        if ( point.x() + 1 < image.width() )
        {
            stackPointsToProcess.push( QPoint( point.x() + 1, point.y() ) );
        }
        // check W
        if ( point.x() - 1 >= 0 )
        {
            stackPointsToProcess.push( QPoint( point.x() - 1, point.y() ) );
        }
    }
}

void PCanvas::doFillFloodTexture2( const QPoint &point )
{
    QPair<int,int> pointSeed( point.x(), point.y() );                           // seed point (QPair works as a map key - QPoint does not)
    QColor colorSeed        = image.pixel( point );                           // seed color (color clicked on for fill)
    QImage imageOriginal    = image;                                            // we will use to determine the boundary
    QImage imageFilled      = QImage( image.size(), image.format() );       // we will use to get our new pixel colors
    bool bReachWest         = false;                                            // we do not always need to process point to the West
    bool bReachEast         = false;                                            // we do not always need to process point to the East
    QList<QPair<int,int>>       stackPointsToProcess;                           // points to process (based upon peeking East and West)
    QMap<QPair<int,int>,int>    mapPointsChanged;                               // points to ignore because we have already processed them

    QBrush brush = g_Context->getBrush();

    // fill entire image to get our new pixel colors 
    // - for now; fill is just a brush (color/pattern/texture) but this could be an algo in the future
    {
        QPainter painter( &imageFilled );
        painter.setPen( Qt::NoPen );
        painter.setBrush( brush );
        painter.fillRect( 0, 0, image.size().width(), image.size().height(), brush );
    }

    // start with seed point
    stackPointsToProcess.push_back( pointSeed );

    // process until point stack is empty
    // - we fill by working our way down the y-axis
    // - we move along the x-axis by alternating between West and East
    while ( stackPointsToProcess.length() > 0 )
    {
        bReachWest = false;
        bReachEast = false;

        // process first point on stack
        QPair<int,int> pointCurrent = stackPointsToProcess[0];
        stackPointsToProcess.pop_front();

        // go North until we hit; edge of image, fill boundary, or processed point
        // - no processing - just move
        while ( pointCurrent.second - 1 >= 0 )
        {
            // have we hit the boundary?
            QColor colorOriginal = imageOriginal.pixel( pointCurrent.first, pointCurrent.second );
            if ( colorOriginal != colorSeed || mapPointsChanged.contains( pointCurrent )) break;
            pointCurrent.second--;
        }
//        pointCurrent.second++;

        // work our way South until we hit edge of image or fill boundary
        // - process each point along the way
        while ( pointCurrent.second + 1 < image.height() )
        {
            pointCurrent.second++;

            // have we hit the boundary?
            QColor colorOriginal = imageOriginal.pixel( pointCurrent.first, pointCurrent.second );
            if ( colorOriginal != colorSeed || mapPointsChanged.contains( pointCurrent ) ) break;

            // apply fill at current point
            image.setPixelColor( pointCurrent.first, pointCurrent.second, imageFilled.pixel( pointCurrent.first, pointCurrent.second ) );
            mapPointsChanged.insert( pointCurrent, 0 );

            // is there a point to the West?
            if ( pointCurrent.first > 1 )
            {
                // does the point need to be processed?
                QPair<int, int> pointWest( pointCurrent.first - 1, pointCurrent.second );
                QColor colorOriginal = imageOriginal.pixel( pointWest.first, pointWest.second );
                if ( colorOriginal == colorSeed && !mapPointsChanged.contains( pointWest ) )
                {
                    if ( !bReachWest )
                    {
                        stackPointsToProcess.push_back( pointWest );
                        bReachWest = true;
                    }
                }
                else
                    bReachWest = false;
            }

            // is there a point to the East?
            if ( pointCurrent.first < image.width() )
            {
                // does the point need to be processed?
                QPair<int, int> pointEast( pointCurrent.first + 1, pointCurrent.second );
                QColor colorOriginal = imageOriginal.pixel( pointEast.first, pointEast.second );
                if ( colorOriginal == colorSeed && !mapPointsChanged.contains( pointEast ) )
                {
                    if ( !bReachEast )
                    {
                        stackPointsToProcess.push_back( pointEast );
                        bReachEast = true;
                    }
                }
                else
                    bReachEast = false;
            }
        } // while
    } // while
}

void PCanvas::doFillGradient( const QPoint &pointSeed )
{
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


