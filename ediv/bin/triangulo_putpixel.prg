PROGRAM colorido;
LOCAL
	i1 , i2 ;
	r , g , b ;
BEGIN


	from i2 = 36 to 165 ;
		for ( i1 = 160 - ((( i2-36 ) * 75 ) / 129 ) ; i1 < 160 + ((( i2-36 ) * 75 ) / 129 ) ; i1++ )
			r = 255 - 2*fget_dist( i1 , i2 , 160 , 36 ) ; if ( r< 0) r = 0 ; end
			g = 255 - 2*fget_dist( i1 , i2 , 85 , 165 ) ; if ( g< 0) g = 0 ; end
			b = 255 - 2*fget_dist( i1 , i2 , 245 , 165 ) ; if ( b< 0) b = 0 ; end
			put_pixel( i1 , i2 ,  rgb(r,g,b) ) ;
		end
	end

	

	loop
		frame;
	end
end