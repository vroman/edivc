program prueba_signal;
global
    continua;

private
    wri1;
    wri2;

begin
    load_fnt("system.fnt");
    load_fpg("test16.fpg");
    wri1=write(1,160,30,4,"Vamos a probar la funcion Signal...");
    wri2=write(1,160,40,4,"Pulsa ENTER para continuar.");
    espera_enter();
    while(!continua) frame; end
    continua=0;

    delete_text(wri1);
    wri1=write(1,160,30,4,"Hemos creado un proceso. Vamos a congelarlo.");
    bola(160,100);
    espera_enter();
    while(!continua) frame; end
    continua=0;

    delete_text(wri1);
    wri1=write(1,160,30,4,"Ya esta... ¨ha funcionado?");
    signal(type bola,s_freeze);
    espera_enter();
    while(!continua) frame; end
    continua=0;

    exit("Fin de la prueba, por ahora :P",0);
    
end

process espera_enter()
begin
    while(!key(_enter))
        frame;
    end
    while(key(_enter))
        frame;
    end
    continua=1;
end

process bola(x1,y1)
private
    angulo;

begin
    x=x1;
    y=y1;
    graph=100;
    loop
        for(angulo=0;angulo<360000;angulo+=1000)
            x=x1+get_distx((angulo-45000),50);
            y=y1+get_disty((angulo-45000)*2,20);
            frame;
        end
    end
end

