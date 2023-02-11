#ifndef H_PLayers
#define H_PLayers

#include "PBackground.h"
#include "PCanvas.h"

/*!
 * \brief A stack of \sa PCanvas widgets with a \sa PBackground always at the back. 
 *  
 * re. stacking order 
 *  
 * This will always have exactly 1 PBackground and it will always be at the back. 
 * This will have 1-n PCanvas layered on the PBackground - so there will always be a 'current' canvas. 
 *  
 * re. size 
 *  
 * This will harmonize size of PBackground and all of the PCanvas's - they should be same size.
 *  
 * \author pharvey (2/5/23)
 */
class PLayers : public QWidget
{
public:
    PLayers( QWidget *parent, const QSize &size = QSize( 1024, 768 ) );

    PCanvas *getCanvas() { return vectorCanvas.at( nCurrent ); }

protected:
    PBackground *       pBackground;            /*!< only used to indicate transparency - default is transparent tile   */
    QVector<PCanvas *>  vectorCanvas;           /*!< 1-n                                                                */
    int                 nCurrent        = 0;        

    void resizeEvent( QResizeEvent *pEvent ) override;
};

#endif
