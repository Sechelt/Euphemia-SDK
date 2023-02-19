#include "LibInfo.h"
#include "PContext.h"

#define PCONTEXT_DOC_CLASS "PContext"

void PContextGradient::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
}

void PContextGradient::doLoad( QDomElement *pdomElem )
{
}

void PContextGradientLinear::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
}

void PContextGradientLinear::doLoad( QDomElement *pdomElem )
{
}

void PContextGradientRadial::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
}

void PContextGradientRadial::doLoad( QDomElement *pdomElem )
{
}

void PContextGradientConical::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
}

void PContextGradientConical::doLoad( QDomElement *pdomElem )
{
}

void PContextFreeHand::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    pdomElem->setAttribute( "Shape", nShape );
    pdomElem->setAttribute( "Width", size.width() );
    pdomElem->setAttribute( "Height", size.height() );
    if ( !image.isNull() ) pdomElem->appendChild( WPersistNative::doSaveImage( image, pdomDoc, "Image" ) );
}

void PContextFreeHand::doLoad( QDomElement *pdomElem )
{
    nShape = PContextFreeHand::Shapes(pdomElem->attribute( "Shape", QString::number( nShape ) ).toInt());
    size.setWidth( pdomElem->attribute( "Width", QString::number( size.width() ) ).toInt() );
    size.setHeight( pdomElem->attribute( "Height", QString::number( size.height() ) ).toInt() );

    // load child nodes (of interest)
    QDomElement         domElem;
    QDomNode            domNode;
    domNode = pdomElem->firstChild();
    while( !domNode.isNull() ) 
    {
        domElem = domNode.toElement();
        if( domElem.isNull() ) 
        {
            domNode = domNode.nextSibling();
            continue;
        }

        if ( domElem.tagName() == "Image" ) image = WPersistNative::doLoadImage( pdomElem );

        domNode = domNode.nextSibling();
    }
}

void PContextErase::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    pdomElem->setAttribute( "Shape", nShape );
    pdomElem->setAttribute( "Width", size.width() );
    pdomElem->setAttribute( "Height", size.height() );
    if ( !image.isNull() ) pdomElem->appendChild( WPersistNative::doSaveImage( image, pdomDoc, "Image" ) );
}

void PContextErase::doLoad( QDomElement *pdomElem )
{
    nShape = PContextErase::Shapes(pdomElem->attribute( "Shape", QString::number( int(nShape) ) ).toInt());
    size.setWidth( pdomElem->attribute( "Width", QString::number( size.width() ) ).toInt() );
    size.setHeight( pdomElem->attribute( "Height", QString::number( size.height() ) ).toInt() );

    // load child nodes (of interest)
    QDomElement         domElem;
    QDomNode            domNode;
    domNode = pdomElem->firstChild();
    while( !domNode.isNull() ) 
    {
        domElem = domNode.toElement();
        if( domElem.isNull() ) 
        {
            domNode = domNode.nextSibling();
            continue;
        }

        if ( domElem.tagName() == "Image" ) image = WPersistNative::doLoadImage( pdomElem );

        domNode = domNode.nextSibling();
    }
}

void PContextPaste::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    Q_UNUSED(pdomDoc);
    pdomElem->setAttribute( "CompositionMode", nCompositionMode );
    pdomElem->setAttribute( "Stamp", bStamp );
}

void PContextPaste::doLoad( QDomElement *pdomElem )
{
    nCompositionMode = QPainter::CompositionMode(pdomElem->attribute( "CompositionMode", QString::number( int(nCompositionMode) ) ).toInt());
    bStamp = pdomElem->attribute( "Stamp", QString::number( bStamp ) ).toInt();
}

void PContextSpray::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    Q_UNUSED(pdomDoc);
    pdomElem->setAttribute( "Radius", nRadius );
    pdomElem->setAttribute( "Points", nPoints );
}

void PContextSpray::doLoad( QDomElement *pdomElem )
{
    nRadius = pdomElem->attribute( "Radius", QString::number( nRadius ) ).toInt();
    nPoints = pdomElem->attribute( "Points", QString::number( nPoints ) ).toInt();
}

void PContextPolygonFilled::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    Q_UNUSED(pdomDoc);
    pdomElem->setAttribute( "FillRule", nFillRule );
}

void PContextPolygonFilled::doLoad( QDomElement *pdomElem )
{
    nFillRule = Qt::FillRule(pdomElem->attribute( "FillRule", QString::number( nFillRule ) ).toInt());
}

void PContextText::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    Q_UNUSED(pdomDoc);
    pdomElem->setAttribute( "HAlign", nHAlign );
    pdomElem->setAttribute( "nVAlign", nVAlign );
}

void PContextText::doLoad( QDomElement *pdomElem )
{
    nHAlign = Qt::AlignmentFlag(pdomElem->attribute( "HAlign", QString::number( int(nHAlign) ) ).toInt());
    nVAlign = Qt::AlignmentFlag(pdomElem->attribute( "VAlign", QString::number( int(nVAlign) ) ).toInt());
}

PContextGeneral::PContextGeneral()
{
    brushTransparency.setTextureImage( QImage( ":W/Transparent" ) );
}

void PContextGeneral::doSave( QDomDocument *pdomDoc, QDomElement *pdomElem )
{
    pdomElem->setAttribute( "RestoreState", bRestoreState );
    pdomElem->setAttribute( "AutoCommit", bAutoCommit );
    pdomElem->appendChild( WPersistNative::doSaveBrush( brushTransparency, pdomDoc, "BrushTransparency" ) );
}

void PContextGeneral::doLoad( QDomElement *pdomElem )
{
    // clear to get rid of any texture
    brushTransparency = QBrush();

    bRestoreState = Qt::AlignmentFlag(pdomElem->attribute( "RestoreState", QString::number( int(bRestoreState) ) ).toInt());
    bAutoCommit = Qt::AlignmentFlag(pdomElem->attribute( "AutoCommit", QString::number( int(bAutoCommit) ) ).toInt());

    // load child nodes (of interest)
    QDomElement         domElem;
    QDomNode            domNode;
    domNode = pdomElem->firstChild();
    while( !domNode.isNull() ) 
    {
        domElem = domNode.toElement();
        if( domElem.isNull() ) 
        {
            domNode = domNode.nextSibling();
            continue;
        }

        if ( domElem.tagName() == "BrushTransparency" ) brushTransparency = WPersistNative::doLoadBrush( &domElem );

        domNode = domNode.nextSibling();
    }
}

PContext::PContext()
{
    QString stringDir = QStandardPaths::writableLocation( QStandardPaths::ConfigLocation );
    stringDir += ("/" LIB_ORG);

    QDir dir;
    if ( !dir.exists( stringDir ) ) 
    {
        dir.mkdir( stringDir );
        stringDir += ("/" LIB_NAME);
        dir.mkdir( stringDir );
    }

    stringFileName += stringDir + ("/" PCONTEXT_DOC_CLASS ".xml");
}

PContext* PContext::instance()
{
    static PContext instance;
    return &instance;
}

void PContext::setImage( QImage *p )
{
    if ( pImage == p ) return;
    pImage = p;
}

void PContext::setPen( const QPen &t )
{
    if ( pen == t ) return;
    pen = t;
    emit signalModified( pen );
}

void PContext::setBrush( const QBrush &t )
{
    if ( brush == t ) return;
    brush = t;
    emit signalModified( brush );
}

void PContext::setFont( const QFont &t )
{
    if ( font == t ) return;
    font = t;
    emit signalModified( font );
}

void PContext::setGradient( const PContextGradient &t )
{
    if ( gradient == t ) return;
    gradient = t;
    emit signalModified( gradient );
}

void PContext::setGradientLinear( const PContextGradientLinear &t )
{
    if ( gradientlinear == t ) return;
    gradientlinear = t;
    emit signalModified( gradientlinear );
}

void PContext::setGradientRadial( const PContextGradientRadial &t )
{
    if ( gradientradial == t ) return;
    gradientradial = t;
    emit signalModified( gradientradial );
}

void PContext::setGradientConical( const PContextGradientConical &t )
{
    if ( gradientconical == t ) return;
    gradientconical = t;
    emit signalModified( gradientconical );
}

void PContext::setFreeHand( const PContextFreeHand &t )
{
    if ( freehand == t ) return;
    freehand = t;
    emit signalModified( freehand );
}

void PContext::setErase( const PContextErase &t )
{
    if ( erase == t ) return;
    erase = t;
    emit signalModified( erase );
}

void PContext::setSpray( const PContextSpray &t )
{
    if ( spray == t ) return;
    spray = t;
    emit signalModified( spray );
}

void PContext::setText( const PContextText &t )
{
    if ( text == t ) return;
    text = t;
    emit signalModified( text );
}

void PContext::setPolygonFilled( const PContextPolygonFilled &t )
{
    if ( polygonfilled == t ) return;
    polygonfilled = t;
    emit signalModified( polygonfilled );
}

void PContext::setPaste( const PContextPaste &t )
{
    if ( paste == t ) return;
    paste = t;
    emit signalModified( paste );
}

void PContext::setGeneral( const PContextGeneral &t )
{
    if ( general == t ) return;
    general = t;
    emit signalModified( general );
}

void PContext::doSave()
{
    // create
    QFile file( stringFileName );
    if ( !file.open( QIODevice::WriteOnly ) )
    {
        QMessageBox::information( qApp->activeWindow(), tr("Save Default Context"), tr("Could not open ") + file.fileName() );
        return;
    }

    // create an XML document with class name and document version...
    QDomDocument domDoc( PCONTEXT_DOC_CLASS );
    QDomElement domElementRoot = domDoc.createElement( PCONTEXT_DOC_CLASS );
    domElementRoot.setAttribute( "DocVer", LIB_DOC_VER );

    domDoc.appendChild( domElementRoot );

    domElementRoot.appendChild( WPersistNative::doSavePen( pen, &domDoc, "Pen" ) );
    domElementRoot.appendChild( WPersistNative::doSaveBrush( brush, &domDoc, "Brush" ) );
    domElementRoot.appendChild( WPersistNative::doSaveFont( font, &domDoc, "Font" ) );

    QDomElement domElem;

    domElem = domDoc.createElement( "General" );
    general.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "FreeHand" );
    freehand.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "Spray" );
    spray.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "Erase" );
    erase.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "Text" );
    text.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "PolygonFilled" );
    polygonfilled.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    domElem = domDoc.createElement( "Paste" );
    paste.doSave( &domDoc, &domElem );
    domElementRoot.appendChild( domElem );

    // write
    QString stringXML = domDoc.toString();
    file.write( stringXML.toLatin1(), stringXML.length() );

    // close
    file.close();
}

void PContext::doLoad()
{
    // open a file...
    QFile file( stringFileName );
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        // assume we could not load because its the first time running and config file not yet saved
        doSave();
        // QMessageBox::information( qApp->activeWindow(), tr("Open Default Context"), tr("Could not open file.\n\n") + stringFileName, QMessageBox::Ok );
        return;
    }

    // read file into XML document...
    QDomDocument domDoc( PCONTEXT_DOC_CLASS );
    {
        QString stringErrorMsg;
        int nErrorLine;
        int nErrorColumn;
        if ( !domDoc.setContent( &file, false, &stringErrorMsg, &nErrorLine, &nErrorColumn ) )
        {
            QMessageBox::information( qApp->activeWindow(), tr("Open Default Context"), tr("Invalid file format.\nCould not create an XML document from file.\n\n") + file.fileName() );
            file.close();
            return;
        }
    }
    file.close();

    // validate XML...
    // Root element is only used to check that class in XML matches this class.
    QDomElement domElemRoot = domDoc.documentElement();
    if ( domElemRoot.isNull() )
    {
        QMessageBox::information( qApp->activeWindow(), tr("Open Default Context"), tr("Invalid file format.\nCould not find a document element.\n\n") + stringFileName );
        return;
    }
    if ( domElemRoot.tagName() != PCONTEXT_DOC_CLASS )
    {
        QMessageBox::information( qApp->activeWindow(), tr("Open Default Context"), tr("Invalid file format.\nFile does not appear to be a ") + PCONTEXT_DOC_CLASS + "\n\n" + stringFileName );
        return;
    }
    if ( domElemRoot.attribute( "DocVer" ) != LIB_DOC_VER ) 
    {
        QMessageBox::information( qApp->activeWindow(), tr("Open Default Context"), tr("Invalid file format.\nFile is not Version ") + LIB_DOC_VER );
        return;
    }

    // Its our data so lets start processing starting at first child...
    QDomNode domNode = domElemRoot.firstChild();

    while ( !domNode.isNull() )
    {
        if ( domNode.nodeType() != QDomNode::ElementNode )
        {
            domNode = domNode.nextSibling();
            continue;
        }

        QDomElement domElem = domNode.toElement();
        if ( domElem.isNull() )
        {
            domNode = domNode.nextSibling();
            continue;
        }

        if ( domElem.tagName() == "General" )
        { 
            general.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "Pen" )
        { 
            pen = WPersistNative::doLoadPen( &domElem );
        }
        else if ( domElem.tagName() == "Brush" )
        { 
            brush = WPersistNative::doLoadBrush( &domElem );
        }
        else if ( domElem.tagName() == "Font" )
        { 
            font = WPersistNative::doLoadFont( &domElem );
        }
        else if ( domElem.tagName() == "FreeHand" )
        { 
            freehand.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "Spray" )
        { 
            spray.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "Erase" )
        { 
            erase.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "Text" )
        { 
            text.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "PolygonFilled" )
        { 
            polygonfilled.doLoad( &domElem );
        }
        else if ( domElem.tagName() == "Paste" )
        { 
            paste.doLoad( &domElem );
        }

        domNode = domNode.nextSibling();
    }
    return;
}

void PContext::slotImage( QImage *p )
{
    setImage( p );
}

void PContext::slotPen( const QPen &t )
{
    setPen( t );
}

void PContext::slotBrush( const QBrush &t )
{
    setBrush( t );
}

void PContext::slotFont( const QFont &t )
{
    setFont( t );
}

void PContext::slotGradient( const PContextGradient &t )
{
    setGradient( t );
}

void PContext::slotGradientLinear( const PContextGradientLinear &t )
{
    setGradientLinear( t );
}

void PContext::slotGradientRadial( const PContextGradientRadial &t )
{
    setGradientRadial( t );
}

void PContext::slotGradientConical( const PContextGradientConical &t )
{
    setGradientConical( t );
}

void PContext::slotFreeHand( const PContextFreeHand &t )
{
    setFreeHand( t );
}

void PContext::slotErase( const PContextErase &t )
{
    setErase( t );
}

void PContext::slotSpray( const PContextSpray &t )
{
    setSpray( t );
}

void PContext::slotText( const PContextText &t )
{
    setText( t );
}

void PContext::slotPolygonFilled( const PContextPolygonFilled &t )
{
    setPolygonFilled( t );
}

void PContext::slotPaste( const PContextPaste &t )
{
    setPaste( t );
}

void PContext::slotGeneral( const PContextGeneral &t )
{
    setGeneral( t );
}



