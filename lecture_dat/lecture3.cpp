/**
\file
\brief import du code Yumain
*/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#define qAbs std::abs

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

struct YumainQt
{
	int extractVal(int val, int a, int b);
//	int* SpikeDecode(QByteArray Data);
	int* SpikeDecode(uint8_t* Data);
	bool readNextFrame( std::ifstream& );
};
/*

void MainWindow::on_pushButton_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load data file"), "",
            tr("data (*.dat);;All Files (*)"));

    dataFile = new QFile(fileName);
    dataFile->open(QIODevice::ReadOnly);

    dataStream = new QDataStream(dataFile);
    dataStream->setByteOrder(QDataStream::LittleEndian);
    currentPosFile = 1;

     continueRead = true;
     dataFile->seek(0);
     image = new QImage(1920, 1080, QImage::Format_RGB32);
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(display()));
     timer->start(1000/currentFPS); //time specified in ms
     readNextFrame();
}
*/


int YumainQt::extractVal(int val, int a, int b)
{
    int r = 0;
    for (int i = a; i <= b; i++)
        r |= 1 << i;

    return (r & val) >> (a);
}

int* YumainQt::SpikeDecode(uint8_t* Data)
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

bool YumainQt::readNextFrame( std::ifstream& dataFile )
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
                std::cout << "newFrame 1\n";

			uint8_t d = data;
            int* Spike = SpikeDecode(d);

            if (Spike[2] == 1) //Display Image
            {
                std::cout << "newFrame 2\n";
                newFrame = true;
            }
            else {
                x           = Spike[0];
                y           = Spike[1];
                orientation = Spike[5];
                sign        = Spike[4];
                amplitude   = Spike[3];

                if(x<1920 && y<1080)
                {
                    if(sign ==1){// OFF data
                      colors = 1; //offColor;
                    }
                    else{ //ON data
                      colors = 0; //onColor;
                    }
                }
//                image->setPixel(x, y, colors[orientation]);
				std::cout << "x,y=" << x << "," << y << " col=" << colors << '\n';
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

}
