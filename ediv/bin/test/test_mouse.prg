program test_mouse;
private
    mi_mapa;

begin
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
    mi_mapa=load_bmp("hola.bmp");
    mouse.graph=mi_mapa;
    loop
        frame;
    end
end
