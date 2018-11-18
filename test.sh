make

XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinit -- \
    "$XEPHYR" \
        :100 \
        -ac \
        -screen 800x600 \
-host-cursor
