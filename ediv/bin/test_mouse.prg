program test_mouse;
private
    mi_mapa;

begin
    mi_mapa=load_bmp("hola.bmp");
    mouse.graph=mi_mapa;
    loop
        frame;
    end
end
