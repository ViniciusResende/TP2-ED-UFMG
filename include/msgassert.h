#ifndef MSGASSERTH
#define MSGASSERTH

#include <stdio.h>
#include <stdlib.h>

#define warnAssert(e,m) ((void) ((e)?0: __warnassert (#e,__FILE__,__LINE__,m)))
#define __warnassert(e,file,line,m) ((void) fprintf(stderr,"%s:%u: Warn: '%s' - %s\n",file,line,e,m),0)
#define errorAssert(e,m) ((void) ((e)?0: __errorassert (#e,__FILE__,__LINE__,m)))
#define __errorassert(e,file,line,m) ((void) fprintf(stderr,"%s:%u: Error '%s' - %s\n",file,line,e,m), abort(),0)

#endif
