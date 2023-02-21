#ifndef H_PFillGradient
#define H_PFillGradient

#include "PShapeBase.h"

/*!
 * \brief Fill area clicked in - with a gradient. 
 *  
 * Preset 
 *  
 *      Uses a set of predefined gradients based on https://webgradients.com/.
 *  
 * Linear 
 *  
 *      Linear gradients interpolate colors between start and end points.
 *      Outside these points the gradient is either padded, reflected or repeated depending on the currently set spread method.
 *  
 * Radial 
 *  
 *      Simple
 *  
 *          Simple radial gradients interpolate colors between a focal point and end points on a circle surrounding it.
 *          For simple radial gradients the focal point is adjusted to lie inside the center circle.
 *  
 *      Extended
 *  
 *          Extended radial gradients interpolate colors between a focal circle and a center circle. 
 *          Points outside the cone defined by the two circles will be transparent. 
 *          For extended radial gradients the focal point can have any position in an extended radial gradient.
 *  
 *          \note Not implemented yet.
 *  
 * Conical 
 *  
 *      Conical gradients interpolate colors counter-clockwise around a center point.
 *  
 *      \note Not fully implemented yet. 
 * 
 * \author pharvey (2/20/23)
 */
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
    int                 nType               = 0;    /*!< default is first QGradient::Preset                                     */
    QPoint              pointFactory;               /*!< location of first press. where factory handle resides                  */
    QPolygon            polygon;

    // intermediate stops
    QGradientStops  stops;                          /*!< stops between Start and 'final' Stop (each with a color)               */
    QGradientStop   stopStart;                      /*!< having this here makes managing stops easier (qreal is always 0)       */
    QGradientStop   stopStop;                       /*!< having this here makes managing stops easier (qreal is always 1)       */
    QVector<QPoint> vectorStopPoints;               /*!< stop points in scene coordinates. we can not simply use handle pos.    */

    // linear
    struct Linear_t
    {
        QPoint pointStart;              /*!< start point                                            [Start]                     */
        QPoint pointStop;               /*!< stop point                                             [Stop]                      */
    };
    Linear_t linear;

    // radial
    struct RadialSimple_t
    {
        QPointF pointCenter;            /*!< center circle (same as pointFactory and is fixed)                                  */
        QPointF pointCenterRadius;      /*!< radius for center circle                               [Stop]                      */ 
        QPointF pointFocal;             /*!< focal point (restricted to center circle)              [Start]                     */ 
    };
    RadialSimple_t radialSimple;

    struct RadialExtended_t
    {
        QPointF pointCenter;            /*!< center circle (same as pointFactory and is fixed)      [Start]                     */
        QPointF pointCenterRadius;      /*!< radius for center circle                                                           */
        QPointF pointFocal;             /*!< focal point                                            [Stop]                      */ 
        QPointF pointFocalRadius;       /*!< radius for focal circle                                                            */
    };
    RadialExtended_t radialExtended;

    // conical
    struct Conical_t
    {
        QPointF pointCenter;            /*!< center (same as pointFactory and is fixed)                                         */
        QPointF pointAngle;             /*!< start/stop angle                                                                   */
    };
    Conical_t conical;

    virtual void doPaint( QPainter *, bool bCommit = true );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
    virtual void doUpdateStop( int n );
    virtual void doUpdateStops();
    virtual void doRemoveStop();

    virtual bool shouldRemoveStop();

private:
    struct Polar
    {
        qreal radius;
        qreal angle;        // radians
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

    QPointF getNearestPointOnLine( const QPointF &pointBegin, const QPointF &pointEnd, const QPointF &point );
    qreal   getDistance( const QPointF &pointA, const QPointF &pointB );
    qreal   getRadiansToDegrees( qreal nRadian );
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
    void slotSpread( int );

protected:
    QComboBox *pType;
    QComboBox *pSpread;

private:
    QPixmap getSwatch( const QSize size, int nType );

    void doAddType( const QString &stringText, int nType );
};

#endif
