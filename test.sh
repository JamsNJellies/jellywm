make

XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinit -- \
    "$XEPHYR" \
        :100 \
        -ac \
        -screen 1280x768 \
-host-cursor
