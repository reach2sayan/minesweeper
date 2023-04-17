#include "minesweeper.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iterator>

struct RNG {
    int operator() (int n) {
        return std::rand() / (1.0 + RAND_MAX) * n;
    }
};

using std::cout, std::cin, std::endl;
Minesweeper::Minesweeper(int _numRows, int _numColumns, int _numMines){

	numRows = _numRows;
	numColumns = _numColumns;
	numMines = _numMines;
	int totalBlocks = numRows*numColumns;

	//randomly set-up mines
	std::vector<int> temp(totalBlocks);
	std::iota(temp.begin(), temp.end(), 1);
	std::srand(2904);
	std::random_shuffle(temp.begin(), temp.end(), RNG());

	//set-up locations
	for (int i=0; i < numRows; i++){
		for (int j=0; j < numColumns; j++){
			int linearIndex = i*numColumns + j;
			location currLocation;
			currLocation.rowIndex = i;
			currLocation.columnIndex = j;
			currLocation.isOpen = false;
			currLocation.isMarkedBomb = false;
			currLocation.isMarkedDoubt = false;

			//assign mines
			temp.erase(temp.begin()+numMines, temp.end());
			std::vector<int>::iterator it = find(temp.begin(), temp.end(), linearIndex);
			if (it != temp.end()) 
				currLocation.isMine = true;
			else
				currLocation.isMine = false;

			locations.push_back(currLocation);
		}
	}
}

int Minesweeper::countMinesAround(const location &loc) const {

	int numMinesAround = 0;
	int rowIndex = loc.rowIndex;
	int columnIndex = loc.columnIndex;

	for (int i = -1; i <= 1; i++){
		for (int j = -1; j <= 1; j++){
			int neighborRowIndex = rowIndex-i;
			int neighborColumnIndex = columnIndex-j;
			if ((neighborRowIndex < 0) || (neighborRowIndex >= numRows) || (neighborColumnIndex < 0) || (neighborColumnIndex >= numColumns))
				continue;
			else{
				int neighborLinearIndex = (rowIndex-i)*numColumns + (columnIndex-j);
				if ((i == 0) && (j == 0))
					continue;
				else if (locations[neighborLinearIndex].isMine) numMinesAround++;
			}
		}
	}
	return numMinesAround;
}

void Minesweeper::openLocation(location &loc){

	if (loc.isOpen || loc.isMarkedBomb || loc.isMarkedDoubt)
		return;
	else{
		int minesAround = countMinesAround(loc);
		loc.isOpen = true;
		int rowIndex = loc.rowIndex;
		int columnIndex = loc.columnIndex;

		if (minesAround == 0){
			for (int i = -1; i <= 1; i++){
				for (int j = -1; j <= 1; j++){
					int neighborRowIndex = rowIndex-i;
					int neighborColumnIndex = columnIndex-j;
					int neighborLinearIndex = neighborRowIndex*numColumns + neighborColumnIndex;
					if ((neighborRowIndex < 0) 
							|| (neighborRowIndex >= numRows) 
							|| (neighborColumnIndex < 0) 
							|| (neighborColumnIndex >= numColumns) 
							|| ((i == 0) && (j == 0)))
						continue;
					else 
						openLocation(locations[neighborLinearIndex]);
				}
			}
		}
		else
			return;
	}
}

bool Minesweeper::onlyMinesLeft() const{ 
	return (std::all_of(locations.cbegin(), locations.cend(), 
				[](const location &loc) {
				return (loc.isOpen || loc.isMarkedBomb);
				}));
}

void Minesweeper::displayBoard() const{

	cout << "  ";
	for (int i=0; i < numColumns; i++)
		cout << i << " ";
	cout << endl;
	for (int i=0; i < numRows; i++){
		cout << i << " ";
		for (int j=0; j < numColumns; j++){
			int linearIndex = i*numColumns + j;
			location loc = locations[linearIndex];
			int minesAround = countMinesAround(loc);
			if ((!loc.isOpen) && (!loc.isMarkedBomb) && (!loc.isMarkedDoubt))
				cout << "\u25A0 ";
			else if (loc.isMarkedDoubt) cout << "x ";
			else if (loc.isMarkedBomb) cout << "\u25CF ";
			else if (minesAround == 0) { cout << "\u25A1 "; }
			else { cout << minesAround << " "; }
		}
		cout << endl;
	}
	cout << endl;
}

void Minesweeper::displayAll() const{

	cout << "  ";
	for (int i=0; i < numColumns; i++)
		cout << i << " ";
	cout << endl;
	for (int i=0; i < numRows; i++){
		cout << i << " ";
		for (int j=0; j < numColumns; j++){
			int linearIndex = i*numColumns + j;
			location loc = locations[linearIndex];
			if (loc.isMine) cout << "\u2639 ";
			else cout << "\u25A1 ";
		}
		cout << endl;
	}
	cout << endl;
}

void Minesweeper::markAsDoubt(const int x, const int y){
	if (!locations[x*numRows +y].isOpen)
		locations[x*numRows + y].isMarkedDoubt = true;
}
void Minesweeper::markAsBomb(const int x, const int y){
	if (!locations[x*numRows +y].isOpen)
		locations[x*numRows + y].isMarkedBomb = true;
}
void Minesweeper::unmarkLocation(const int x, const int y){ 
	if (locations[x*numRows +y].isMarkedDoubt || locations[x*numRows +y].isMarkedBomb){
		locations[x*numRows + y].isMarkedDoubt = false;
		locations[x*numRows +y].isMarkedBomb = false;
	}
}

int main(){
	int rows, columns, mines;
	cout << "Enter No. of Rows: ";
	cin >> rows;

	cout << "Enter No. of Columns: ";
	cin >> columns;

	cout << "Enter No. of Mines: ";
	cin >> mines;

	Minesweeper game(rows,columns,mines);
	std::system("clear");

	int x,y;
	char mark;
	while (true){
		game.displayBoard();
		cout << "Enter (S,x,y) location to open;\n (M,x,y) to mark as Doubt;\n (B,x,y) to mark as Bomb;\n (U,x,y) to unmark" << endl;
		cin >> mark >> x >> y;
		if (!game.isValidLocation(x, y)){
			std::system("clear");
			continue;
		}
		if (game.locationIsMine(x,y) && (mark == 's')){
			std::system("clear");
			cout << "Busted" << endl;
			game.displayAll();
			break;
		}
		else{
			std::system("clear");
			if (mark == 'm')
				game.markAsDoubt(x,y);
			else if (mark == 'u')
				game.unmarkLocation(x,y);
			else if (mark == 'b')
				game.markAsBomb(x,y);
			else
				game.openLocation(x,y);
			cout << endl;
			if (game.onlyMinesLeft()){
				cout << "Congratualtions! You Won." << endl;
				game.displayBoard();
				game.displayAll();
				break;
			}
		}
	}
}

