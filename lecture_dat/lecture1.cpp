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

#include <iostream>
#include <fstream>

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

using uchar = unsigned char;

class Spike
{
private:
	static size_t _index; ///< index du spike
	size_t _frameIndex;   ///< préliminaire, pas utilisé ici
	size_t _posX,_posY;

public:
	Spike( char* buf )
	{
		std::cout << "Spike bytes: " << std::hex;
		for( int i=0; i<4; i++ )
			std::cout << (0xff&buf[i]) << '-'; // masquage pour l'affichage uniquement
		std::cout << '\n';

/*		for( int i=0; i<4; i++ )
			std::cout << std::dec << i << ":" << (3-i)*8 << ":" << std::hex <<(int32_t(buf[i]) << ((3-i)*8)) << '-';
		std::cout << '\n';
*/
		uint32_t value = 0;
		unsigned char buf2[4];
		for( int i=0; i<4; i++ )
		{
			buf2[i] = buf[i];
			value |= ( buf2[i] << (3-i)*8 );  // décalage vers la gauche de 24, 16, 8, 0 et OU logique pour insérer dans le mot 32 bits
		}
        _index++;
		std::cout << "lecture de " << std::hex << value << '\n';
/// x: bit 16 à 6: 0000 0000 0000 0001 1111 1111 1100 0000 = 0x00.01.FF.c0
        _posX = value & 0x0001ffc0; // masquage
        _posX = _posX >> 6; // décalage

/// y: bit 27 à 17: 0000 1111 1111 1110 0000 0000 0000 0000 = 0x0F.FE.00.00
        _posY = value & 0x0FFE0000; // masquage
        _posY = _posY >> 17; // décalage
	}

	friend std::ostream& operator << ( std::ostream& f, Spike sp )
	{
		f << std::dec << sp._index << ": x,y="
			<< sp._posX << "," << sp._posY << '\n';
		return f;
	}
};

size_t Spike::_index = 0;

/// arg1: nom fichier, arg2 (optionnel): nombre de spikes à lire
int main( int argc, char** argv )
{
	if( argc < 2 )
	{
		std::cout << "Error: missing filename\n";
		return 1;
	}
	size_t nbSpikes = 5;
	if( argc == 3 )
		nbSpikes = std::atoi( argv[2] );
	std::cout << " - reading " << nbSpikes << " spikes\n";

	std::ifstream f( argv[1], std::ios::binary );
    if( !f.is_open() )
	{
		std::cout << "Error: failed to open file\n";
		return 1;
	}

	size_t c=0;
	char buf[4];
	do
	{
		c++;
		f.read( buf, 4 );
		Spike sp( buf );
		std::cout << sp;
	}
	while( c != nbSpikes && !f.eof() );
}
