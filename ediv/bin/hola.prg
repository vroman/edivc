program hola;
private
    fuente;

begin
    set_mode(320,200,24,_fullscreen);
    graph=load_bmp("hola.bmp");
    smooth=1;
    x=160;
    y=100;
    size=200;
    fuente=load_fnt("system.fnt");
    write(fuente,160,40,4,"HOLA");
    loop
        frame;
        angle+=1000;
    end
end

