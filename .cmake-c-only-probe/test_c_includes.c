#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "archiver.h"
#include "bedug.h"
#include "bmacro.h"
#include "circle_buf_gc.h"
#include "fsm_gc.h"
#include "glog.h"
#include "gstate.h"
#include "gstring.h"
#include "gtime.h"
#include "gutils.h"
#include "gversion.h"
#include "queue_gc.h"

int main(void)
{
    gtimer_t tm = {0};
    gversion_t ver = {0};
    archiver_status_t st = ARCHIVER_OK;

    (void)tm;
    (void)ver;
    (void)st;
    (void)archiver_min_frame_size();

    return 0;
}
