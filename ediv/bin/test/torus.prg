/*
 * TOROIDE EN 3D
 * Adaptado de un ejemplo de PCMan¡a
 * s¡, antes en PCMan¡a hab¡a cosas de programaci¢n... :'(
 *
 * TECLAS:
 * Space = detener/continuar rotaci¢n
 * P     = alternar entre modo puntos/wireframe
 */

Program Torus;
GLOBAL
    Total_puntos;
    Total_lineas;
    donut[500,2];
    Rotado[500,2];
    lineas[1000,1];
    xdeg,ydeg,zdeg;
    xoff,yoff,zoff;
    sen[360],cosen[360];
    dibupun;
    gir=1;

BEGIN
    crea_tablas();
    repeat
        clear_screen();
        gira_Donut(xdeg,ydeg,zdeg);
        pon_lineas(rgb(255,255,0));
        frame;
        if(key(_p))
            dibupun++;
            while(key(_p))
                frame;
            end
        end
        if(key(_space))
            gir++;
            while(key(_space))
                frame;
            end
        end
        if(gir)
            xdeg=(xdeg+3)mod 360;
            ydeg=(ydeg+3)mod 360;
            zdeg=(zdeg+3)mod 360;
        end
    until(key(_enter))
END


Function Line(x1,y1,x2,y2,color)
PRIVATE
    i,deltax,deltay,numpixels;
    d,dinc1,dinc2;
    xx,xinc1,xinc2;
    yy,yinc1,yinc2;

BEGIN
    deltax=abs(x2-x1);
    deltay=abs(y2-y1);
    if(deltax=>deltay)
        numpixels=deltax+1;
        d=(2*deltay)-deltax;
        dinc1=deltay;
        dinc2=(deltay-deltax);
        xinc1=1;
        xinc2=1;
        yinc1=0;
        yinc2=1;
    else
        numpixels=deltay+1;
        d=(2*deltax)-deltay;
        dinc1=deltax;
        dinc2=(deltax-deltay);
        xinc1=0;
        xinc2=1;
        yinc1=1;
        yinc2=1;
    end
    if(x1>x2)
        xinc1=-xinc1;
        xinc2=-xinc2;
    end
    if(y1>y2)
        yinc1=-yinc1;
        yinc2=-yinc2;
    end
    for(i=1;i<=numpixels;i++)
        put_pixel(x1,y1,color);
        if(d<0)
            d+=dinc1;
            x1+=xinc1;
            y1+=yinc1;
        else
            d+=dinc2;
            x1+=xinc2;
            y1+=yinc2;
        end
    end
END


Function Crea_donut(radio1,radio2,partes1,partes2)
PRIVATE
    cont1,cont2;
    circulo[25,2];
    xtemp;

BEGIN
    if(partes1*partes2>500)
        // demasiao p'al body :P
        return;
    end
    total_lineas=0;
    total_puntos=partes1*partes2;
    for(cont1=1;cont1<=partes1;cont1++)
        circulo[cont1,0]=radio1*sen[360*cont1/partes1]+radio2*256;
        circulo[cont1,1]=radio1*cosen[360*cont1/partes1];
    end
    for(cont2=1;cont2<=partes2;cont2++)
        for(cont1=1;cont1<=partes1;cont1++)
            xtemp=circulo[cont1,0];
            donut[(cont2-1)*partes1+cont1,0]=xtemp*cosen[360*cont2/partes2]/256;
            donut[(cont2-1)*partes1+cont1,1]=circulo[cont1,1];
            donut[(cont2-1)*partes1+cont1,2]=xtemp*sen[360*cont2/partes2]/256;
            total_lineas++;
            lineas[total_lineas,0]=(cont2-1)*partes1+cont1;
            lineas[total_lineas,1]=(cont2-1)*partes1+(cont1 mod partes1)+1;
            total_lineas++;
            lineas[total_lineas,0]=(cont2-1)*partes1+cont1;
            lineas[total_lineas,1]=(((cont2-1)*partes1+(cont1)+partes1-1)mod (total_puntos))+1;
        end
    end
END

Function Crea_tablas();
PRIVATE
    xpos,ypos;
    cont;

BEGIN
    xoff=160;
    yoff=100;
    zoff=64;
    total_puntos=0;
    from cont=0 to 360;
        sen[cont]=(256*sin(pi*2*cont/360))/1000;
        cosen[cont]=(256*cos(pi*2*cont/360))/1000;
    end
    crea_donut(5,15,15,15);
END

Function Gira_Donut(Xrot,Yrot,Zrot);
PRIVATE
    cont;
    xtemp;
    ytemp;
    ztemp;

BEGIN
    for(cont=1;cont<=total_puntos;cont++)
        rotado[cont,0]=Donut[cont,0];
        rotado[cont,1]=Donut[cont,1];
        rotado[cont,2]=Donut[cont,2];
        if(xrot<>0)
            ytemp=rotado[cont,1];
            ztemp=rotado[cont,2];
            rotado[cont,1]=(ytemp*cosen[xrot]-ztemp*sen[xrot])/256;
            rotado[cont,2]=(ytemp*sen[xrot]+ztemp*cosen[xrot])/256;
        end
        if(yrot<>0)
            xtemp=rotado[cont,0];
            ztemp=rotado[cont,2];
            rotado[cont,0]=(ztemp*sen[yrot]+xtemp*cosen[yrot])/256;
            rotado[cont,2]=(ztemp*cosen[xrot]-xtemp*sen[yrot])/256;
        end
        if(zrot<>0)
            xtemp=rotado[cont,0];
            ytemp=rotado[cont,1];
            rotado[cont,0]=(xtemp*cosen[zrot]-ytemp*sen[zrot])/256;
            rotado[cont,1]=(xtemp*sen[zrot]+ytemp*cosen[zrot])/256;
        end
    end
END

Function Pon_lineas(col)
PRIVATE
    cont;
    punto1,punto2;
    xfin1,yfin1;
    xfin2,yfin2;

BEGIN
    for(cont=1;cont<=total_lineas;cont++)
        punto1=lineas[cont,0];
        punto2=lineas[cont,1];
        xfin1=xoff+rotado[punto1,0]/(zoff+rotado[punto1,2]/256);
        yfin1=yoff+rotado[punto1,1]/(zoff+rotado[punto1,2]/256);
        xfin2=xoff+rotado[punto2,0]/(zoff+rotado[punto2,2]/256);
        yfin2=yoff+rotado[punto2,1]/(zoff+rotado[punto2,2]/256);
        if(!dibupun)
            line(xfin1,yfin1,xfin2,yfin2,col);
        else
            put_pixel(xfin1,yfin1,col);
        end
    end
END
