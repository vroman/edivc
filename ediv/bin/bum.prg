program bum;
private
    numprocs;

begin
    rand_seed(343);
//    load_bmp("hola.bmp");
    load_fpg("test.fpg");
    load_fnt("system.fnt");
    put_screen(0,1);
    write_int(1,0,0,0,&numprocs);
    define_region(1,0,10,320,190);
    loop
        if(key(_space)) cosa(); numprocs++; end
        if(key(_w)) delete_text(all_text); end
        frame;
    end
end

process cosa()
begin
    graph=100;

    x=rand(0,319);
    y=rand(0,199);

    region=1;

    loop
        frame;
        x+=rand(-1,1);
        y+=rand(-1,1);
    end
end

