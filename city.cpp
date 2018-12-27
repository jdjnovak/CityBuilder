#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>

#include "city.hpp"
#include "tile.hpp"

double City::distributePool(double &pool, Tile &tile, double rate) {
    const static int moveRate = 4;
	unsigned int maxPop = tile.maxPopPerLevel * (tile.tileVariant + 1);

	if (pool > 0) {
		int moving = maxPop - tile.population;
		if (moving > moveRate) moving = moveRate;
		if (pool - moving < 0) moving = pool;
		pool -= moving;
		tile.population += moving;
	}

	tile.population += tile.population * rate;

	if (tile.population > maxPop) {
		pool += tile.population - maxPop;
		tile.population = maxPop;
	}

	return tile.population;
}

void City::bulldoze(const Tile& tile) {
	for (int pos = 0; pos < this->map.width; ++pos) {
		if (map.selected[pos] == 1) {
			if (this->map.tiles[pos].tileType == TileType::RESIDENTIAL) {
				this->populationPool += this->map.tiles[pos].population;
			} else if (this->map.tiles[pos].tileType == TileType::COMMERCIAL) {
				this->employmentPool += this->map.tiles[pos].population;
			} else if (this->map.tiles[pos].tileType == TileType::INDUSTRIAL) {
				this->employmentPool += this->map.tiles[pos].population;
			}
			this->map.tiles[pos] = tile;
		}
	}
	return;
}

void City::shuffleTiles() {
	while (this->shuffledTiles.size() < this->map.tiles.size()) {
		this->shuffledTiles.push_back(0);
	}
	std::iota(shuffledTiles.begin(), shuffledTiles.end(), 1);
	std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());
	return;
}

void City::tileChanged() {
	this->map.updateDirection(TileType::ROAD);
	this->map.findConnectedRegions({
	    TileType::ROAD,
		TileType::RESIDENTIAL,
		TileType::COMMERCIAL,
		TileType::INDUSTRIAL,
	}, 0);
	return;
}

void City::load(std::string cityName, std::map<std::string, Tile>& tileAtlas) {
	int width = 0;
	int height = 0;

	std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);
	std::string line;

	while (std::getline(inputFile, line)) {

		std::istringstream lineStream(line);
		std::string key;

		if (std::getline(lineStream, key, '=')) {
			std::string value;
			if (std::getline(lineStream, value)) {
				if (key == "width") width = std::stoi(value);
				else if (key == "height") height = std::stoi(value);
				else if (key == "day") this->day = std::stoi(value);
				else if (key == "populationPool") this->populationPool = std::stoi(value);
				else if (key == "employmentpool") this->employmentPool = std::stoi(value);
				else if (key == "population") this->population = std::stoi(value);
				else if (key == "employable") this->employable = std::stoi(value);
				else if (key == "birthRate") this->birthRate = std::stoi(value);
				else if (key == "deathRate") this->deathRate = std::stoi(value);
				else if (key == "residentialTax") this->residentialTax = std::stoi(value);
				else if (key == "commercialTax") this->commercialTax = std::stoi(value);
				else if (key == "industrialTax") this->industrialTax = std::stoi(value);
				else if (key == "funds") this->funds = std::stoi(value);
				else if (key == "earnings") this->earnings = std::stoi(value);
		    } else {
		        std::cerr << "Error, no value for key " << key << std::endl;
			}
		}
	}

	inputFile.close();
	this->map.load(cityName + "_map.dat", width, height, tileAtlas);
	tileChanged();
	return;
}

void City::save(std::string cityName) {
	std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

	outputFile << "width="           << this->map.width << std::endl;
	outputFile << "height="          << this->map.height << std::endl;
	outputFile << "day="             << this->day << std::endl;
	outputFile << "populationPool="  << this->populationPool << std::endl;
	outputFile << "employmentPool="  << this->employmentPool << std::endl;
	outputFile << "population="      << this->population << std::endl;
	outputFile << "employable="      << this->employable << std::endl;
	outputFile << "birthRate="       << this->birthRate << std::endl;
	outputFile << "deathRate="       << this->deathRate << std::endl;
	outputFile << "residentialTax="  << this->residentialTax << std::endl;
	outputFile << "commercialTax="   << this->commercialTax << std::endl;
	outputFile << "industrialTax="   << this->industrialTax << std::endl;
	outputFile << "funds="           << this->funds << std::endl;
	outputFile << "earnings="        << this->earnings << std::endl;

	outputFile.close();
	this->map.save(cityName + "_map.dat");
	return;
}

void City::update(float dt) {
	double popTotal = 0;
	double commercialRevenue = 0;
	double industrialRevenue = 0;

	this->currentTime += dt;
	if (this->currentTime > this->timePerDay) return;

	++day;
	this->currentTime = 0.0;
	if (day % 30 == 0) {
		this->funds += this->earnings;
		this->earnings = 0;
	}

	for (int i = 0; i < this->map.tiles.size(); ++i) {
		Tile& tile = this->map.tiles[this->shuffleTiles[i]];
		if (tile.tileType == TileType::RESIDENTIAL) {
			this->distributePool(this->populationPool, tile, this->birthRate - this->deathRate);
			popTotal += tile.population;
		} else if (tile.tileType == TileType::COMMERCIAL) {
			/* Hire people */
			if (rand() % 100 < 15 * (1.0 - this->commercialTax))
				this->distributePool(this->employmentPool, tile, 0.00);

		} else if (tile.tileType == TileType::INDUSTRIAL) {
			/* Extract resources from the ground */
			if (this->tile.resources[i] > 0 && rand() % 100 < this->population) {
				++tile.production;
				--this->map.resources[i];
			}

			/* Hire people */
			if (rand() % 100 < 15 * (1.0 - this->industrialTax))
				this->distributePool(this->employmentPool, tile, 0.0);
		}
		tile.update();
	}

	/////////////////////////////////////////////
	// STOPPED HERE ON CH 10 from the tutorial //
	/////////////////////////////////////////////
}
