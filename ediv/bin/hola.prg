program hola;
private
    fuente;

begin
    graph=load_bmp("hola.bmp");
    x=160;
    y=100;
    fuente=load_fnt("system.fnt");
    write(fuente,160,40,4,"HOLA");
    loop
        frame;
        size+=10;
    end
end

