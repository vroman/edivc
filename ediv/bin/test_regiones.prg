PROGRAM regiones;
GLOBAL
	grafico ;
	cuad ;
	tras = 0 ;
LOCAL
	i1 , i2 ;
BEGIN
	define_region(1 , 0 , 0 , 200 , 200 ) ;
	region = 1 ;
//	cuad = draw( 3 , rgb(0,255,0) , 14 , 0 , 10 , 10 , 20 , 20 ) ;
	graph = load_bmp("sion.bmp") ;
	file = load_fpg("test.fpg") ;
	put_screen(file,1) ;
	detras() ;
	//graph = 1 ;
	LOOP
	
//		tras = (tras+1) % 16 ;
//		if ( tras > 15 ) tras = 0 ; end
//		move_draw( cuad , rgb(0,255,0) , tras , 10+tras , 10+tras*2 , 20+tras*2 , 20+tras*4 ) ;	
		if ( key(_right)) x+=5 ; end
		if ( key(_left)) x-=5 ; end
		if ( key(_up)) y-= 5 ; end
		if ( key(_down)) y+=5 ; end
//		if ( key(_d)) delete_draw( cuad ) ; end
		FRAME;
	END
END

PROCESS detras()
BEGIN

	graph = load_bmp("ediv.bmp") ;
	x = 160 ;
	y = 100 ;
	z = 10 ;
	LOOP
		FRAME;
	END
END
