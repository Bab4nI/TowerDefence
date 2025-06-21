#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    
    std::string path = "assets\\images\\cute_cat.jpg";
    if (!std::filesystem::exists(path)) {
        std::cerr << "File not found: " << std::filesystem::absolute(path) << std::endl;
    }

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("assets\\images\\cute_cat.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
 
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("assets\\fonts\\HARLOWSI.TTF"))
        return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);
 
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("assets\\audio\\g1.mp3"))
        return EXIT_FAILURE;
 
    // Play the music
    music.play();
 
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
 
        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        window.draw(text);
 
        // Update the window
        window.display();
    }
 
    return EXIT_SUCCESS;
}