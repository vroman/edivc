program hola;
private
    fuente;

begin
    set_mode(320,200,24);
    graph=load_bmp("hola.bmp");
    smooth=1;
    x=1600;
    y=1000;
    resolution=10;
    size=200;
    fuente=load_fnt("system.fnt");
    write(fuente,160,40,4,"HOLA");
    define_region(1,10,10,300,180);
    region=1;
    loop
        frame;
        angle+=1000;
        if(key(_left)) x-=5; end
        if(key(_right)) x+=5; end
        if(key(_up)) y-=5; end
        if(key(_down)) y+=5; end
        if(key(_a)) size+=10; end
        if(key(_z)) size-=10; end
    end
end

