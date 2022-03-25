/**
Lecture de fichier dat (préliminaire)

build: make
(aucune dépendance autre que compilo récent)

A tester avec:
./lecture1 2022_03_04-17_14_27_069_caiman_0_data.dat

va générer:

 - reading 5 spikes
Spike bytes: 4a-0-0-0-
lecture de 4a000000
1: x,y=0,1280
Spike bytes: 8a-3e-fb-0-
lecture de 8a3efb00
2: x,y=1004,1311
Spike bytes: 4a-b-2b-1-
lecture de 4a0b2b01
3: x,y=1196,1285
Spike bytes: ca-36-5f-1-
lecture de ca365f01
4: x,y=380,1307
Spike bytes: ca-36-61-1-
lecture de ca366101
5: x,y=388,1307

=> ça me parait pas très cohérent...
*/

#include <fstream>

#include "spike.hpp"
/**
\verbatim
    Stockage du spike sous la forme d'un mot de 32 bits:

        Spike(0) => Flag de nouvelle Image, sépare les Spikes issues des différentes images. Si ce bit est à "1", cela indique le changement d'image au cours de l'acquisition.
        Spike(1) => Signe du Spike. indique si un éclaircissement ou un assombrissement du pixel à eu lieu
        Spike (5 downto 2) => indice du Filtre déclenchés

            "0000" => filtre 0°
            "0001" => filtre + 45°
            "0010" => filtre +90°
            "0011" => filtre -45°

        Spike (16 downto 6) => Position X du Spike dans l’image

        Spike(27 downto 17) => Position Y du Spike dans l’image
        Spike(31 downto 28) => Non utilisé pour le moment, ce champs est toujours lus en tant que "1111"
\endverbatim
*/



/// arg1: nom fichier, arg2 (optionnel): nombre de spikes à lire
int main( int argc, char** argv )
{
	if( argc < 2 )
	{
		std::cout << "Error: missing filename\n";
		return 1;
	}
	size_t nbSpikes{5};
	bool readAll{false};
	if( argc == 3 )
	{
		if( std::string{ argv[2]} == "a" ) // read all
			readAll = true;
		else
			nbSpikes = std::atoi( argv[2] );
	}

	if( readAll )
		std::cout << "- reading whole file\n";
	else
		std::cout << "- attempt reading " << nbSpikes << " spikes\n";

	std::ifstream f( argv[1], std::ios::binary );
    if( !f.is_open() )
	{
		std::cout << "Error: failed to open input file " << argv[1] << '\n';
		return 1;
	}

	std::ofstream fout( "out.dat" );
	if( !fout.is_open() )
	{
		std::cout << "Error: failed to open output file\n";
		return 2;
	}

	fout << "# frame;index;orientation;x;y\n";


	size_t c=0;
	char buf[4];
	do
	{
		c++;
		f.read( buf, 4 );
//		std::cout << "buf" <<
		Spike sp( buf );
		fout << sp;
		if( sp._frameHasChanged )
		{
            fout2.close();
            std::ostringstream ssfn;
            ssfn << "data/out_" << std::setw(5) << set::setf('0') << sp._frame;
            fout2.open( ssfn.str() );
		}
	}
	while( (c != nbSpikes | readAll) && !f.eof() );

	fout << "# end, read " << c << " spikes in " << Spike::s_frame << " frames\n";

	std::cout << "- read " << c << " spikes in " << Spike::s_frame << " frames\n";

}
