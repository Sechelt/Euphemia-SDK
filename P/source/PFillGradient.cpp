#include "LibInfo.h"
#include "PFillGradient.h"

#include "PContext.h"
#include "PCanvas.h"

#define PFillGradientSeed 0

#define PFillGradientLinearStart 1
#define PFillGradientLinearStop 2

#define PFillGradientRadialRadius 1
#define PFillGradientRadialFocal 2

#define PFillGradientConicalAngle 1

//
//
//
PFillGradient::PFillGradient( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
    connect( g_Context, SIGNAL(signalModified(const PContextGradient &)), SLOT(slotRefresh(const PContextGradient &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientLinear &)), SLOT(slotRefresh(const PContextGradientLinear &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientRadial &)), SLOT(slotRefresh(const PContextGradientRadial &)) );
    connect( g_Context, SIGNAL(signalModified(const PContextGradientConical &)), SLOT(slotRefresh(const PContextGradientConical &)) );
}

PFillGradient::~PFillGradient()
{
    doCancel();
}

/*!
 * \brief Return a copy of the CURRENT SHAPE.
 *  
 * This is done by getting doPaint to draw on an empty, temp, canvas and then 
 * copying from that canvas. 
 *  
 * \author pharvey (2/6/23)
 * 
 * \return QImage 
 */
QImage PFillGradient::getCopy()
{
    QImage image( g_Context->getImage()->size(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    doPaint( &painter );

    return image.copy( polygon.boundingRect() );
}

/*!
 * \brief Switch from drawing mode to manipulating mode.
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return QRect 
 */
void PFillGradient::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );
}

/*!
 * \brief Create a new point (drawing mode) or start to move a point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
void PFillGradient::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

void PFillGradient::doMove( PMouseEvent *pEvent ) 
{
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PFillGradient::doRelease( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        break;
    case StateManipulate:
        pHandle = nullptr;
        break;
    }
}

void PFillGradient::doCommit()
{
    Q_ASSERT( nState == StateManipulate );

    QPainter painter( g_Context->getImage() );
    doPaint( &painter );

    emit signalCommitted();
    doIdleState();
}

void PFillGradient::slotRefresh( const PContextGradient &t )
{
    if ( nState != StateManipulate ) return;

    if ( t.nType != nType )
    {
        doCancel();
        return;
    }
}

void PFillGradient::slotRefresh( const PContextGradientLinear &t )
{
}

void PFillGradient::slotRefresh( const PContextGradientRadial &t )
{
}

void PFillGradient::slotRefresh( const PContextGradientConical &t )
{
}


void PFillGradient::doPaint( QPainter *pPainter )
{
    if ( nState != StateManipulate ) return;

    if ( pGradientPreset )
    {
        pGradientPreset->setSpread( g_Context->getGradient().nSpread );
        pPainter->setBrush( QBrush( *pGradientPreset ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
    }
    else if ( pGradientLinear )
    {
        pGradientLinear->setSpread( g_Context->getGradient().nSpread );
        pGradientLinear->setStart( linear.pointStart );
        pGradientLinear->setFinalStop( linear.pointStop );
//        gradient.setColorAt( 0, Qt::black );                                         
//        gradient.setColorAt( 0.5, Qt::yellow );                                      
//        gradient.setColorAt( 0.75, Qt::blue );                                       
//        gradient.setColorAt( 1, Qt::white );                                         
//        gradient.setSpread( QGradient::ReflectSpread ); // for linear and radial only
        pPainter->setBrush( QBrush( *pGradientLinear ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
    }
    else if ( pGradientRadial )
    {
        pGradientRadial->setSpread( g_Context->getGradient().nSpread );
        pGradientRadial->setRadius( radial.nRadius );
        pGradientRadial->setFocalPoint( radial.pointFocalPoint );
        pPainter->setBrush( QBrush( *pGradientRadial ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
    }
    else if ( pGradientConical )
    {
        pGradientConical->setSpread( g_Context->getGradient().nSpread );
        // 0=E
        // 45=NE
        // 90=N
        // 180=W
        // 270=S
        // 360=E
        pGradientConical->setAngle( conical.nStartAngle );
        pPainter->setBrush( QBrush( *pGradientConical ) );
        pPainter->setPen( QPen( Qt::NoPen ) );
        pPainter->drawPolygon( polygon );
    }
}

void PFillGradient::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    nType = g_Context->getGradient().nType;
    if ( nType >= 0 )
    {
        pGradientPreset = new QGradient( (QGradient::Preset)nType );
    }
    else if ( nType == PContextGradient::StandardGradientLinear )
    {
        linear.pointStart = linear.pointStop = point;
        pGradientLinear = new QLinearGradient( linear.pointStart, linear.pointStop );
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        radial.nRadius = 10;
        radial.pointFocalPoint = point;
        pGradientRadial = new QRadialGradient( point, radial.nRadius, radial.pointFocalPoint );
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        conical.nStartAngle = 10;
        pGradientConical = new QConicalGradient( point, conical.nStartAngle );
        // pGradientConical->setCoordinateMode( QGradient::StretchToDeviceMode );
    }
    else
    {
        Q_ASSERT( 1==0 );
    }

    pointSeed = point;
    polygon = getPolygon( pointSeed );
    nState = StateDraw;
    doManipulateState();
}

void PFillGradient::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PFillGradient::doIdleState()
{
    Q_ASSERT( nState == StateManipulate );

    doDeleteHandles();
    if ( pGradientPreset ) 
    {
        delete pGradientPreset;
        pGradientPreset = nullptr;
    }
    if ( pGradientLinear ) 
    {
        delete pGradientLinear;
        pGradientLinear = nullptr;
    }
    if ( pGradientRadial ) 
    {
        delete pGradientRadial;
        pGradientRadial = nullptr;
    }
    if ( pGradientConical ) 
    {
        delete pGradientConical;
        pGradientConical = nullptr;
    }
    nType = 0;
    polygon.clear();
    nState = StateIdle;
    update();
    emit signalChangedState();
}


/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 */
void PFillGradient::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // seed handle is always vectorHandles[0]
    pHandle = new PHandle( pView, PHandle::TypeFillSeed, pView->mapFromScene( pointSeed ) );
    vectorHandles.append( pHandle );
    pHandle->show();

    if ( pGradientLinear )
    {
        // start handle is always vectorHandles[1]
        pHandle = new PHandle( pView, PHandle::TypeFillStart, pView->mapFromScene( linear.pointStart ) );
        vectorHandles.append( pHandle );
        pHandle->show();

        // stop handle is always vectorHandles[2]
        pHandle = new PHandle( pView, PHandle::TypeFillStop, pView->mapFromScene( linear.pointStop ) );
        vectorHandles.append( pHandle );
        pHandle->show();
    }
    else if ( pGradientRadial )
    {
        QPoint point( pointSeed.x() + radial.nRadius, pointSeed.y() );
        pHandle = new PHandle( pView, PHandle::TypeFillStart, pView->mapFromScene( point ) );
        vectorHandles.append( pHandle );
        pHandle->show();

        pHandle = new PHandle( pView, PHandle::TypeFillStop, pView->mapFromScene( radial.pointFocalPoint ) );
        vectorHandles.append( pHandle );
        pHandle->show();
    }
    else if ( pGradientConical )
    {
        QPointF pointCartesian = getPolarToCartesian( conical.nRadius, conical.nStartAngle );
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << pointCartesian;
        pHandle = new PHandle( pView, PHandle::TypeFillStart, pView->mapFromScene( pointSeed + pointCartesian.toPoint() ) );
        vectorHandles.append( pHandle );
        pHandle->show();
    }
}


// react to zoom
void PFillGradient::doSyncHandles()
{
    vectorHandles[PFillGradientSeed]->setCenter( pView->mapFromScene( pointSeed ) );

    if ( pGradientLinear )
    {
        vectorHandles[PFillGradientLinearStart]->setCenter( pView->mapFromScene( linear.pointStart ) );
        vectorHandles[PFillGradientLinearStop]->setCenter( pView->mapFromScene( linear.pointStop ) );
    }
    else if ( pGradientRadial )
    {
        QPoint point( pointSeed.x() + radial.nRadius, pointSeed.y() );
        vectorHandles[PFillGradientRadialRadius]->setCenter( pView->mapFromScene( point ) );
        vectorHandles[PFillGradientRadialFocal]->setCenter( pView->mapFromScene( radial.pointFocalPoint ) );
    }
    else if ( pGradientConical )
    {
        QPointF point = getPolarToCartesian( conical.nRadius, conical.nStartAngle );
        vectorHandles[PFillGradientConicalAngle]->setCenter( pView->mapFromScene( point ) );
    }
}

void PFillGradient::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    QPoint pointViewPos = pView->mapFromScene( pointPos );

    if ( pGradientLinear )
    {
        if ( pHandle == vectorHandles[PFillGradientLinearStart] )
        {
            pHandle->setCenter( pointViewPos );
            linear.pointStart = pointPos;
            update();
        }
        else if ( pHandle == vectorHandles[PFillGradientLinearStop] )
        {
            pHandle->setCenter( pointViewPos );
            linear.pointStop = pointPos;
            update();
        }
    }
    else if ( pGradientRadial )
    {
        if ( pHandle == vectorHandles[PFillGradientRadialRadius] )
        {
            pHandle->setCenter( pointViewPos );
            radial.nRadius = qAbs( pointSeed.x() - pointPos.x() );
            update();
        }
        else if ( pHandle == vectorHandles[PFillGradientRadialFocal] )
        {
            pHandle->setCenter( pointViewPos );
            radial.pointFocalPoint = pointPos;
            update();
        }
    }
    else if ( pGradientConical )
    {
        if ( pHandle == vectorHandles[PFillGradientConicalAngle] )
        {
            pHandle->setCenter( pointViewPos );
            QPoint pointCartesian = pointPos - pointSeed; // pointPos has origin at topLeft but we need it to be relative to the seed
qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "] pointSeed:" << pointSeed << " pointPos:" << pointPos << " pointCartesian:" << pointCartesian;

            Polar polar = getCartesianToPolar( pointCartesian.x(), pointCartesian.y() );
            conical.nRadius = polar.radius;
            conical.nStartAngle = polar.angle;
// qInfo() << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]" << conical.nRadius << conical.nStartAngle;
            update();
        }
    }
}

/*!
 * \brief Returns a polygon representing a boundary. 
 *  
 * The boundary is defined by not being the color at pointSeed starting at pointSeed. 
 *  
 * Uses the 'Square tracing algorithm'. 
 *  
 * \author pharvey (2/17/23)
 * 
 * \param point  
 * 
 * \return QPolygon The inside of the polygon is the area outlined.
 */
QPolygon PFillGradient::getPolygon( const QPoint &pointSeed )
{
    QImage *    pImage      = g_Context->getImage();
    int         nX          = pointSeed.x();
    int         nY          = pointSeed.y();
    QColor      colorSeed   = pImage->pixelColor( pointSeed );
    QPolygon    polygon;

    // go west until we hit a boundary (or go off image)
    do
    {
        nX--;
        if ( isBoundary( pImage, colorSeed, QPoint( nX, nY ) ) ) break; 
    } while ( 1 );

    // start point
    QPoint pointStart( nX, nY );
    polygon.append( pointStart );

    //
    QPoint pointStep = getLeft( QPoint( 0, 1 ) );               
    QPoint point = pointStart + pointStep;
    while ( point != pointStart )
    {
        if ( isBoundary( pImage, colorSeed, point ) )
        {
            polygon.append( point );
            pointStep = getLeft( pointStep );
            point = point + pointStep;
        }
        else
        {
            point = point - pointStep;
            pointStep = getRight( pointStep );
            point = point + pointStep;
        }
    }

    return polygon;
}

QPointF PFillGradient::getPolarToCartesian( qreal nRadius, qreal nAngle )
{
    return QPointF( nRadius * qCos( nAngle ), nRadius * qSin( nAngle ) );
}

PFillGradient::Polar PFillGradient::getCartesianToPolar( qreal x, qreal y )
{
    Polar p;

    // θ = tan-1 ( y / x )
    p.angle = qAtan2( y, x );
    if ( p.angle < 0 )
    {
        p.angle += (2* M_PI);
    }

    // r = √ ( x2 + y2 )
    p.radius = qSqrt( (x*x) + (y*y) );

    return p;
}


//
// PFillGradientToolBar
//
PFillGradientToolBar::PFillGradientToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pType = new QComboBox( this );
    doAddType( "Linear",           PContextGradient::StandardGradientLinear );
    doAddType( "Radial",           PContextGradient::StandardGradientRadial );
    doAddType( "Conical",          PContextGradient::StandardGradientConical );
    doAddType( "WarmFlame",        QGradient::WarmFlame );                          
    doAddType( "NightFade",        QGradient::NightFade );                          
    doAddType( "SpringWarmth",     QGradient::SpringWarmth );                       
    doAddType( "JuicyPeach",       QGradient::JuicyPeach );                         
    doAddType( "YoungPassion",     QGradient::YoungPassion );                       
    doAddType( "LadyLips",         QGradient::LadyLips );                           
    doAddType( "SunnyMorning",     QGradient::SunnyMorning );                       
    doAddType( "RainyAshville",    QGradient::RainyAshville );                      
    doAddType( "FrozenDreams",     QGradient::FrozenDreams );                       
    doAddType( "WinterNeva",       QGradient::WinterNeva );                         
    doAddType( "DustyGrass",       QGradient::DustyGrass );                         
    doAddType( "TemptingAzure",    QGradient::TemptingAzure );                      
    doAddType( "HeavyRain",        QGradient::HeavyRain );                          
    doAddType( "AmyCrisp",         QGradient::AmyCrisp );                           
    doAddType( "MeanFruit",        QGradient::MeanFruit );                          
    doAddType( "DeepBlue",         QGradient::DeepBlue );                           
    doAddType( "RipeMalinka",      QGradient::RipeMalinka );                        
    doAddType( "CloudyKnoxville",  QGradient::CloudyKnoxville );                    
    doAddType( "MalibuBeach",      QGradient::MalibuBeach );                        
    doAddType( "NewLife",          QGradient::NewLife );                            
    doAddType( "TrueSunset",       QGradient::TrueSunset );                         
    doAddType( "MorpheusDen",      QGradient::MorpheusDen );                        
    doAddType( "RareWind",         QGradient::RareWind );                           
    doAddType( "NearMoon",         QGradient::NearMoon );                           
    doAddType( "WildApple",        QGradient::WildApple );                          
    doAddType( "SaintPetersburg",  QGradient::SaintPetersburg );                    
    doAddType( "PlumPlate",        QGradient::PlumPlate );                          
    doAddType( "EverlastingSky",   QGradient::EverlastingSky );                     
    doAddType( "HappyFisher",      QGradient::HappyFisher );                        
    doAddType( "Blessing",         QGradient::Blessing );                           
    doAddType( "SharpeyeEagle",    QGradient::SharpeyeEagle );                      
    doAddType( "LadogaBottom",     QGradient::LadogaBottom );                       
    doAddType( "LemonGate",        QGradient::LemonGate );                          
    doAddType( "ItmeoBranding",    QGradient::ItmeoBranding );                      
    doAddType( "ZeusMiracle",      QGradient::ZeusMiracle );                        
    doAddType( "OldHat",           QGradient::OldHat );                             
    doAddType( "StarWine",         QGradient::StarWine );                           
    doAddType( "HappyAcid",        QGradient::HappyAcid );                          
    doAddType( "AwesomePine",      QGradient::AwesomePine );                        
    doAddType( "NewYork",          QGradient::NewYork );                            
    doAddType( "ShyRainbow",       QGradient::ShyRainbow );                         
    doAddType( "MixedHopes",       QGradient::MixedHopes );                         
    doAddType( "FlyHigh",          QGradient::FlyHigh );                            
    doAddType( "StrongBliss",      QGradient::StrongBliss );                        
    doAddType( "FreshMilk",        QGradient::FreshMilk );                          
    doAddType( "SnowAgain",        QGradient::SnowAgain );                          
    doAddType( "FebruaryInk",      QGradient::FebruaryInk );                        
    doAddType( "KindSteel",        QGradient::KindSteel );                          
    doAddType( "SoftGrass",        QGradient::SoftGrass );                          
    doAddType( "GrownEarly",       QGradient::GrownEarly );                         
    doAddType( "SharpBlues",       QGradient::SharpBlues );                         
    doAddType( "ShadyWater",       QGradient::ShadyWater );                         
    doAddType( "DirtyBeauty",      QGradient::DirtyBeauty );                        
    doAddType( "GreatWhale",       QGradient::GreatWhale );                         
    doAddType( "TeenNotebook",     QGradient::TeenNotebook );                       
    doAddType( "PoliteRumors",     QGradient::PoliteRumors );                       
    doAddType( "SweetPeriod",      QGradient::SweetPeriod );                        
    doAddType( "WideMatrix",       QGradient::WideMatrix );                         
    doAddType( "SoftCherish",      QGradient::SoftCherish );                        
    doAddType( "RedSalvation",     QGradient::RedSalvation );                       
    doAddType( "BurningSpring",    QGradient::BurningSpring );                      
    doAddType( "NightParty",       QGradient::NightParty );                         
    doAddType( "SkyGlider",        QGradient::SkyGlider );                          
    doAddType( "HeavenPeach",      QGradient::HeavenPeach );                        
    doAddType( "PurpleDivision",   QGradient::PurpleDivision );                     
    doAddType( "AquaSplash",       QGradient::AquaSplash );                         
    doAddType( "SpikyNaga",        QGradient::SpikyNaga );                          
    doAddType( "LoveKiss",         QGradient::LoveKiss );                           
    doAddType( "CleanMirror",      QGradient::CleanMirror );                        
    doAddType( "PremiumDark",      QGradient::PremiumDark );                        
    doAddType( "ColdEvening",      QGradient::ColdEvening );                        
    doAddType( "CochitiLake",      QGradient::CochitiLake );                        
    doAddType( "SummerGames",      QGradient::SummerGames );                        
    doAddType( "PassionateBed",    QGradient::PassionateBed );                      
    doAddType( "MountainRock",     QGradient::MountainRock );                       
    doAddType( "DesertHump",       QGradient::DesertHump );                         
    doAddType( "JungleDay",        QGradient::JungleDay );                          
    doAddType( "PhoenixStart",     QGradient::PhoenixStart );                       
    doAddType( "OctoberSilence",   QGradient::OctoberSilence );                     
    doAddType( "FarawayRiver",     QGradient::FarawayRiver );                       
    doAddType( "AlchemistLab",     QGradient::AlchemistLab );                       
    doAddType( "OverSun",          QGradient::OverSun );                            
    doAddType( "PremiumWhite",     QGradient::PremiumWhite );                       
    doAddType( "MarsParty",        QGradient::MarsParty );                          
    doAddType( "EternalConstance", QGradient::EternalConstance );                   
    doAddType( "JapanBlush",       QGradient::JapanBlush );                         
    doAddType( "SmilingRain",      QGradient::SmilingRain );                        
    doAddType( "CloudyApple",      QGradient::CloudyApple );                        
    doAddType( "BigMango",         QGradient::BigMango );                           
    doAddType( "HealthyWater",     QGradient::HealthyWater );                       
    doAddType( "AmourAmour",       QGradient::AmourAmour );                         
    doAddType( "RiskyConcrete",    QGradient::RiskyConcrete );                      
    doAddType( "StrongStick",      QGradient::StrongStick );                        
    doAddType( "ViciousStance",    QGradient::ViciousStance );                      
    doAddType( "PaloAlto",         QGradient::PaloAlto );                           
    doAddType( "HappyMemories",    QGradient::HappyMemories );                      
    doAddType( "MidnightBloom",    QGradient::MidnightBloom );                      
    doAddType( "Crystalline",      QGradient::Crystalline );                        
    doAddType( "PartyBliss",       QGradient::PartyBliss );                         
    doAddType( "ConfidentCloud",   QGradient::ConfidentCloud );                     
    doAddType( "LeCocktail",       QGradient::LeCocktail );                         
    doAddType( "RiverCity",        QGradient::RiverCity );                          
    doAddType( "FrozenBerry",      QGradient::FrozenBerry );                        
    doAddType( "ChildCare",        QGradient::ChildCare );                          
    doAddType( "FlyingLemon",      QGradient::FlyingLemon );                        
    doAddType( "NewRetrowave",     QGradient::NewRetrowave );                       
    doAddType( "HiddenJaguar",     QGradient::HiddenJaguar );                       
    doAddType( "AboveTheSky",      QGradient::AboveTheSky );                        
    doAddType( "Nega",             QGradient::Nega );                               
    doAddType( "DenseWater",       QGradient::DenseWater );                         
    doAddType( "Seashore",         QGradient::Seashore );                           
    doAddType( "MarbleWall",       QGradient::MarbleWall );                         
    doAddType( "CheerfulCaramel",  QGradient::CheerfulCaramel );                    
    doAddType( "NightSky",         QGradient::NightSky );                           
    doAddType( "MagicLake",        QGradient::MagicLake );                          
    doAddType( "YoungGrass",       QGradient::YoungGrass );                         
    doAddType( "ColorfulPeach",    QGradient::ColorfulPeach );                      
    doAddType( "GentleCare",       QGradient::GentleCare );                         
    doAddType( "PlumBath",         QGradient::PlumBath );                           
    doAddType( "HappyUnicorn",     QGradient::HappyUnicorn );                       
    doAddType( "AfricanField",     QGradient::AfricanField );                       
    doAddType( "SolidStone",       QGradient::SolidStone );                         
    doAddType( "OrangeJuice",      QGradient::OrangeJuice );                        
    doAddType( "GlassWater",       QGradient::GlassWater );                         
    doAddType( "NorthMiracle",     QGradient::NorthMiracle );                       
    doAddType( "FruitBlend",       QGradient::FruitBlend );                         
    doAddType( "MillenniumPine",   QGradient::MillenniumPine );                     
    doAddType( "HighFlight",       QGradient::HighFlight );                         
    doAddType( "MoleHall",         QGradient::MoleHall );                           
    doAddType( "SpaceShift",       QGradient::SpaceShift );                         
    doAddType( "ForestInei",       QGradient::ForestInei );                         
    doAddType( "RoyalGarden",      QGradient::RoyalGarden );                        
    doAddType( "RichMetal",        QGradient::RichMetal );                          
    doAddType( "JuicyCake",        QGradient::JuicyCake );                          
    doAddType( "SmartIndigo",      QGradient::SmartIndigo );                        
    doAddType( "SandStrike",       QGradient::SandStrike );                         
    doAddType( "NorseBeauty",      QGradient::NorseBeauty );                        
    doAddType( "AquaGuidance",     QGradient::AquaGuidance );                       
    doAddType( "SunVeggie",        QGradient::SunVeggie );                          
    doAddType( "SeaLord",          QGradient::SeaLord );                            
    doAddType( "BlackSea",         QGradient::BlackSea );                           
    doAddType( "GrassShampoo",     QGradient::GrassShampoo );                       
    doAddType( "LandingAircraft",  QGradient::LandingAircraft );                    
    doAddType( "WitchDance",       QGradient::WitchDance );                         
    doAddType( "SleeplessNight",   QGradient::SleeplessNight );                     
    doAddType( "AngelCare",        QGradient::AngelCare );                          
    doAddType( "CrystalRiver",     QGradient::CrystalRiver );                       
    doAddType( "SoftLipstick",     QGradient::SoftLipstick );                       
    doAddType( "SaltMountain",     QGradient::SaltMountain );                       
    doAddType( "PerfectWhite",     QGradient::PerfectWhite );                       
    doAddType( "FreshOasis",       QGradient::FreshOasis );                         
    doAddType( "StrictNovember",   QGradient::StrictNovember );                     
    doAddType( "MorningSalad",     QGradient::MorningSalad );                       
    doAddType( "DeepRelief",       QGradient::DeepRelief );                         
    doAddType( "SeaStrike",        QGradient::SeaStrike );                          
    doAddType( "NightCall",        QGradient::NightCall );                          
    doAddType( "SupremeSky",       QGradient::SupremeSky );                         
    doAddType( "LightBlue",        QGradient::LightBlue );                          
    doAddType( "MindCrawl",        QGradient::MindCrawl );                          
    doAddType( "LilyMeadow",       QGradient::LilyMeadow );                         
    doAddType( "SugarLollipop",    QGradient::SugarLollipop );                      
    doAddType( "SweetDessert",     QGradient::SweetDessert );                       
    doAddType( "MagicRay",         QGradient::MagicRay );                           
    doAddType( "TeenParty",        QGradient::TeenParty );                          
    doAddType( "FrozenHeat",       QGradient::FrozenHeat );                         
    doAddType( "GagarinView",      QGradient::GagarinView );                        
    doAddType( "FabledSunset",     QGradient::FabledSunset );                       
    doAddType( "PerfectBlue",      QGradient::PerfectBlue );                        
    pType->setCurrentIndex( pType->findData( g_Context->getGradient().nType ) );
    pLayout->addWidget( pType );
    connect( pType, SIGNAL(activated(int)), SLOT(slotType(int)) );

    pSpread = new QComboBox( this );
    pSpread->addItem( "PadSpread", QGradient::PadSpread ); 
    pSpread->addItem( "RepeatSpread", QGradient::RepeatSpread ); 
    pSpread->addItem( "ReflectSpread", QGradient::ReflectSpread ); 
    pSpread->setCurrentIndex( pSpread->findData( g_Context->getGradient().nSpread ) );
    pLayout->addWidget( pSpread );
    connect( pSpread, SIGNAL(activated(int)), SLOT(slotSpread(int)) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(const PContextGradient &)), SLOT(slotRefresh(const PContextGradient &)) );
}

void PFillGradientToolBar::slotRefresh( const PContextGradient &t )
{
    pType->setCurrentIndex( pType->findData( t.nType ) );
}

void PFillGradientToolBar::slotType( int n )
{
    PContextGradient t = g_Context->getGradient();
    t.nType = pType->itemData( n ).toInt();
    g_Context->setGradient( t );
}

void PFillGradientToolBar::slotSpread( int n )
{
    PContextGradient t = g_Context->getGradient();
    t.nSpread = QGradient::Spread(pSpread->itemData( n ).toInt());
    g_Context->setGradient( t );
}

QPixmap PFillGradientToolBar::getSwatch( const QSize size, int nType )
{
    QPixmap pixmap( size );
    QRect r( 0, 0, size.width(), size.height() );
    pixmap.fill( Qt::white );
    QPainter painter( &pixmap );

    if ( nType >= 0 )
    {
        QGradient gradient( (QGradient::Preset)nType );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientLinear )
    {
        QLinearGradient gradient( QPoint( r.left(), r.center().y() ), QPoint( r.right(),  r.center().y() )  );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientRadial )
    {
        QRadialGradient gradient( r.center(), r.width() / 2 );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }
    else if ( nType == PContextGradient::StandardGradientConical )
    {
        QConicalGradient gradient( r.center(), 0.0 );
        painter.setBrush( gradient );
        painter.setPen( Qt::NoPen );
        painter.drawRoundedRect( r, 2, 2 );
    }

    return pixmap;
}

void PFillGradientToolBar::doAddType( const QString &stringText, int nType )
{
    pType->addItem( QIcon( getSwatch( QSize( 48, 48 ), nType ) ), stringText, nType );
}


