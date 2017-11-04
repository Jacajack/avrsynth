clear

fcpu = 16000000;
prescaler = 1; 
samplesize = 16;
noterange = 1:128;

//Calculate timer frequency
ftimer = fcpu / prescaler;

//Calculate note frequencies
fnote = [];
for i = noterange do
   fnote( i ) = ( 2 ^ ( 1 / 12 ) ) ^ ( i - 70 ) * 440;
end

//Calculate comparator values
compv = [];
for i = noterange do
    compv( i ) = ftimer / fnote( i ) / samplesize;
    if compv( i ) > 65535 then
    	compv( i ) = 65535;
    end
end


//Print data in form of C array
printf( "\n{\n" );
for i = noterange do
    printf( "\t%6du, //[%03d] - %0.2fHz\n", round( compv( i ) ), i, fnote( i ) );  
end
printf( "}\n" );

exit
