#ifndef H_PDrawFreeHand
#define H_PDrawFreeHand

#include <WImageButton.h>

#include "PFreeBase.h"

class PDrawFreeHand : public PFreeBase
{
public:
    PDrawFreeHand( PCanvas *pCanvas );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doMove( QMouseEvent *pEvent ) override;
    virtual QRect doRelease( QMouseEvent *pEvent ) override;

protected:
    QPoint              pointLast;
    QPen                pen;
    QBrush              brush;
    PContextFreeHand    t;

    QRect doDraw( const QPoint & );
    QRect doDrawPen( const QPoint & );
    QRect doDrawEllipse( const QPoint & );
    QRect doDrawRectangle( const QPoint & );
    QRect doDrawCross( const QPoint & );
    QRect doDrawImage( const QPoint & );
};

class PFreeHandToolBar : public QWidget
{
    Q_OBJECT
public:
    PFreeHandToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextFreeHand & );

protected slots:
    void slotShape( int );
    void slotWidth( int );
    void slotHeight( int );
    void slotImage();

protected:
    QComboBox *     pShape;
    QSpinBox *      pWidth;
    QSpinBox *      pHeight;
    WImageButton *  pImage;
};

#endif
