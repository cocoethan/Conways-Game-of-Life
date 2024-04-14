// Standard libraries
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>

// External libraries
#include <SFML/Graphics.hpp>

// Constants
const int windowWidth = 800;
const int windowHeight = 600;
const int cellSize = 2;
const int gridWidth = windowWidth / cellSize;
const int gridHeight = windowHeight / cellSize;
static int numAlive = 15000; // Number of 'alive' cells at initialization
static int pauseTime = 50; // (In milliseconds)

// Function to initialize the grid with random states
void initializeGrid(std::vector<std::vector<bool>>& grid, int numAlive) {
    for (int i = 0; i < gridHeight; ++i) { // Initialize all cells to 'dead'
        for (int j = 0; j < gridWidth; ++j) {
            grid[i][j] = false;
        }
    }

    for (int k = 0; k < numAlive; ++k) { // Randomly set specified number of cells to 'alive'
        int randRow = rand() % gridHeight;
        int randCol = rand() % gridWidth;
        grid[randRow][randCol] = true;
    }
}

// Function to count 'alive' neighbors for a given cell
int countAliveNeighbors(const std::vector<std::vector<bool>>& grid, int row, int col) {
    int count = 0; // Number of 'alive' neighboring cells 

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) { // If cell is the middle cell
                continue; // Skip cell
            }
            int r = row + i; // Get neighboring cell's row
            int c = col + j; // Get neighboring cell's column
            if (r >= 0 && r < gridHeight && c >= 0 && c < gridWidth && grid[r][c]) { // If neighboring cell is in grid and 'alive'
                ++count;
            }
        }
    }
    return count;
}

// Function to update the grid based on 'Conway's Game of Life' rules
void updateGrid(std::vector<std::vector<bool>>& grid) {
    std::vector<std::vector<bool>> newGrid(gridHeight, std::vector<bool>(gridWidth, false)); // New grid object

    for (int i = 0; i < gridHeight; ++i) {
        for (int j = 0; j < gridWidth; ++j) {
            int liveNeighbors = countAliveNeighbors(grid, i, j); // Number of 'alive' neighboring cells
            if (grid[i][j]) { // If current cell is alive
                if (liveNeighbors == 2 || liveNeighbors == 3) {
                    newGrid[i][j] = true; // Keep cell 'alive'
                }
                else {
                    newGrid[i][j] = false; // Make cell 'dead'
                }
            }
            else { // Current cell is dead
                if (liveNeighbors == 3) {
                    newGrid[i][j] = true; // Make cell 'alive'
                }
                else {
                    newGrid[i][j] = false; // Keep cell 'dead'
                }
            }
        }
    }

    grid = newGrid; // Set grid to new grid
}

// Function to draw the grid in the SFML window
void drawGrid(sf::RenderWindow& window, const std::vector<std::vector<bool>>& grid) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize)); // Square shape object (i.e. cell)

    window.clear(sf::Color::White); // Background color

    for (int i = 0; i < gridHeight; ++i) {
        for (int j = 0; j < gridWidth; ++j) {
            if (grid[i][j]) {
                cell.setPosition(j * cellSize, i * cellSize); // Set current cell's position
                cell.setFillColor(sf::Color::Black); // Set current cell to the 'alive' state's color
                window.draw(cell); // Draw current cell
            }
        }
    }

    window.display();
}

// Main function
int main() {

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Conway's Game of Life"); // SFML window object

    std::vector<std::vector<bool>> grid(gridHeight, std::vector<bool>(gridWidth, false)); // Grid object

    initializeGrid(grid, numAlive); // Initializes the grid

    while (window.isOpen()) { // Main loop
        
        sf::Event windowEvent;

        while (window.pollEvent(windowEvent)) {
            if (windowEvent.type == sf::Event::Closed) {
                window.close();
            }
        }

        updateGrid(grid); // Updates the next grid

        drawGrid(window, grid); // Draws new grid

        sf::sleep(sf::milliseconds(pauseTime)); // Pauses time between grids
    }

    return 0;
}
