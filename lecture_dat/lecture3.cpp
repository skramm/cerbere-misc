/**
\file
\brief tentative d'import du code Yumain
*/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#define qAbs std::abs


// globals

size_t g_eventIdx = 0;
size_t g_frameIdx = 0;
bool g_switchFrame = false;
char sep{' '};

struct Event
{
    int x, y, orientation, sign, amplitude;

	friend std::ostream& operator << ( std::ostream& f, Event sp )
	{
		f << std::dec
			<< g_frameIdx << sep
			<< g_eventIdx << sep
			<< sp.x << sep
			<< sp.y << sep
			<< sp.orientation << sep
			<< sp.sign << sep
			<< sp.amplitude
			<< '\n';
		return f;
	}
};

struct YumainQt
{
	int extractVal(int val, int a, int b);
//	int* SpikeDecode(QByteArray Data);
	int* SpikeDecode(char* Data);
	bool readNextFrame( std::ifstream&, Event&, std::ofstream& );
};


int YumainQt::extractVal(int val, int a, int b)
{
    int r = 0;
    for (int i = a; i <= b; i++)
        r |= 1 << i;

    return (r & val) >> (a);
}

int* YumainQt::SpikeDecode(char* Data)
{
    int* ListeSpike = new int[6];

    uint32_t currentData = 0;
    for (int indData = 0; indData < 4; indData++)
    {
        uint32_t a = ((uint8_t)(Data[indData]) << (8 * indData));
        currentData = currentData | a;
    }
    ListeSpike[0] = qAbs(extractVal(currentData, 6, 16));   // X value
    ListeSpike[1] = qAbs(extractVal(currentData, 17, 27));  // Y value
    ListeSpike[5] = qAbs(extractVal(currentData, 2, 5)); // orientation
    ListeSpike[3] = qAbs(extractVal(currentData, 28, 31)); // Amplitude
    ListeSpike[4] = qAbs(extractVal(currentData, 1, 1)); // sign
    ListeSpike[2] = qAbs(extractVal(currentData, 0, 0)); // new frame

    return ListeSpike;
}

bool YumainQt::readNextFrame( std::ifstream& dataFile, Event& ev, std::ofstream& fout )
{
//    QRgb* colors;
    int colors;
    bool newFrame = false;
    int readLength = 4;
    int x, y, orientation, sign, amplitude;

//    QByteArray data;
//    uint8_t data[4];
    char data[4];
//    image->fill(qRgb(127, 127, 127));


    while(!newFrame)
	{
		//            data=dataFile->read(readLength);
		dataFile.read( data, readLength);
		//            qDebug() << "new frame";
		//				std::cout << "- start new Frame\n";

		//			uint8_t d = data;
		int* Spike = SpikeDecode(data);

		if (Spike[2] == 1) //Display Image
		{
			newFrame = true;
			g_frameIdx++;
			std::cout << "- switch to new Frame: " << g_frameIdx << ", previous had " << g_eventIdx << "events\n";
			g_eventIdx = 0;

		}
		else
		{
			ev.x           = Spike[0];
			ev.y           = Spike[1];
			ev.orientation = Spike[5];
			ev.sign        = Spike[4];
			ev.amplitude   = Spike[3];

			if(x<1920 && y<1080)
			{
				if(sign ==1)
				{// OFF data
					colors = 1; //offColor;
				}
				else
				{ //ON data
					colors = 0; //onColor;
				}
			}
				//                image->setPixel(x, y, colors[orientation]);
			fout << ev;
		}
		/*            if(data.length()<readLength)
		{
		return false;
		}
		*/
	}
	return true;
}



int main( int argc, char** argv )
{
	if( argc < 2 )
	{
		std::cout << "Error: missing filename\n";
		return 1;
	}
	size_t nbSpikes{5};
	bool readAll{false};
	if( argc > 2 )
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

	bool saveSeparateFiles{false};
	if( argc > 3 )
	{
		if( std::atoi( argv[3] ) == 1 )
			saveSeparateFiles = true;
	}

	std::ifstream datafile( argv[1], std::ios::binary );
    if( !datafile.is_open() )
	{
		std::cout << "Error: failed to open input file " << argv[1] << '\n';
		return 1;
	}

	std::ofstream fout( "out.dat" );
	if( !fout.is_open() )
	{
		std::cout << "Error: failed to open output file out.dat\n";
		return 2;
	}

	std::ofstream fout2;
	if( saveSeparateFiles )
	{
		fout2.open( "out_data/out_0.dat" );
		if( !fout2.is_open() )
		{
			std::cout << "Error: failed to open output file data/out_0.dat\n";
			return 3;
		}
	}

	YumainQt yu;
	size_t c=0;
//	char buf[4];
	Event event;
	do
	{
		std::cout << "main loop c=" << c << '\n';
		c++;
		yu.readNextFrame( datafile, event, fout );
//		f.read( buf, 4 );
//		std::cout << "buf" <<
//		Spike sp( buf );
//		fout << sp;

		if( saveSeparateFiles )
		{
            fout2.close();
            std::ostringstream ssfn;
            ssfn << "out_data/out_" << std::setw(5) << std::setfill('0') << g_frameIdx;
            fout2.open( ssfn.str() );
		}
		fout2 << event;
	}
	while( (c != nbSpikes | readAll) && !datafile.eof() );

//	fout << "# end, read " << c << " spikes in " << Spike::s_frame << " frames\n";

	std::cout << std::dec << "- read " << c << " spikes in " << g_frameIdx << " frames\n";

}

