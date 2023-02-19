#ifndef H_PFillGradient
#define H_PFillGradient

#include "PShapeBase.h"

class PFillGradient : public PShapeBase
{
    Q_OBJECT
public:
    PFillGradient( PCanvas *pCanvas );
    virtual ~PFillGradient();

    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent * ) override;
    virtual void doPress( PMouseEvent * ) override;
    virtual void doMove( PMouseEvent * ) override; 
    virtual void doRelease( PMouseEvent * ) override;
    virtual void doCommit() override;

public slots:
    void slotRefresh( const PContextGradient & );
    void slotRefresh( const PContextGradientLinear & );
    void slotRefresh( const PContextGradientRadial & );
    void slotRefresh( const PContextGradientConical & );

protected:
    int                 nType               = 0;
    QPoint              pointSeed;
    QPolygon            polygon;
    QGradient *         pGradientPreset     = nullptr;
    QLinearGradient *   pGradientLinear     = nullptr;
    QRadialGradient *   pGradientRadial     = nullptr;
    QConicalGradient *  pGradientConical    = nullptr;

    // linear
    struct Linear_t
    {
        QPoint pointStart;
        QPoint pointStop;
    };
    Linear_t linear;

    // radial
    // - center is always the seed point
    struct Radial_t
    {
        qreal   nRadius = 10;
        QPointF pointFocalPoint;
    };
    Radial_t radial;

    // conical
    // - center is always the seed point
    struct Conical_t
    {
        qreal nRadius = 50;
        qreal nStartAngle = 10;
    };
    Conical_t conical;

    virtual void doPaint( QPainter * );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );

private:
    struct Polar
    {
        qreal radius;
        qreal angle;
    };

    QPolygon getPolygon( const QPoint &point );

    QPoint getLeft( const QPoint &point ) { return QPoint( point.y(), -point.x() ); }
    QPoint getRight( const QPoint &point ) { return QPoint( -point.y(), point.x() ); }

    bool isBoundary( QImage *pImage, const QColor &color, const QPoint &point )
    {
        return ( point.x() < 0 || point.x() >= pImage->size().width() || point.y() < 0 || point.y() >= pImage->size().height() || pImage->pixelColor( point ) != color );
    }

    QPointF getPolarToCartesian( qreal nRadius, qreal nAngle );
    Polar   getCartesianToPolar( qreal x, qreal y );
};

class PFillGradientToolBar : public QWidget
{
    Q_OBJECT
public:
    PFillGradientToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextGradient & );

protected slots:
    void slotType( int );

protected:
    QComboBox *pType;

private:
    QPixmap getSwatch( const QSize size, int nType );

    void doAddType( const QString &stringText, int nType );
};

#endif
