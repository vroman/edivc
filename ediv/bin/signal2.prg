program prueba_signal2;
global
    idbola;

begin
    load_fnt("system.fnt");
    load_fpg("test16.fpg");
    put_screen(0,1);

    write_int(1,0,0,0,&idbola);

    idbola=bola(0);

    loop
        frame;
        if(key(_f))
            signal(idbola,s_freeze_tree);
            while(key(_f))
                frame;
            end
            signal(idbola,s_wakeup_tree);
        end
        if(key(_s))
            signal(idbola,s_sleep_tree);
            while(key(_s))
                frame;
            end
            signal(idbola,s_wakeup_tree);
        end
        if(key(_k))
            signal(idbola,s_kill_tree);
            while(key(_k))
                frame;
            end
            triangulo(7);
            idbola=bola(0);
        end
        if(key(_esc))
            exit("Taluego lucas!!",0);
        end
    end
end

process bola(num)
begin
    graph=100;
    x=160;
    y=30+num*20;

    if(num<3)
        bola(num+1);
    else
        triangulo(num+1);
    end

    loop
        frame;
        x+=rand(-5,5);
        if(x<30) x=30; end
        if(x>290) x=290; end
    end
end

process triangulo(num)
begin
    graph=101;
    x=160;
    y=30+num*20;

    if(num<6)
        triangulo(num+1);
    end

    loop
        frame;
        x+=rand(-5,5);
        if(x<30) x=30; end
        if(x>290) x=290; end
    end
end

