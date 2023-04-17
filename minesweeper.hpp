#pragma once
#include <vector>

class Minesweeper{
	private:
		int numRows;
		int numColumns;
		int numMines;

		struct location{
			int rowIndex;
			int columnIndex;
			bool isMine;
			bool isOpen;
			bool isMarkedDoubt;
			bool isMarkedBomb;
		};

		std::vector<location> locations;

	public:
		Minesweeper(int numRows, int numColumns, int numMines);

		void displayBoard() const;
		void displayAll() const;

		void openLocation(location &loc);
		void openLocation(const int x, const int y) { openLocation(locations[x*numRows + y]); }

		void markAsDoubt(const int x, const int y);
		void markAsBomb(const int x, const int y);
		void unmarkLocation(const int x, const int y);

		int countMinesAround(const location &loc) const;
		bool onlyMinesLeft() const;
		bool locationIsMine(int x, int y) const{ return locations[x*numRows + y].isMine; }

		bool isValidLocation(int x, int y) const { return ((x < numRows) && (x >= 0) && (y < numColumns) && (y >= 0)) ? true : false; }
};

