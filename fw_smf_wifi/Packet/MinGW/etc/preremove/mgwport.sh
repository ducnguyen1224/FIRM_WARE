if [ -f /mingw/etc/mgwport.conf ] && cmp -s /mingw/etc/defaults/mingw/etc/mgwport.conf /mingw/etc/mgwport.conf
then
    rm /mingw/etc/mgwport.conf
fi

