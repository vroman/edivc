PROGRAM colorido;
LOCAL
	i1 , i2 ;
	r , g , b ;
	fondo ;
	vx , vy ;
	angulo ;
	graf_barra[200] ;

BEGIN
	rand_seed( 0 ) ;

	from i2 = 36 to 165 ;
		for ( i1 = 160 - ((( i2-36 ) * 75 ) / 129 ) ; i1 < 160 + ((( i2-36 ) * 75 ) / 129 ) ; i1++ )
			r = 255 - 2*fget_dist( i1 , i2 , 160 , 36 ) ; if ( r< 0) r = 0 ; end
			g = 255 - 2*fget_dist( i1 , i2 , 85 , 165 ) ; if ( g< 0) g = 0 ; end
			b = 255 - 2*fget_dist( i1 , i2 , 245 , 165 ) ; if ( b< 0) b = 0 ; end
			put_pixel( i1 , i2 ,  rgb(r,g,b) ) ;
		end
		FRAME;
	end
	
	fondo = new_map( 320 , 200 , 0 , 0 , 0 ) ;
	screen_copy( 0 , 0 , fondo , 0 , 0 , 320 , 200 ) ;
	map_block_copy( 0 , graph , 0 , 0 , fondo , 85 , 36 , 150 , 126 ) ;
        clear_screen() ;
	angle = 1500 ;
	from i1 = 0 to 199 ;
		angle+= 3000 ;
			graf_barra[i1] = new_map( 320 , 1 , 0 , 0 , 0 ) ;
			map_block_copy( 0 , graf_barra[i1] , 0 , 0 , fondo , 0 , i1*1 , 320 , 1 ) ;
	end

	vx = rand( -3 , 3 ) ;
	vy = rand( -3 , 3 ) ;	

	flags = 4 ;
	transparency = 128 ;		

	loop
		angle+= 15000 ;
		from i1 = 0 to 199 ;
			put( 0 , graf_barra[i1] , cos(angle+i1*6000) / 50 , i1 *1 ) ;
		end

		frame;
                clear_screen() ;
	end
end


PROCESS barra( graph , angle , y ) ;
BEGIN
	
	LOOP
		angle += 15000 ;
		x = cos(angle+i1*30000) / 50 ;
		FRAME;
	END
END
