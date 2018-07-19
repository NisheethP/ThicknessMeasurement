#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

int main()
{
	std::string imagePath;
	std::cin >> imagePath;

	sf::Image baseImage, grayScaleImage, contrastImage;
	if (!baseImage.loadFromFile(imagePath))
	{
		std::cout << "Image loading failed";
		return -1;
	}

	return 0;
}