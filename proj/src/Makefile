PROG=proj

SRCS = proj.c vector2d.c game.c car.c gameObject.c sprite.c tile.c rectangle.c circle.c keyboard.c video_gr.c  timer.c bitmap.c list.c obstacle.c quadtree.c powerup.c mouse.c camera.c numbers.c pista.c MouseD.c rtc.c serial_port.c serial_port_asm.S

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf -lm


.include <minix.lcom.mk>
