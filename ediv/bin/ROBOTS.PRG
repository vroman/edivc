/*********************
  Los Robots Asesinos
 *********************
 
  (c) Er_Makina, 2000
  
   www.sferasoft.org
   
 ********************
  Este programa es SOFTWARE LIBRE y puedes hacer con él
  lo de te salga de las narices. Puedes modificarlo y
  redistribuirlo libremente sin necesidad de notificar-
  me nada.
 ********************
  NOTAS:
  - El programa funcionará bajo DIV 1 si se quita la 1ª
    línea del programa (compiler_options)
  - Para ejecutar el EXE, es necesario el archivo
    DIV32RUN.DLL que podrás encontrar en la web oficial
	de DIV (www.divgames.com)
 ********************/

COMPILER_OPTIONS _extended_conditions;

Program Juego_robots;
CONST
    max_robots=25;

GLOBAL
    prota;
    num_robots=5;
    robots[max_robots];
    muerto[max_robots];
    muertos;
    cogido;
    ya;

LOCAL
    i;

PRIVATE
    protax;
    protay;
    mueve;
    harf;
    j;
    espe;

BEGIN
    load_fpg("robots.fpg");
    load_fnt("system.fnt");
    write(1,0,0,0,"Pulsa F1 para obtener ayuda");
    write(1,160,100,4,"¨PREPARADO?");
    espe=1;
    mouse.flags=4;
    loop
//    fade_on();
    prota=protag(rand(0,39)*8,rand(0,24)*8);
    for(i=1;i<=num_robots;i++)
        loop
            x=rand(0,39);
            y=rand(0,24);
            graph=2;
            if(fget_dist(x,y,prota.x/8,prota.y/8)>2)
                x*=8;
                y*=8;
                if(i==1)
                    robots[i]=robot(x,y);
                    muerto[i]=0;
                    break;
                else
                    harf=0;
                    for(j=1;j<=i-1;j++)
                        if(robots[j].x<>x && robots[j].y<>y)
                            harf++;
                        end
                    end
                    if(harf==i-1)
                        robots[i]=robot(x,y);
                        muerto[i]=0;
                        break;
                    end
                end
            end
        end
    end
    graph=0;
    protax=prota.x;
    protay=prota.y;
    if(espe)
        timer[2]=0;
        while(/*fading ||*/ timer[2]<200)
            frame;
        end
        delete_text(all_text);
        espe=0;
    end
    loop
        frame;
        if(cogido)
            repeat
                frame;
            until(ya)
            break;
        end
        if((key(_up) || key(_c_up) || key(_c_home) || key(_c_pgup)) && prota.y>0)
            while(key(_up) || key(_c_up))
                frame;
            end
            protay-=8;
            mueve=1;
        end
        if((key(_down) || key(_c_down) || key(_c_end) || key(_c_pgdn)) && prota.y<191)
            while(key(_down) || key(_c_down))
                frame;
            end
            protay+=8;
            mueve=1;
        end
        if((key(_left) || key(_c_left) || key(_c_home) || key(_c_end)) && prota.x>0)
            while(key(_left) || key(_c_left) || key(_c_home) || key(_c_end))
                frame;
            end
            protax-=8;
            mueve=1;
        end
        if((key(_right) || key(_c_right) || key(_c_pgup) || key(_c_pgdn)) && prota.x<311)
            while(key(_right) || key(_c_right) || key(_c_pgup) || key(_c_pgdn))
                frame;
            end
            protax+=8;
            mueve=1;
        end
        if(key(_c_center))
            while(key(_c_center))
                frame;
            end
            mueve=1;
        end
        if(key(_t))
            while(key(_t))
                frame;
            end
            loop
                x=rand(0,39);
                y=rand(0,24);
                j=0;
                for(i=1;i<=num_robots;i++)
                    if(fget_dist(x,y,robots[i].x/8,robots[i].y/8)>2)
                        j++;
                    end
                end
                if(j==num_robots)
                    break;
                end
            end
            protax=x*8;
            protay=y*8;
            mueve=1;
        end
        if(key(_u))
            while(key(_u))
                frame;
            end
            protax=rand(0,39)*8;
            protay=rand(0,24)*8;
            mueve=1;
        end
        if(key(_f1))
            graph=14;
            x=160;
            y=100;
            z=-20;
            while(key(_f1))
                frame;
            end
            repeat
                frame;
            until(key(_space) || key(_enter))
            graph=0;
        end
        if(mueve==1)
            prota.x=protax;
            prota.y=protay;
            mueve=0;
            mueve_robots();
        end
    end
    //signal(prota,s_kill);
    //signal(type robot,s_kill);
    muertos=0;
    cogido=0;
    ya=0;
    end
END

Process Protag(x,y)
PRIVATE
    imagen=0;
    imagenes[12]=1,9,10,11,12,13,12,13,12,11,10,9,1;
    saludando;
    esp_frame;
    coge_robot;

BEGIN
    graph=imagenes[imagen];
    timer[1]=0;
    loop
        frame;
        if(coge_robot=collision(type robot))
            cogido=1;
            if(coge_robot.graph==3)
                write(1,160,100,4,"­­ PERO QU HACES !!");
            else
                write(1,160,100,4,"­­ TE HAN COGIDO !!");
            end
            write(1,160,199,7,"Pulsa una tecla");
            repeat
                frame;
            until(key(_space) || key(_enter))
            /*fade_off();
            while(fading)
                frame;
            end*/
            delete_text(all_text);
            ya=1;
            frame;
        end
        if(saludando==0 && timer[1]=>500)
            saludando=1;
        end
        if(saludando==1)
            if(imagen<12)
                esp_frame++;
                if(esp_frame==2)
                    esp_frame=0;
                    imagen++;
                    graph=imagenes[imagen];
                end
            else
                saludando=0;
                imagen=0;
                timer[1]=0;
            end
        end
        if(muertos==num_robots)
            cogido=1;
            write(1,160,100,4,"­­ MUY BIEN !!");
            write(1,160,199,7,"Pulsa una tecla");
            num_robots++;
            repeat
                frame;
            until(key(_space) || key(_enter))
            /*fade_off();
            while(fading)
                frame;
            end*/
            delete_text(all_text);
            ya=1;
            if(num_robots>max_robots)
                //exit("­Te has pasado el juego!",0);
            end
            frame;
        end
    end
END

Process Robot(x,y)
PRIVATE
    imagen=0;
    imagenes[3]=2,6,7,8;
    ya_mue;
    esp_frame;

BEGIN
    graph=imagenes[imagen];
    loop
        frame;
        if(collision(type robot))
            graph=3;
            if(!ya_mue)
                muertos++;
                ya_mue=1;
            end
        end
        if(graph<>3)
            esp_frame++;
            if(esp_frame==2)
                esp_frame=0;
                imagen=(imagen+1)mod 3;
                graph=imagenes[imagen];
            end
        end
    end
END

Process mueve_robots()
BEGIN
    for(i=1;i<=num_robots;i++)
        if(robots[i].graph==3)
            muerto[i]=1;
        end
        if(!muerto[i])
            if(robots[i].x<prota.x)
                robots[i].x+=8;
            end
            if(robots[i].x>prota.x)
                robots[i].x-=8;
            end
            if(robots[i].y<prota.y)
                robots[i].y+=8;
            end
            if(robots[i].y>prota.y)
                robots[i].y-=8;
            end
        end
    end
END
