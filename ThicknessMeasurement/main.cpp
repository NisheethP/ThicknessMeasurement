#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>

using uint = unsigned int;

struct Coord
{
	uint x, y;
};

void makeGrayScale(sf::Image& base, sf::Image& gray);
void makeContrast(sf::Image& base, sf::Image& cont);
void GetWidth(sf::Image& base, sf::Image& cont, std::string outputFileName,int threshold=150);



sf::Font font;

int main()
{
	std::string baseImagePath = "G:\\TEST_IMAGES\\Original\\";
	std::string imageName;
	
	bool singleFile = false;
	char x;
	std::cout << "Single file (y/n): ";
	std::cin >> x;

	singleFile = ((x=='y') || x=='Y');

	int initNumber;
	int finalNumber;

	if (!singleFile)
	{
		std::cout << "\nEnter the initial file number: ";
		std::cin >> initNumber;

		std::cout << "\nEnter the final file number: ";
		std::cin >> finalNumber;

		int threshold = 150;
		std::cout << "\nEnter the threshold for the intensity of the edge: ";
		std::cin >> threshold;

		for (int i = initNumber; i <= finalNumber; i++)
		{
			for (int j = 1; j <= 3; j++)
			{
				baseImagePath = "G:\\TEST_IMAGES\\Original\\";
				imageName = std::to_string(i) + "-" + std::to_string(j);
				baseImagePath += imageName + ".jpg";
				sf::Image baseImage, grayScaleImage, contrastImage;
				if (!baseImage.loadFromFile(baseImagePath))
				{
					std::cout << "Image loading failed";
					//return -1;
				}

				if (!contrastImage.loadFromFile(baseImagePath))
				{
					std::cout << "Image loading failed";
					//return -1;
				}

				GetWidth(baseImage, contrastImage, imageName, threshold);
				std::string contrastImagePath = "G:\\TEST_IMAGES\\Final\\" + imageName + ".jpg";
				contrastImage.saveToFile(contrastImagePath);
			}
		}
	}
	else if (singleFile)
	{
		std::cout << "Enter File Name (without extention):  ";
		std::cin >> imageName;
		baseImagePath += imageName + ".jpg";
		int threshold = 100;
		std::cout << "\nEnter the threshold for the intensity of the edge: ";
		std::cin >> threshold;
		
		sf::Image baseImage, grayScaleImage, contrastImage;
		if (!baseImage.loadFromFile(baseImagePath))
		{
			std::cout << "Image loading failed";
			//return -1;
		}

		if (!contrastImage.loadFromFile(baseImagePath))
		{
			std::cout << "Image loading failed";
			//return -1;
		}

		GetWidth(baseImage, contrastImage, imageName, threshold);
		std::string contrastImagePath = "G:\\TEST_IMAGES\\Final\\" + imageName + ".jpg";
		contrastImage.saveToFile(contrastImagePath);		
	}

	return 0;
}

void makeGrayScale(sf::Image& base, sf::Image& gray)
{
	sf::Vector2u baseImageSize = base.getSize();
	
	//sf::RectangleShape barBack(sf::Vector2f(500,50));
	//barBack.setPosition(5,5);
	//barBack.setFillColor(sf::Color::White);


	gray.create(baseImageSize.x, baseImageSize.y, sf::Color::White);

	//uint imageSize = baseImageSize.x * baseImageSize.y;
	//double percentCompleted = 0.f;

	for (uint i = 0; i < baseImageSize.x; i++)
	{
		for (uint j = 0; j < baseImageSize.y; j++)
		{
			sf::Color pix = base.getPixel(i, j);
			sf::Uint8 grayCol = static_cast<sf::Uint8>(pix.r * 0.299 + pix.g* 0.587 + pix.b* 0.114);
			sf::Color newPix = { grayCol, grayCol, grayCol};

			gray.setPixel(i, j, newPix);
		
			/*percentCompleted = static_cast<double>(i * baseImageSize.x + j);
			percentCompleted /= imageSize;
			std::string percStr = std::to_string(percentCompleted) + "%";
			sf::String str(percStr);

			sf::Text percs(str, font, 25);
			percs.setPosition(105,10);
			percs.setFillColor(sf::Color::Cyan);

			window.clear(sf::Color::Black);
			window.draw(barBack);
			window.draw(percs);
			window.display();*/
		}
	}
}

void makeContrast(sf::Image& base, sf::Image& cont)
{
	sf::Vector2u baseImageSize = base.getSize();
	cont.create(baseImageSize.x, baseImageSize.y-1, sf::Color::White);

	for (uint i = 0; i < baseImageSize.x; i++)
	{
		for (uint j = 0; j < baseImageSize.y-1; j++)
		{
			sf::Color pix = base.getPixel(i, j);
			sf::Color nextPix = base.getPixel(i, j+1);
			double Contrast = 0;
			Contrast = nextPix.r - pix.r;
			Contrast /= pix.r;

			sf::Uint8 contCol = static_cast<sf::Uint8>(255.f*Contrast);
			sf::Color newPix = { contCol, contCol, contCol };

			cont.setPixel(i, j, newPix);

		}
	}
}

void GetWidth(sf::Image& base, sf::Image& cont, std::string outputFileName, int threshold)
{
	sf::Vector2u baseImageSize = base.getSize();
	int pixels = 0;
	bool isPixelBright = false;

	std::string path = "G:\\TEST_IMAGES\\Thickness\\" + outputFileName + ".txt";

	std::ofstream file(path);
	Coord initCoord = { 0,0 }, finalCoord = {0,0};
	
	std::stringstream stream;


	if (file.is_open())
	{
		for (uint i = 0; i < baseImageSize.x; i++)
		{
			stream.str("");

			initCoord = { 0,0 };
			finalCoord = { 0,0 };
			pixels = 0;
			bool lineBegun = false;
			bool lineEnd = false;
			bool initMarked = false;
			bool endMarked = false;
			
			for (uint j = 0; j < baseImageSize.y; j++)
			{
				sf::Color pix = base.getPixel(i, j);

				if (!lineBegun)
				lineBegun = (pix.r > threshold);
				
				isPixelBright = pix.r > threshold;

				if (lineBegun )
					pixels++;

				if (lineBegun && !initMarked)
				{
					cont.setPixel(i, j, sf::Color::Red);
					initMarked = true;
					initCoord = { i,j };
				}

				if (lineBegun && pixels > 100)
				{
					lineBegun = !(pix.r > threshold);
					if (isPixelBright && !endMarked)
					{
						cont.setPixel(i, j, sf::Color::Cyan);
						endMarked = true;
						finalCoord = { i,j };
					}
				}
			}

			if (finalCoord.x != 0 || finalCoord.y != 0)
			{
				stream.width(5);
				stream << initCoord.x;
				stream << "  ";

				stream.width(5);
				stream << initCoord.y;
				stream << '\t';

				stream.width(5);
				stream << finalCoord.x;
				stream << "  ";

				stream.width(5);
				stream << finalCoord.y;
				stream << "\t\t";

				stream.width(5);
				stream << pixels;
				stream << std::endl;

				file << stream.str();
			}
		}
	}

	else
		std::cout << "File not open";

	file.close();
	
}