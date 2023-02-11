#ifndef H_PFreeBase
#define H_PFreeBase

#include "PContext.h"

class PCanvas;

class PFreeBase
{
public:
    PFreeBase( PCanvas *pCanvas );
    virtual ~PFreeBase() {};

    virtual void doPress( PMouseEvent *pEvent ) = 0;                            /*!< mouse press from canvas    */
    virtual void doMove( PMouseEvent *pEvent ) = 0;                             /*!< mouse move from canvas     */
    virtual void doRelease( PMouseEvent *pEvent ) = 0;                          /*!< mouse release from canvas  */

protected:
    PCanvas *pCanvas;
};

#endif
