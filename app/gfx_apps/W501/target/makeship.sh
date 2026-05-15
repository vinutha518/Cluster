#!/bin/bash
shopt -s extglob
date=$(date -u '+%Y%m%d')
svnRevision=$(svn info|grep Revision|cut -d' ' -f2)
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/src/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/res
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/res/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/res
mkdir -p ./ship/vlib/app/common/dhd
cp -p /cygdrive/c/vl2/vlib/app/common/dhd/!(*.a) ./ship/vlib/app/common/dhd
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src
mkdir -p ./ship/vlib/device/d1mx/src/rh850_ghs
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/rh850_ghs/!(*.a) ./ship/vlib/device/d1mx/src/rh850_ghs
mkdir -p ./ship/vlib/device/d1mx/src/dev
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/dev/!(*.a) ./ship/vlib/device/d1mx/src/dev
mkdir -p ./ship/vlib/device/d1x_common/src/dev
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/dev/!(*.a) ./ship/vlib/device/d1x_common/src/dev
mkdir -p ./ship/vlib/bsp/stdio/src
cp -p /cygdrive/c/vl2/vlib/bsp/stdio/src/!(*.a) ./ship/vlib/bsp/stdio/src
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/stdio
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/stdio/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/stdio
mkdir -p ./ship/vlib/bsp/hmi/src
cp -p /cygdrive/c/vl2/vlib/bsp/hmi/src/!(*.a) ./ship/vlib/bsp/hmi/src
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/hmi
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/hmi/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/hmi
mkdir -p ./ship/vlib/bsp/gfx/src
cp -p /cygdrive/c/vl2/vlib/bsp/gfx/src/!(*.a) ./ship/vlib/bsp/gfx/src
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/gfx
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/gfx/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/gfx
mkdir -p ./ship/vlib/macro/bus/pbg/src
cp -p /cygdrive/c/vl2/vlib/macro/bus/pbg/src/!(*.a) ./ship/vlib/macro/bus/pbg/src
mkdir -p ./ship/vlib/device/d1x_common/src/pbg
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/pbg/!(*.a) ./ship/vlib/device/d1x_common/src/pbg
mkdir -p ./ship/vlib/macro/csi/csisw/src
cp -p /cygdrive/c/vl2/vlib/macro/csi/csisw/src/!(*.a) ./ship/vlib/macro/csi/csisw/src
mkdir -p ./ship/vlib/device/d1x_common/src/csisw
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/csisw/!(*.a) ./ship/vlib/device/d1x_common/src/csisw
mkdir -p ./ship/vlib/macro/drw2d/platform/davehd
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/platform/davehd/!(*.a) ./ship/vlib/macro/drw2d/platform/davehd
mkdir -p ./ship/vlib/macro/drw2d/platform/os/no_os
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/platform/os/no_os/!(*.a) ./ship/vlib/macro/drw2d/platform/os/no_os
mkdir -p ./ship/vlib/macro/drw2d/src
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/src/!(*.a) ./ship/vlib/macro/drw2d/src
mkdir -p ./ship/vlib/macro/flashc/sfcdb/src
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfcdb/src/!(*.a) ./ship/vlib/macro/flashc/sfcdb/src
mkdir -p ./ship/vlib/macro/flashc/sfma/src
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfma/src/!(*.a) ./ship/vlib/macro/flashc/sfma/src
mkdir -p ./ship/vlib/device/d1x_common/src/sfma
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/sfma/!(*.a) ./ship/vlib/device/d1x_common/src/sfma
mkdir -p ./ship/vlib/macro/gfxbus/src
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/src/!(*.a) ./ship/vlib/macro/gfxbus/src
mkdir -p ./ship/vlib/device/d1x_common/src/gfxbus
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/gfxbus/!(*.a) ./ship/vlib/device/d1x_common/src/gfxbus
mkdir -p ./ship/vlib/macro/gfxbus/xbus/src
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/xbus/src/!(*.a) ./ship/vlib/macro/gfxbus/xbus/src
mkdir -p ./ship/vlib/device/d1x_common/src/xbus
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/xbus/!(*.a) ./ship/vlib/device/d1x_common/src/xbus
mkdir -p ./ship/vlib/macro/gpio/src
cp -p /cygdrive/c/vl2/vlib/macro/gpio/src/!(*.a) ./ship/vlib/macro/gpio/src
mkdir -p ./ship/vlib/device/d1x_common/src/gpio
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/gpio/!(*.a) ./ship/vlib/device/d1x_common/src/gpio
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/src
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/src/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/src
mkdir -p ./ship/vlib/macro/gpu/davehd/user/src
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/user/src/!(*.a) ./ship/vlib/macro/gpu/davehd/user/src
mkdir -p ./ship/vlib/macro/gpu/davehd/common/inc
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/common/inc/!(*.a) ./ship/vlib/macro/gpu/davehd/common/inc
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/platform/d1mx/bridge
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/platform/d1mx/bridge/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/platform/d1mx/bridge
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/platform/d1mx
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/platform/d1mx/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/platform/d1mx
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/platform/ree
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/platform/ree/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/platform/ree
mkdir -p ./ship/vlib/macro/gpu/davehd/user/platform/d1mx/no_os
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/user/platform/d1mx/no_os/!(*.a) ./ship/vlib/macro/gpu/davehd/user/platform/d1mx/no_os
mkdir -p ./ship/vlib/device/d1mx/src/dhd
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/dhd/!(*.a) ./ship/vlib/device/d1mx/src/dhd
mkdir -p ./ship/vlib/macro/gpu/jcua/src
cp -p /cygdrive/c/vl2/vlib/macro/gpu/jcua/src/!(*.a) ./ship/vlib/macro/gpu/jcua/src
mkdir -p ./ship/vlib/device/d1mx/src/jcua
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/jcua/!(*.a) ./ship/vlib/device/d1mx/src/jcua
mkdir -p ./ship/vlib/macro/i2c/riic/src
cp -p /cygdrive/c/vl2/vlib/macro/i2c/riic/src/!(*.a) ./ship/vlib/macro/i2c/riic/src
mkdir -p ./ship/vlib/device/d1x_common/src/riic
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/riic/!(*.a) ./ship/vlib/device/d1x_common/src/riic
mkdir -p ./ship/vlib/macro/tick/src
cp -p /cygdrive/c/vl2/vlib/macro/tick/src/!(*.a) ./ship/vlib/macro/tick/src
mkdir -p ./ship/vlib/device/d1x_common/src/tick
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/tick/!(*.a) ./ship/vlib/device/d1x_common/src/tick
mkdir -p ./ship/vlib/macro/timer/ostm/src
cp -p /cygdrive/c/vl2/vlib/macro/timer/ostm/src/!(*.a) ./ship/vlib/macro/timer/ostm/src
mkdir -p ./ship/vlib/device/d1x_common/src/ostm
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/ostm/!(*.a) ./ship/vlib/device/d1x_common/src/ostm
mkdir -p ./ship/vlib/macro/timer/tauj/src
cp -p /cygdrive/c/vl2/vlib/macro/timer/tauj/src/!(*.a) ./ship/vlib/macro/timer/tauj/src
mkdir -p ./ship/vlib/device/d1x_common/src/tauj
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/tauj/!(*.a) ./ship/vlib/device/d1x_common/src/tauj
mkdir -p ./ship/vlib/macro/vdce/src
cp -p /cygdrive/c/vl2/vlib/macro/vdce/src/!(*.a) ./ship/vlib/macro/vdce/src
mkdir -p ./ship/vlib/device/d1x_common/src/vdce
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/vdce/!(*.a) ./ship/vlib/device/d1x_common/src/vdce
mkdir -p ./ship/vlib/macro/vo/ddb/src
cp -p /cygdrive/c/vl2/vlib/macro/vo/ddb/src/!(*.a) ./ship/vlib/macro/vo/ddb/src
mkdir -p ./ship/vlib/device/d1x_common/src/ddb
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/ddb/!(*.a) ./ship/vlib/device/d1x_common/src/ddb
mkdir -p ./ship/vlib/macro/vo/spea/src
cp -p /cygdrive/c/vl2/vlib/macro/vo/spea/src/!(*.a) ./ship/vlib/macro/vo/spea/src
mkdir -p ./ship/vlib/device/d1x_common/src/spea
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/spea/!(*.a) ./ship/vlib/device/d1x_common/src/spea
mkdir -p ./ship/vlib/macro/vo/vowe/src
cp -p /cygdrive/c/vl2/vlib/macro/vo/vowe/src/!(*.a) ./ship/vlib/macro/vo/vowe/src
mkdir -p ./ship/vlib/device/d1mx/src/vowe
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/vowe/!(*.a) ./ship/vlib/device/d1mx/src/vowe
mkdir -p ./ship/vlib/macro/wm/src
cp -p /cygdrive/c/vl2/vlib/macro/wm/src/!(*.a) ./ship/vlib/macro/wm/src
mkdir -p ./ship/vlib/device/d1x_common/src/wm
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/src/wm/!(*.a) ./ship/vlib/device/d1x_common/src/wm
mkdir -p ./ship/vlib/cdi/src
cp -p /cygdrive/c/vl2/vlib/cdi/src/!(*.a) ./ship/vlib/cdi/src
mkdir -p ./ship/vlib/middleware/gfxutils/src
cp -p /cygdrive/c/vl2/vlib/middleware/gfxutils/src/!(*.a) ./ship/vlib/middleware/gfxutils/src
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/config
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/config/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/config
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/src/images
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/res
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/res/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/res
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/!(*.a) ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs
mkdir -p ./ship/vlib/app/common/dhd
cp -p /cygdrive/c/vl2/vlib/app/common/dhd/!(*.a) ./ship/vlib/app/common/dhd
mkdir -p ./ship/vlib/bsp/lib
cp -p /cygdrive/c/vl2/vlib/bsp/lib/!(*.a) ./ship/vlib/bsp/lib
mkdir -p ./ship/vlib/device/lib
cp -p /cygdrive/c/vl2/vlib/device/lib/!(*.a) ./ship/vlib/device/lib
mkdir -p ./ship/vlib/bsp/stdio/lib
cp -p /cygdrive/c/vl2/vlib/bsp/stdio/lib/!(*.a) ./ship/vlib/bsp/stdio/lib
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/config
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/config/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/config
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/stdio
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/stdio/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/stdio
mkdir -p ./ship/vlib/bsp/board//config
cp -p /cygdrive/c/vl2/vlib/bsp/board//config/!(*.a) ./ship/vlib/bsp/board//config
mkdir -p ./ship/vlib/bsp/board//src/stdio
cp -p /cygdrive/c/vl2/vlib/bsp/board//src/stdio/!(*.a) ./ship/vlib/bsp/board//src/stdio
mkdir -p ./ship/vlib/bsp/hmi/lib
cp -p /cygdrive/c/vl2/vlib/bsp/hmi/lib/!(*.a) ./ship/vlib/bsp/hmi/lib
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/config
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/config/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/config
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/hmi
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/hmi/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/hmi
mkdir -p ./ship/vlib/bsp/board//config
cp -p /cygdrive/c/vl2/vlib/bsp/board//config/!(*.a) ./ship/vlib/bsp/board//config
mkdir -p ./ship/vlib/bsp/board//src/hmi
cp -p /cygdrive/c/vl2/vlib/bsp/board//src/hmi/!(*.a) ./ship/vlib/bsp/board//src/hmi
mkdir -p ./ship/vlib/bsp/gfx/lib
cp -p /cygdrive/c/vl2/vlib/bsp/gfx/lib/!(*.a) ./ship/vlib/bsp/gfx/lib
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/config
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/config/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/config
mkdir -p ./ship/vlib/bsp/board/d1mx_mango/src/gfx
cp -p /cygdrive/c/vl2/vlib/bsp/board/d1mx_mango/src/gfx/!(*.a) ./ship/vlib/bsp/board/d1mx_mango/src/gfx
mkdir -p ./ship/vlib/bsp/board//config
cp -p /cygdrive/c/vl2/vlib/bsp/board//config/!(*.a) ./ship/vlib/bsp/board//config
mkdir -p ./ship/vlib/bsp/board//src/gfx
cp -p /cygdrive/c/vl2/vlib/bsp/board//src/gfx/!(*.a) ./ship/vlib/bsp/board//src/gfx
mkdir -p ./ship/vlib/macro/bus/pbg/lib
cp -p /cygdrive/c/vl2/vlib/macro/bus/pbg/lib/!(*.a) ./ship/vlib/macro/bus/pbg/lib
mkdir -p ./ship/vlib/macro/csi/csisw/lib
cp -p /cygdrive/c/vl2/vlib/macro/csi/csisw/lib/!(*.a) ./ship/vlib/macro/csi/csisw/lib
mkdir -p ./ship/vlib/macro/drw2d/lib
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/lib/!(*.a) ./ship/vlib/macro/drw2d/lib
mkdir -p ./ship/vlib/macro/drw2d/platform/davehd
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/platform/davehd/!(*.a) ./ship/vlib/macro/drw2d/platform/davehd
mkdir -p ./ship/vlib/macro/drw2d/lib
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/lib/!(*.a) ./ship/vlib/macro/drw2d/lib
mkdir -p ./ship/vlib/macro/flashc/sfcdb/lib
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfcdb/lib/!(*.a) ./ship/vlib/macro/flashc/sfcdb/lib
mkdir -p ./ship/vlib/macro/flashc/sfma/lib
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfma/lib/!(*.a) ./ship/vlib/macro/flashc/sfma/lib
mkdir -p ./ship/vlib/macro/gfxbus/lib
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/lib/!(*.a) ./ship/vlib/macro/gfxbus/lib
mkdir -p ./ship/vlib/macro/gfxbus/xbus/lib
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/xbus/lib/!(*.a) ./ship/vlib/macro/gfxbus/xbus/lib
mkdir -p ./ship/vlib/macro/gpio/lib
cp -p /cygdrive/c/vl2/vlib/macro/gpio/lib/!(*.a) ./ship/vlib/macro/gpio/lib
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/inc
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/inc/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/inc
mkdir -p ./ship/vlib/macro/gpu/davehd/user/inc
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/user/inc/!(*.a) ./ship/vlib/macro/gpu/davehd/user/inc
mkdir -p ./ship/vlib/macro/gpu/davehd/common/inc
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/common/inc/!(*.a) ./ship/vlib/macro/gpu/davehd/common/inc
mkdir -p ./ship/vlib/macro/gpu/davehd/common/inc/platform
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/common/inc/platform/!(*.a) ./ship/vlib/macro/gpu/davehd/common/inc/platform
mkdir -p ./ship/vlib/macro/gpu/davehd/common/inc/platform/d1mx
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/common/inc/platform/d1mx/!(*.a) ./ship/vlib/macro/gpu/davehd/common/inc/platform/d1mx
mkdir -p ./ship/vlib/macro/gpu/davehd/user/src
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/user/src/!(*.a) ./ship/vlib/macro/gpu/davehd/user/src
mkdir -p ./ship/vlib/macro/gpu/davehd/kernel/src
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/kernel/src/!(*.a) ./ship/vlib/macro/gpu/davehd/kernel/src
mkdir -p ./ship/vlib/macro/gpu/davehd/user/platform/d1mx/no_os
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/user/platform/d1mx/no_os/!(*.a) ./ship/vlib/macro/gpu/davehd/user/platform/d1mx/no_os
mkdir -p ./ship/vlib/device/d1mx/src/dhd
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/dhd/!(*.a) ./ship/vlib/device/d1mx/src/dhd
mkdir -p ./ship/vlib/macro/gpu/jcua/lib
cp -p /cygdrive/c/vl2/vlib/macro/gpu/jcua/lib/!(*.a) ./ship/vlib/macro/gpu/jcua/lib
mkdir -p ./ship/vlib/macro/i2c/riic/lib
cp -p /cygdrive/c/vl2/vlib/macro/i2c/riic/lib/!(*.a) ./ship/vlib/macro/i2c/riic/lib
mkdir -p ./ship/vlib/macro/tick/lib
cp -p /cygdrive/c/vl2/vlib/macro/tick/lib/!(*.a) ./ship/vlib/macro/tick/lib
mkdir -p ./ship/vlib/macro/timer/ostm/lib
cp -p /cygdrive/c/vl2/vlib/macro/timer/ostm/lib/!(*.a) ./ship/vlib/macro/timer/ostm/lib
mkdir -p ./ship/vlib/macro/timer/tauj/lib
cp -p /cygdrive/c/vl2/vlib/macro/timer/tauj/lib/!(*.a) ./ship/vlib/macro/timer/tauj/lib
mkdir -p ./ship/vlib/macro/vdce/lib
cp -p /cygdrive/c/vl2/vlib/macro/vdce/lib/!(*.a) ./ship/vlib/macro/vdce/lib
mkdir -p ./ship/vlib/macro/vo/ddb/lib
cp -p /cygdrive/c/vl2/vlib/macro/vo/ddb/lib/!(*.a) ./ship/vlib/macro/vo/ddb/lib
mkdir -p ./ship/vlib/macro/vo/spea/lib
cp -p /cygdrive/c/vl2/vlib/macro/vo/spea/lib/!(*.a) ./ship/vlib/macro/vo/spea/lib
mkdir -p ./ship/vlib/macro/vo/vowe/lib
cp -p /cygdrive/c/vl2/vlib/macro/vo/vowe/lib/!(*.a) ./ship/vlib/macro/vo/vowe/lib
mkdir -p ./ship/vlib/macro/wm/lib
cp -p /cygdrive/c/vl2/vlib/macro/wm/lib/!(*.a) ./ship/vlib/macro/wm/lib
mkdir -p ./ship/vlib/cdi/lib
cp -p /cygdrive/c/vl2/vlib/cdi/lib/!(*.a) ./ship/vlib/cdi/lib
mkdir -p ./ship/vlib/middleware/gfxutils/lib
cp -p /cygdrive/c/vl2/vlib/middleware/gfxutils/lib/!(*.a) ./ship/vlib/middleware/gfxutils/lib
mkdir -p ./ship/vlib/compiler/rh850_ghs/inc
cp -p /cygdrive/c/vl2/vlib/compiler/rh850_ghs/inc/!(*.a) ./ship/vlib/compiler/rh850_ghs/inc
mkdir -p ./ship/vlib/device/d1mx/lib
cp -p /cygdrive/c/vl2/vlib/device/d1mx/lib/!(*.a) ./ship/vlib/device/d1mx/lib
mkdir -p ./ship/vlib/device/d1mx/macro_cfg
cp -p /cygdrive/c/vl2/vlib/device/d1mx/macro_cfg/!(*.a) ./ship/vlib/device/d1mx/macro_cfg
mkdir -p ./ship/vlib/device/d1x_common/macro_cfg
cp -p /cygdrive/c/vl2/vlib/device/d1x_common/macro_cfg/!(*.a) ./ship/vlib/device/d1x_common/macro_cfg
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/compiler/rh850_ghs/
mkdir -p ./ship/vlib/device/d1mx/
mkdir -p ./ship/vlib/device/d1mx/
mkdir -p ./ship/vlib/bsp/stdio/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/bsp/hmi/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/bsp/gfx/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/bus/pbg/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/csi/csisw/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/drw2d/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/flashc/sfcdb/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/flashc/sfma/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/gfxbus/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/gfxbus/xbus/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/gpio/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/gpu/davehd/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/gpu/jcua/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/i2c/riic/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/tick/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/timer/ostm/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/timer/tauj/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/vdce/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/vo/ddb/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/vo/spea/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/vo/vowe/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/macro/wm/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/middleware/gfxutils/
mkdir -p ./ship/vlib/middleware/
mkdir -p ./ship/vlib/compiler/rh850_ghs/
mkdir -p ./ship/vlib/compiler/rh850_ghs/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/compiler/rh850_ghs/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/compiler/
mkdir -p ./ship/vlib/compiler/
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/Makefile ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/Makefile
cp -p /cygdrive/c/vl2/vlib/compiler/setup.mk ./ship/vlib/compiler/setup.mk
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/d1m1_flashdash_demo.mk ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/d1m1_flashdash_demo.mk
cp -p /cygdrive/c/vl2/vlib/compiler/compiler.mk ./ship/vlib/compiler/compiler.mk
cp -p /cygdrive/c/vl2/vlib/compiler/rh850_ghs/arch.mk ./ship/vlib/compiler/rh850_ghs/arch.mk
cp -p /cygdrive/c/vl2/vlib/device/d1mx/d1mx.mk ./ship/vlib/device/d1mx/d1mx.mk
cp -p /cygdrive/c/vl2/vlib/device/d1mx/config_rgl_src.mk ./ship/vlib/device/d1mx/config_rgl_src.mk
cp -p /cygdrive/c/vl2/vlib/bsp/stdio/stdio.mk ./ship/vlib/bsp/stdio/stdio.mk
cp -p /cygdrive/c/vl2/vlib/compiler/bsp.mk ./ship/vlib/compiler/bsp.mk
cp -p /cygdrive/c/vl2/vlib/bsp/hmi/hmi.mk ./ship/vlib/bsp/hmi/hmi.mk
cp -p /cygdrive/c/vl2/vlib/compiler/bsp.mk ./ship/vlib/compiler/bsp.mk
cp -p /cygdrive/c/vl2/vlib/bsp/gfx/gfx.mk ./ship/vlib/bsp/gfx/gfx.mk
cp -p /cygdrive/c/vl2/vlib/compiler/bsp.mk ./ship/vlib/compiler/bsp.mk
cp -p /cygdrive/c/vl2/vlib/macro/bus/pbg/pbg.mk ./ship/vlib/macro/bus/pbg/pbg.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/csi/csisw/csisw.mk ./ship/vlib/macro/csi/csisw/csisw.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/drw2d/drw2d.mk ./ship/vlib/macro/drw2d/drw2d.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfcdb/sfcdb.mk ./ship/vlib/macro/flashc/sfcdb/sfcdb.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/flashc/sfma/sfma.mk ./ship/vlib/macro/flashc/sfma/sfma.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/gfxbus.mk ./ship/vlib/macro/gfxbus/gfxbus.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/gfxbus/xbus/xbus.mk ./ship/vlib/macro/gfxbus/xbus/xbus.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/gpio/gpio.mk ./ship/vlib/macro/gpio/gpio.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/gpu/davehd/davehd.mk ./ship/vlib/macro/gpu/davehd/davehd.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/gpu/jcua/jcua.mk ./ship/vlib/macro/gpu/jcua/jcua.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/i2c/riic/riic.mk ./ship/vlib/macro/i2c/riic/riic.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/tick/tick.mk ./ship/vlib/macro/tick/tick.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/timer/ostm/ostm.mk ./ship/vlib/macro/timer/ostm/ostm.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/timer/tauj/tauj.mk ./ship/vlib/macro/timer/tauj/tauj.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/vdce/vdce.mk ./ship/vlib/macro/vdce/vdce.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/vo/ddb/ddb.mk ./ship/vlib/macro/vo/ddb/ddb.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/vo/spea/spea.mk ./ship/vlib/macro/vo/spea/spea.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/vo/vowe/vowe.mk ./ship/vlib/macro/vo/vowe/vowe.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/macro/wm/wm.mk ./ship/vlib/macro/wm/wm.mk
cp -p /cygdrive/c/vl2/vlib/compiler/macro.mk ./ship/vlib/compiler/macro.mk
cp -p /cygdrive/c/vl2/vlib/middleware/gfxutils/gfxutils.mk ./ship/vlib/middleware/gfxutils/gfxutils.mk
cp -p /cygdrive/c/vl2/vlib/middleware/mware.mk ./ship/vlib/middleware/mware.mk
cp -p /cygdrive/c/vl2/vlib/compiler/rh850_ghs/rules.mk ./ship/vlib/compiler/rh850_ghs/rules.mk
cp -p /cygdrive/c/vl2/vlib/compiler/rh850_ghs/ship.mk ./ship/vlib/compiler/rh850_ghs/ship.mk
cp -p /cygdrive/c/vl2/vlib/compiler/svn.mk ./ship/vlib/compiler/svn.mk
cp -p /cygdrive/c/vl2/vlib/compiler/rh850_ghs/multi.mk ./ship/vlib/compiler/rh850_ghs/multi.mk
cp -p /cygdrive/c/vl2/vlib/compiler/tag.mk ./ship/vlib/compiler/tag.mk
cp -p /cygdrive/c/vl2/vlib/compiler/help.mk ./ship/vlib/compiler/help.mk
cp -p /cygdrive/c/vl2/vlib/compiler/ultraedit.mk ./ship/vlib/compiler/ultraedit.mk
mkdir -p ./ship/vlib/device/d1mx/src/rh850_ghs/
cp -p /cygdrive/c/vl2/vlib/device/d1mx/src/rh850_ghs/d1mx.ld ./ship/vlib/device/d1mx/src/rh850_ghs/d1mx.ld
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/d1m1_flashdash_demo.gpj ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/d1m1_flashdash_demo.gpj
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/d1m1_flashdash_demo.out.rc ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/d1m1_flashdash_demo.out.rc
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/drivers.gpj ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/drivers.gpj
cp -p /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/app.gpj ./ship/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/app.gpj
cp /cygdrive/c/vl2/vlib/compiler/rh850_ghs/ghs_shortcut.bat /cygdrive/c/vl2/vlib/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/ship/vlib/get_ghs_shortcut.bat
cd ./ship
for i in $(find ./vlib -name "*~"); do rm $i; done
for i in $(find ./vlib -name ".svn"); do rm -rf $i; done
chmod -R 777 ./vlib
tar czf d1m1_flashdash_demo_${date}_svnRev${svnRevision}.tar.gz vlib
cd ..
