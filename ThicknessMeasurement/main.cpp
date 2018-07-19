#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

void makeGrayScale(sf::Image& base, sf::Image& gray, sf::RenderWindow& window);


using uint = unsigned int;

sf::Font font;

int main()
{
	//sf::RenderWindow window(sf::VideoMode(510, 125), "Percent done");
	
	if (!font.loadFromFile("C:\\Windows\\Fonts\\Calibri.ttf"))
	{
		std::cout << "Font not loaded";
		return -1;
	}

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

	makeGrayScale(baseImage,grayScaleImage, window);

	std::string grayImagePath = "G:\\TEST_IMAGES\\Grayscale\\" + imageName;
	grayScaleImage.saveToFile(grayImagePath);

	return 0;
}

void makeGrayScale(sf::Image& base, sf::Image& gray, sf::RenderWindow& window)
{
	sf::Vector2u baseImageSize = base.getSize();
	sf::RectangleShape barBack(sf::Vector2f(500,50));
	barBack.setPosition(5,5);
	barBack.setFillColor(sf::Color::White);


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
