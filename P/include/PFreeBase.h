#ifndef H_PFreeBase
#define H_PFreeBase

#include "PContext.h"

class PCanvas;

class PFreeBase
{
public:
    PFreeBase( PCanvas *pCanvas );
    virtual ~PFreeBase() {};

    virtual QRect doPress( QMouseEvent *pEvent ) = 0;                            /*!< mouse press from canvas - return false when done   */
    virtual QRect doMove( QMouseEvent *pEvent ) = 0;                             /*!< mouse move from canvas - return false when done    */
    virtual QRect doRelease( QMouseEvent *pEvent ) = 0;                          /*!< mouse release from canvas - return false when done */

protected:
    PCanvas *pCanvas;
};

#endif
