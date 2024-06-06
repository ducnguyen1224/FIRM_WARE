if [ ! -f /mingw/etc/mgwport.conf ]
then
    /usr/bin/mkdir -p /mingw/etc
    /usr/bin/cp /mingw/etc/defaults/mingw/etc/mgwport.conf /mingw/etc/mgwport.conf
fi

if [ -x /mingw/bin/update-mime-database ]
then
    /mingw/bin/update-mime-database /mingw/share/mime
fi

