program bum;
private
    numprocs;

begin
    rand_seed(343);
    load_bmp("hola.bmp");
    load_fnt("system.fnt");
    write_int(1,0,0,0,&numprocs);
    loop
        if(key(_space)) cosa(); numprocs++; end
        frame;
    end
end

process cosa()
begin
    graph=1000;

    x=rand(0,319);
    y=rand(0,199);

    loop
        frame;
        x+=rand(-1,1);
        y+=rand(-1,1);
    end
end

