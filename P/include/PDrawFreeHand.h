#ifndef H_PDrawFreeHand
#define H_PDrawFreeHand

#include <WImageButton.h>

#include "PFreeBase.h"

class PDrawFreeHand : public PFreeBase
{
public:
    PDrawFreeHand( PCanvas *pCanvas );

    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override;
    virtual void doRelease( PMouseEvent *pEvent ) override;

protected:
    QPoint              pointLast;
    QPen                pen;
    QBrush              brush;
    PContextFreeHand    t;

    void doDrawState( const QPoint & );
    void doDrawPen( const QPoint & );
    void doDrawEllipse( const QPoint & );
    void doDrawRectangle( const QPoint & );
    void doDrawCross( const QPoint & );
    void doDrawImage( const QPoint & );
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
