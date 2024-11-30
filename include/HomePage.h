#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <SFML/Graphics.hpp>
#include <string>

class HomePage {
    public:
      HomePage(sf::RenderWindow *window);
      void display();
      bool isContinueButtonClicked();

    private:
      void initialize();
      sf::RenderWindow *window;
      sf::Text title1;
      sf::Text title2;
      sf::Text description;
      sf::RectangleShape continueButton;
      sf::Text continueButtonText;

      sf::Font font;
};



#endif //HOMEPAGE_H
