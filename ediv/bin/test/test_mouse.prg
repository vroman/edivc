program test_mouse;
private
    mi_mapa;

begin
    set_mode(320,200,24,_fullscreen);
    load_fnt("system.fnt");
    write(1,0,0,0,"X:");
    write(1,0,10,0,"Y:");
    write(1,0,20,0,"Left:");
    write(1,0,30,0,"Middle:");
    write(1,0,40,0,"Right:");
    write(1,0,50,0,"WheelUp:");
    write(1,0,60,0,"WheelDown:");
    write_int(1,80,0,0,&mouse.x);
    write_int(1,80,10,0,&mouse.y);
    write_int(1,80,20,0,&mouse.left);
    write_int(1,80,30,0,&mouse.middle);
    write_int(1,80,40,0,&mouse.right);
    write_int(1,80,50,0,&mouse.wheelup);
    write_int(1,80,60,0,&mouse.wheeldown);
    write_int(1,319,199,8,&mouse.angle);
    write_int(1,319,189,8,&mouse.size);
    mi_mapa=load_bmp("center.bmp");
    mouse.graph=mi_mapa;
    cursor();
    loop
        if(key(_q)) mouse.angle-=1000; end
        if(key(_w)) mouse.angle+=1000; end
        if(key(_a)) mouse.size+=1; end
        if(key(_z)) mouse.size-=1; end
        if(key(_e)) mouse.angle=0; end
        if(key(_x)) mouse.size=100; end
        if(key(_s)) smooth=!smooth; end
        if(key(_1)) mouse.flags^=1; end
        if(key(_2)) mouse.flags^=2; end
        frame;
    end
end

process cursor()
begin
    graph=load_bmp("cursor.bmp");
    z=-600;
    loop
        x=mouse.x;
        y=mouse.y;
        frame;
    end
end

