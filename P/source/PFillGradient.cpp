#include "LibInfo.h"
#include "PFillGradient.h"

#include "PContext.h"
#include "PCanvas.h"

PFillGradientLinear::PFillGradientLinear( PCanvas *pCanvas )
    : PShapeBase( pCanvas )
{
}

PFillGradientLinear::~PFillGradientLinear()
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
QImage PFillGradientLinear::getCopy()
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
void PFillGradientLinear::doDoubleClick( PMouseEvent *pEvent )
{
    Q_UNUSED( pEvent );

    if ( nState != StateDraw ) return;

    doManipulateState();

    if ( pCanvas->getAutoCommit() ) doCommit();
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
void PFillGradientLinear::doPress( PMouseEvent *pEvent )
{
    if ( pEvent->button() != Qt::LeftButton ) return;

    switch ( nState )
    {
    case StateIdle:
        doDrawState( pEvent->pos() );
        polygon.boundingRect();
        break;
    case StateDraw:
        polygon.append( pEvent->pos() );
        polygon.boundingRect();
        update();
        break;
    case StateManipulate:
        pHandle = getHandle( pEvent->pos() );
        if ( !pHandle ) doCommit();
        break;
    }
}

/*!
 * \brief Move the last point (drawing mode) or the handle/point (manipulating mode).
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pEvent 
 * 
 * \return bool 
 */
void PFillGradientLinear::doMove( PMouseEvent *pEvent ) 
{
    switch ( nState )
    {
    case StateIdle:
        break;
    case StateDraw:
        polygon.replace( polygon.count() - 1, pEvent->pos() );
        update();
        break;
    case StateManipulate:
        if ( pHandle ) doMoveHandle( pEvent->pos() );
        break;
    }
}

void PFillGradientLinear::doRelease( PMouseEvent *pEvent )
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
        break;
    }
}

/*!
 * \brief Commit the polygon to canvas. 
 *  
 * 
 * \author pharvey (2/1/23)
 */
void PFillGradientLinear::doCommit()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    QPainter painter( g_Context->getImage());
    doPaint( &painter );
    emit signalCommitted();
    doIdleState();
}

void PFillGradientLinear::doPaint( QPainter *pPainter )
{
    // apply context
    pPainter->setPen( g_Context->getPen() );

    if ( nState == StateManipulate )
    {
        QPointF pointStart = polygon.at( 0 );
        QPointF pointStop = polygon.at( 1 );

        QLinearGradient linearGrad( pointStart, pointStop );
        // linearGrad.setColorAt(0, Qt::black);
        // linearGrad.setColorAt(1, Qt::white);

    }
    else
    {
        pPainter->drawPolyline( polygon );                         
    }
}

void PFillGradientLinear::doDrawState( const QPoint &point )
{
    Q_ASSERT( nState == StateIdle );

    polygon.append( point );    // begin
    polygon.append( point );    // current

    nState = StateDraw;
    update();
    emit signalChangedState();
}

void PFillGradientLinear::doManipulateState()
{
    Q_ASSERT( nState == StateDraw );
    doCreateHandles();
    nState = StateManipulate;
    emit signalChangedState();
}

void PFillGradientLinear::doIdleState()
{
    Q_ASSERT( nState == StateDraw || nState == StateManipulate );

    if ( nState == StateDraw )
    {
        nState = StateIdle;
    }
    else if ( nState == StateManipulate )
    {
        doDeleteHandles();
        polygon.clear();
        nState = StateIdle;
    }
    update();
    emit signalChangedState();
}


/*!
 * \brief Create handles.
 *  
 * Order matters when handles share a position. Last handle will be found first. 
 *  
 * It would be easiest to append the move handle at the end as this would mean there would 
 * be a direct correspondence between point indexs and handle indexs - but this would make 
 * the move handle the default when all handles happened to be stacked upon each other. 
 * This would make resizing a very small polygon difficult - so we have the move handle 
 * first (at bottom). 
 *  
 * \author pharvey (2/1/23)
 */
void PFillGradientLinear::doCreateHandles()
{
    Q_ASSERT( vectorHandles.count() == 0 );

    // their parent will be the viewport so...
    QPolygon polygonView = pView->mapFromScene( polygon );

    // Order matters when handles share a position. Last handle will be found first.
    PHandle *pHandle;

    // move handle is always vectorHandles[0]
    pHandle = new PHandle( pView, PHandle::TypeDrag, polygonView.boundingRect().center() );
    vectorHandles.append( pHandle );
    pHandle->show();

    // add a handle for each point
    for ( QPoint point : polygonView )
    {
        pHandle = new PHandle( pView, PHandle::TypeMovePoint, point );
        vectorHandles.append( pHandle );
        pHandle->show();
    }
}

void PFillGradientLinear::doSyncHandles()
{
    QPolygon polygonView = pView->mapFromScene( polygon );

    vectorHandles[0]->setCenter( polygonView.boundingRect().center() );
    for ( int n = 0; n < polygonView.count(); n++ )
    {
        vectorHandles[n+1]->setCenter( polygonView[n] );
    }
}


/*!
 * \brief Move current handle.
 *  
 * Only used during manipulation mode. 
 *  
 * The polygon will be moved if move handle otherwise we will move handle/point and 
 * adjust move handle so its center. 
 * 
 * \author pharvey (2/1/23)
 * 
 * \param pointPos 
 */
void PFillGradientLinear::doMoveHandle( const QPoint &pointPos )
{
    Q_ASSERT( pHandle );

    // their parent will be the viewport so...
    QPolygon    polygonView     = pView->mapFromScene( polygon );
    QPoint      pointViewPos    = pView->mapFromScene( pointPos );

    // move handle?
    if ( pHandle == vectorHandles[0] )
    {
        QPoint pointDelta = pointPos - pView->mapToScene( pHandle->getCenter() ).toPoint();
        // move points
        for ( int n = 0; n < polygon.count(); n++ )
        {
            polygon.replace( n, polygon.at( n ) + pointDelta );
            vectorHandles.at( n + 1 )->doMoveBy( pointDelta );
        }
        // move self
        vectorHandles.at( 0 )->doMoveBy( pointDelta );
        update();
        return;
    }

    // just a single point handle
    pHandle->setCenter( pointViewPos );
    polygon.replace( vectorHandles.indexOf( pHandle ) - 1, pointPos );
    vectorHandles.at( 0 )->setCenter( polygonView.boundingRect().center() );
    update();
}

PFillGradientToolBar::PFillGradientToolBar( QWidget *p )
    : QWidget( p )
{
    QHBoxLayout *pLayout = new QHBoxLayout( this );
    pPreset = new QComboBox( this );
    pPreset->addItem( "WarmFlame",        QGradient::WarmFlame );                          
    pPreset->addItem( "NightFade",        QGradient::NightFade );                          
    pPreset->addItem( "SpringWarmth",     QGradient::SpringWarmth );                       
    pPreset->addItem( "JuicyPeach",       QGradient::JuicyPeach );                         
    pPreset->addItem( "YoungPassion",     QGradient::YoungPassion );                       
    pPreset->addItem( "LadyLips",         QGradient::LadyLips );                           
    pPreset->addItem( "SunnyMorning",     QGradient::SunnyMorning );                       
    pPreset->addItem( "RainyAshville",    QGradient::RainyAshville );                      
    pPreset->addItem( "FrozenDreams",     QGradient::FrozenDreams );                       
    pPreset->addItem( "WinterNeva",       QGradient::WinterNeva );                         
    pPreset->addItem( "DustyGrass",       QGradient::DustyGrass );                         
    pPreset->addItem( "TemptingAzure",    QGradient::TemptingAzure );                      
    pPreset->addItem( "HeavyRain",        QGradient::HeavyRain );                          
    pPreset->addItem( "AmyCrisp",         QGradient::AmyCrisp );                           
    pPreset->addItem( "MeanFruit",        QGradient::MeanFruit );                          
    pPreset->addItem( "DeepBlue",         QGradient::DeepBlue );                           
    pPreset->addItem( "RipeMalinka",      QGradient::RipeMalinka );                        
    pPreset->addItem( "CloudyKnoxville",  QGradient::CloudyKnoxville );                    
    pPreset->addItem( "MalibuBeach",      QGradient::MalibuBeach );                        
    pPreset->addItem( "NewLife",          QGradient::NewLife );                            
    pPreset->addItem( "TrueSunset",       QGradient::TrueSunset );                         
    pPreset->addItem( "MorpheusDen",      QGradient::MorpheusDen );                        
    pPreset->addItem( "RareWind",         QGradient::RareWind );                           
    pPreset->addItem( "NearMoon",         QGradient::NearMoon );                           
    pPreset->addItem( "WildApple",        QGradient::WildApple );                          
    pPreset->addItem( "SaintPetersburg",  QGradient::SaintPetersburg );                    
    pPreset->addItem( "PlumPlate",        QGradient::PlumPlate );                          
    pPreset->addItem( "EverlastingSky",   QGradient::EverlastingSky );                     
    pPreset->addItem( "HappyFisher",      QGradient::HappyFisher );                        
    pPreset->addItem( "Blessing",         QGradient::Blessing );                           
    pPreset->addItem( "SharpeyeEagle",    QGradient::SharpeyeEagle );                      
    pPreset->addItem( "LadogaBottom",     QGradient::LadogaBottom );                       
    pPreset->addItem( "LemonGate",        QGradient::LemonGate );                          
    pPreset->addItem( "ItmeoBranding",    QGradient::ItmeoBranding );                      
    pPreset->addItem( "ZeusMiracle",      QGradient::ZeusMiracle );                        
    pPreset->addItem( "OldHat",           QGradient::OldHat );                             
    pPreset->addItem( "StarWine",         QGradient::StarWine );                           
    pPreset->addItem( "HappyAcid",        QGradient::HappyAcid );                          
    pPreset->addItem( "AwesomePine",      QGradient::AwesomePine );                        
    pPreset->addItem( "NewYork",          QGradient::NewYork );                            
    pPreset->addItem( "ShyRainbow",       QGradient::ShyRainbow );                         
    pPreset->addItem( "MixedHopes",       QGradient::MixedHopes );                         
    pPreset->addItem( "FlyHigh",          QGradient::FlyHigh );                            
    pPreset->addItem( "StrongBliss",      QGradient::StrongBliss );                        
    pPreset->addItem( "FreshMilk",        QGradient::FreshMilk );                          
    pPreset->addItem( "SnowAgain",        QGradient::SnowAgain );                          
    pPreset->addItem( "FebruaryInk",      QGradient::FebruaryInk );                        
    pPreset->addItem( "KindSteel",        QGradient::KindSteel );                          
    pPreset->addItem( "SoftGrass",        QGradient::SoftGrass );                          
    pPreset->addItem( "GrownEarly",       QGradient::GrownEarly );                         
    pPreset->addItem( "SharpBlues",       QGradient::SharpBlues );                         
    pPreset->addItem( "ShadyWater",       QGradient::ShadyWater );                         
    pPreset->addItem( "DirtyBeauty",      QGradient::DirtyBeauty );                        
    pPreset->addItem( "GreatWhale",       QGradient::GreatWhale );                         
    pPreset->addItem( "TeenNotebook",     QGradient::TeenNotebook );                       
    pPreset->addItem( "PoliteRumors",     QGradient::PoliteRumors );                       
    pPreset->addItem( "SweetPeriod",      QGradient::SweetPeriod );                        
    pPreset->addItem( "WideMatrix",       QGradient::WideMatrix );                         
    pPreset->addItem( "SoftCherish",      QGradient::SoftCherish );                        
    pPreset->addItem( "RedSalvation",     QGradient::RedSalvation );                       
    pPreset->addItem( "BurningSpring",    QGradient::BurningSpring );                      
    pPreset->addItem( "NightParty",       QGradient::NightParty );                         
    pPreset->addItem( "SkyGlider",        QGradient::SkyGlider );                          
    pPreset->addItem( "HeavenPeach",      QGradient::HeavenPeach );                        
    pPreset->addItem( "PurpleDivision",   QGradient::PurpleDivision );                     
    pPreset->addItem( "AquaSplash",       QGradient::AquaSplash );                         
    pPreset->addItem( "SpikyNaga",        QGradient::SpikyNaga );                          
    pPreset->addItem( "LoveKiss",         QGradient::LoveKiss );                           
    pPreset->addItem( "CleanMirror",      QGradient::CleanMirror );                        
    pPreset->addItem( "PremiumDark",      QGradient::PremiumDark );                        
    pPreset->addItem( "ColdEvening",      QGradient::ColdEvening );                        
    pPreset->addItem( "CochitiLake",      QGradient::CochitiLake );                        
    pPreset->addItem( "SummerGames",      QGradient::SummerGames );                        
    pPreset->addItem( "PassionateBed",    QGradient::PassionateBed );                      
    pPreset->addItem( "MountainRock",     QGradient::MountainRock );                       
    pPreset->addItem( "DesertHump",       QGradient::DesertHump );                         
    pPreset->addItem( "JungleDay",        QGradient::JungleDay );                          
    pPreset->addItem( "PhoenixStart",     QGradient::PhoenixStart );                       
    pPreset->addItem( "OctoberSilence",   QGradient::OctoberSilence );                     
    pPreset->addItem( "FarawayRiver",     QGradient::FarawayRiver );                       
    pPreset->addItem( "AlchemistLab",     QGradient::AlchemistLab );                       
    pPreset->addItem( "OverSun",          QGradient::OverSun );                            
    pPreset->addItem( "PremiumWhite",     QGradient::PremiumWhite );                       
    pPreset->addItem( "MarsParty",        QGradient::MarsParty );                          
    pPreset->addItem( "EternalConstance", QGradient::EternalConstance );                   
    pPreset->addItem( "JapanBlush",       QGradient::JapanBlush );                         
    pPreset->addItem( "SmilingRain",      QGradient::SmilingRain );                        
    pPreset->addItem( "CloudyApple",      QGradient::CloudyApple );                        
    pPreset->addItem( "BigMango",         QGradient::BigMango );                           
    pPreset->addItem( "HealthyWater",     QGradient::HealthyWater );                       
    pPreset->addItem( "AmourAmour",       QGradient::AmourAmour );                         
    pPreset->addItem( "RiskyConcrete",    QGradient::RiskyConcrete );                      
    pPreset->addItem( "StrongStick",      QGradient::StrongStick );                        
    pPreset->addItem( "ViciousStance",    QGradient::ViciousStance );                      
    pPreset->addItem( "PaloAlto",         QGradient::PaloAlto );                           
    pPreset->addItem( "HappyMemories",    QGradient::HappyMemories );                      
    pPreset->addItem( "MidnightBloom",    QGradient::MidnightBloom );                      
    pPreset->addItem( "Crystalline",      QGradient::Crystalline );                        
    pPreset->addItem( "PartyBliss",       QGradient::PartyBliss );                         
    pPreset->addItem( "ConfidentCloud",   QGradient::ConfidentCloud );                     
    pPreset->addItem( "LeCocktail",       QGradient::LeCocktail );                         
    pPreset->addItem( "RiverCity",        QGradient::RiverCity );                          
    pPreset->addItem( "FrozenBerry",      QGradient::FrozenBerry );                        
    pPreset->addItem( "ChildCare",        QGradient::ChildCare );                          
    pPreset->addItem( "FlyingLemon",      QGradient::FlyingLemon );                        
    pPreset->addItem( "NewRetrowave",     QGradient::NewRetrowave );                       
    pPreset->addItem( "HiddenJaguar",     QGradient::HiddenJaguar );                       
    pPreset->addItem( "AboveTheSky",      QGradient::AboveTheSky );                        
    pPreset->addItem( "Nega",             QGradient::Nega );                               
    pPreset->addItem( "DenseWater",       QGradient::DenseWater );                         
    pPreset->addItem( "Seashore",         QGradient::Seashore );                           
    pPreset->addItem( "MarbleWall",       QGradient::MarbleWall );                         
    pPreset->addItem( "CheerfulCaramel",  QGradient::CheerfulCaramel );                    
    pPreset->addItem( "NightSky",         QGradient::NightSky );                           
    pPreset->addItem( "MagicLake",        QGradient::MagicLake );                          
    pPreset->addItem( "YoungGrass",       QGradient::YoungGrass );                         
    pPreset->addItem( "ColorfulPeach",    QGradient::ColorfulPeach );                      
    pPreset->addItem( "GentleCare",       QGradient::GentleCare );                         
    pPreset->addItem( "PlumBath",         QGradient::PlumBath );                           
    pPreset->addItem( "HappyUnicorn",     QGradient::HappyUnicorn );                       
    pPreset->addItem( "AfricanField",     QGradient::AfricanField );                       
    pPreset->addItem( "SolidStone",       QGradient::SolidStone );                         
    pPreset->addItem( "OrangeJuice",      QGradient::OrangeJuice );                        
    pPreset->addItem( "GlassWater",       QGradient::GlassWater );                         
    pPreset->addItem( "NorthMiracle",     QGradient::NorthMiracle );                       
    pPreset->addItem( "FruitBlend",       QGradient::FruitBlend );                         
    pPreset->addItem( "MillenniumPine",   QGradient::MillenniumPine );                     
    pPreset->addItem( "HighFlight",       QGradient::HighFlight );                         
    pPreset->addItem( "MoleHall",         QGradient::MoleHall );                           
    pPreset->addItem( "SpaceShift",       QGradient::SpaceShift );                         
    pPreset->addItem( "ForestInei",       QGradient::ForestInei );                         
    pPreset->addItem( "RoyalGarden",      QGradient::RoyalGarden );                        
    pPreset->addItem( "RichMetal",        QGradient::RichMetal );                          
    pPreset->addItem( "JuicyCake",        QGradient::JuicyCake );                          
    pPreset->addItem( "SmartIndigo",      QGradient::SmartIndigo );                        
    pPreset->addItem( "SandStrike",       QGradient::SandStrike );                         
    pPreset->addItem( "NorseBeauty",      QGradient::NorseBeauty );                        
    pPreset->addItem( "AquaGuidance",     QGradient::AquaGuidance );                       
    pPreset->addItem( "SunVeggie",        QGradient::SunVeggie );                          
    pPreset->addItem( "SeaLord",          QGradient::SeaLord );                            
    pPreset->addItem( "BlackSea",         QGradient::BlackSea );                           
    pPreset->addItem( "GrassShampoo",     QGradient::GrassShampoo );                       
    pPreset->addItem( "LandingAircraft",  QGradient::LandingAircraft );                    
    pPreset->addItem( "WitchDance",       QGradient::WitchDance );                         
    pPreset->addItem( "SleeplessNight",   QGradient::SleeplessNight );                     
    pPreset->addItem( "AngelCare",        QGradient::AngelCare );                          
    pPreset->addItem( "CrystalRiver",     QGradient::CrystalRiver );                       
    pPreset->addItem( "SoftLipstick",     QGradient::SoftLipstick );                       
    pPreset->addItem( "SaltMountain",     QGradient::SaltMountain );                       
    pPreset->addItem( "PerfectWhite",     QGradient::PerfectWhite );                       
    pPreset->addItem( "FreshOasis",       QGradient::FreshOasis );                         
    pPreset->addItem( "StrictNovember",   QGradient::StrictNovember );                     
    pPreset->addItem( "MorningSalad",     QGradient::MorningSalad );                       
    pPreset->addItem( "DeepRelief",       QGradient::DeepRelief );                         
    pPreset->addItem( "SeaStrike",        QGradient::SeaStrike );                          
    pPreset->addItem( "NightCall",        QGradient::NightCall );                          
    pPreset->addItem( "SupremeSky",       QGradient::SupremeSky );                         
    pPreset->addItem( "LightBlue",        QGradient::LightBlue );                          
    pPreset->addItem( "MindCrawl",        QGradient::MindCrawl );                          
    pPreset->addItem( "LilyMeadow",       QGradient::LilyMeadow );                         
    pPreset->addItem( "SugarLollipop",    QGradient::SugarLollipop );                      
    pPreset->addItem( "SweetDessert",     QGradient::SweetDessert );                       
    pPreset->addItem( "MagicRay",         QGradient::MagicRay );                           
    pPreset->addItem( "TeenParty",        QGradient::TeenParty );                          
    pPreset->addItem( "FrozenHeat",       QGradient::FrozenHeat );                         
    pPreset->addItem( "GagarinView",      QGradient::GagarinView );                        
    pPreset->addItem( "FabledSunset",     QGradient::FabledSunset );                       
    pPreset->addItem( "PerfectBlue",      QGradient::PerfectBlue );                        
//    pPreset->setCurrentIndex( pPreset->findData( g_Context->getGradient() ) );
    pLayout->addWidget( pPreset );
//    connect( pPreset, SIGNAL(activated(int)), SLOT(slotPreset(int)) );

    pType = new QComboBox( this );
    pType->addItem( "Linear", QGradient::LinearGradient );
    pType->addItem( "Radial", QGradient::RadialGradient );
    pType->addItem( "Conical", QGradient::ConicalGradient );
    pType->addItem( "NoGradient", QGradient::NoGradient );
    pType->setCurrentIndex( pType->findData( g_Context->getGradient() ) );
    pLayout->addWidget( pType );
    connect( pType, SIGNAL(activated(int)), SLOT(slotType(int)) );

    pLayout->addStretch( 10 );

    connect( g_Context, SIGNAL(signalModified(QGradient::Type)), SLOT(slotRefresh(QGradient::Type)) );
    connect( g_Context, SIGNAL(signalModified(const QLinearGradient &)), SLOT(slotRefresh(const QLinearGradient &)) );
    connect( g_Context, SIGNAL(signalModified(const QRadialGradient &)), SLOT(slotRefresh(const QRadialGradient &)) );
    connect( g_Context, SIGNAL(signalModified(const QConicalGradient &)), SLOT(slotRefresh(const QConicalGradient &)) );
}

void PFillGradientToolBar::slotRefresh( QGradient::Type n )
{
    pType->setCurrentIndex( pType->findData( n ) );
}

void PFillGradientToolBar::slotRefresh( const QLinearGradient &t )
{
}

void PFillGradientToolBar::slotRefresh( const QRadialGradient &t )
{
}

void PFillGradientToolBar::slotRefresh( const QConicalGradient &t )
{
}

void PFillGradientToolBar::slotType( int n )
{
    g_Context->setGradient( (QGradient::Type)pType->itemData( n ).toInt() );
}

