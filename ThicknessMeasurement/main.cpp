#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

void makeGrayScale(sf::Image& base, sf::Image& gray, sf::RenderWindow& window);


using uint = unsigned int;

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 100), "Percent done");
	

	std::string baseImagePath = "G:\\TEST_IMAGES\\Original\\";
	std::string imageName;
	std::cout << "Enter File Name (with extention):  ";
	std::cin >> imageName;
	baseImagePath += imageName;

	sf::Image baseImage, grayScaleImage, contrastImage;
	if (!baseImage.loadFromFile(baseImagePath))
	{
		std::cout << "Image loading failed";
		return -1;
	}

	sf::Vector2u baseImageSize = baseImage.getSize();

	makeGrayScale(baseImage,grayScaleImage);

	std::string grayImagePath = "G:\\TEST_IMAGES\\Grayscale\\" + imageName;
	grayScaleImage.saveToFile(grayImagePath);

	return 0;
}

void makeGrayScale(sf::Image& base, sf::Image& gray, sf::RenderWindow& window)
{
	sf::Vector2u baseImageSize = base.getSize();

	gray.create(baseImageSize.x, baseImageSize.y, sf::Color::White);

	uint imageSize = baseImageSize.x * baseImageSize.y;
	double percentCompleted = 0.f;
	for (uint i = 0; i < baseImageSize.x; i++)
	{
		for (uint j = 0; j < baseImageSize.y; j++)
		{
			sf::Color pix = base.getPixel(i, j);
			sf::Uint8 grayCol = static_cast<sf::Uint8>(pix.r * 0.299 + pix.g* 0.587 + pix.b* 0.114);
			sf::Color newPix = { grayCol, grayCol, grayCol};

			gray.setPixel(i, j, newPix);
			percentCompleted = static_cast<double>(i * baseImageSize.x + j);
			percentCompleted /= imageSize;

			window.clear(sf::Color::Black);
		}
	}
}
