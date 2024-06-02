#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>

using namespace std;

class Piece {
public:
    virtual char getSymbol() const = 0;
    virtual bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const = 0;
    virtual ~Piece() {}
    bool isWhite;

protected:
    Piece(bool isWhite) : isWhite(isWhite) {}
};

class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'K' : 'k';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        return abs(startX - endX) <= 1 && abs(startY - endY) <= 1;
    }
};

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'Q' : 'q';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        int dx = abs(startX - endX);
        int dy = abs(startY - endY);
        if (dx == dy || startX == endX || startY == endY) {
            return isPathClear(startX, startY, endX, endY, board);
        }
        return false;
    }

    bool isPathClear(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const {
        int dx = (endX > startX) ? 1 : (endX < startX) ? -1 : 0;
        int dy = (endY > startY) ? 1 : (endY < startY) ? -1 : 0;
        int x = startX + dx;
        int y = startY + dy;
        while (x != endX || y != endY) {
            if (board[x][y] != nullptr) return false;
            x += dx;
            y += dy;
        }
        return true;
    }
};

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'B' : 'b';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        int dx = abs(startX - endX);
        int dy = abs(startY - endY);
        if (dx == dy) {
            return isPathClear(startX, startY, endX, endY, board);
        }
        return false;
    }

    bool isPathClear(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const {
        int dx = (endX > startX) ? 1 : -1;
        int dy = (endY > startY) ? 1 : -1;
        int x = startX + dx;
        int y = startY + dy;
        while (x != endX && y != endY) {
            if (board[x][y] != nullptr) return false;
            x += dx;
            y += dy;
        }
        return true;
    }
};

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'N' : 'n';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        int dx = abs(startX - endX);
        int dy = abs(startY - endY);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
};

class Rook : public Piece {
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'R' : 'r';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        if (startX == endX || startY == endY) {
            return isPathClear(startX, startY, endX, endY, board);
        }
        return false;
    }

    bool isPathClear(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const {
        int dx = (endX > startX) ? 1 : (endX < startX) ? -1 : 0;
        int dy = (endY > startY) ? 1 : (endY < startY) ? -1 : 0;
        int x = startX + dx;
        int y = startY + dy;
        while (x != endX || y != endY) {
            if (board[x][y] != nullptr) return false;
            x += dx;
            y += dy;
        }
        return true;
    }
};

class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    char getSymbol() const override {
        return isWhite ? 'P' : 'p';
    }
    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece*>>& board) const override {
        int direction = isWhite ? -1 : 1;
        if (startY == endY) {
            if ((endX - startX) == direction) {
                return board[endX][endY] == nullptr;
            }
            if ((endX - startX) == 2 * direction && startX == (isWhite ? 6 : 1)) {
                return board[startX + direction][startY] == nullptr && board[endX][endY] == nullptr;
            }
        }
        if (abs(startY - endY) == 1 && (endX - startX) == direction) {
            return board[endX][endY] != nullptr && board[endX][endY]->isWhite != isWhite;
        }
        return false;
    }
};

class ChessBoard {
public:
    ChessBoard() {
        board.resize(8, vector<Piece*>(8, nullptr));
        setupBoard();
    }

    void setupBoard() {
        for (int i = 0; i < 8; ++i) {
            board[1][i] = new Pawn(false);
            board[6][i] = new Pawn(true);
        }
        board[0][0] = board[0][7] = new Rook(false);
        board[7][0] = board[7][7] = new Rook(true);
        board[0][1] = board[0][6] = new Knight(false);
        board[7][1] = board[7][6] = new Knight(true);
        board[0][2] = board[0][5] = new Bishop(false);
        board[7][2] = board[7][5] = new Bishop(true);
        board[0][3] = new Queen(false);
        board[7][3] = new Queen(true);
        board[0][4] = new King(false);
        board[7][4] = new King(true);
    }

    void printBoard() const {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr) {
                    cout << board[i][j]->getSymbol() << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    bool movePiece(int startX, int startY, int endX, int endY, bool isWhiteTurn) {
        if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 || endX < 0 || endX >= 8 || endY < 0 || endY >= 8) {
            return false;
        }
        Piece* piece = board[startX][startY];
        if (piece == nullptr || piece->isWhite != isWhiteTurn || !piece->isValidMove(startX, startY, endX, endY, board)) {
            return false;
        }
        Piece* destination = board[endX][endY];
        if (destination != nullptr && destination->isWhite == isWhiteTurn) {
            return false;
        }
        delete destination;
        board[endX][endY] = piece;
        board[startX][startY] = nullptr;
        return true;
    }

    void saveGame(const string& filename, bool isWhiteTurn) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << (isWhiteTurn ? 'W' : 'B') << endl;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr) {
                        file << board[i][j]->getSymbol();
                    } else {
                        file << '.';
                    }
                }
                file << endl;
            }
            file.close();
        }
    }

private:
    vector<vector<Piece*>> board;
};

int main() {
    ChessBoard chessBoard;
    bool isWhiteTurn = true;
    string command;

    cout << "Aby grac, podaj koordynaty poczatkowe i koncowe figury ktora chcesz ruszyc" << endl;
    cout << "Aby zapisac gre, uzyj komendy - zapisz" << endl;
    cout << "Aby wyjsc z gry, uzyj komendy - wyjdz" << endl;
    while (true) {
        chessBoard.printBoard();
        cout << (isWhiteTurn ? "Ruch bialych: " : "Ruch czarnych: ");
        cin >> command;

        if (command == "wyjdz") break;
        if (command == "zapisz") {
            string filename;
            cout << "Podaj nazwe pliku: ";
            cin >> filename;
            chessBoard.saveGame(filename, isWhiteTurn);
            cout << "Gra zapisana." << endl;
            continue;
        }


        if (command.size() == 4) {
            int startX = 8 - (command[1] - '0');
            int startY = command[0] - 'a';
            int endX = 8 - (command[3] - '0');
            int endY = command[2] - 'a';

            if (chessBoard.movePiece(startX, startY, endX, endY, isWhiteTurn)) {
                isWhiteTurn = !isWhiteTurn;
            } else {
                cout << "Niepoprawny ruch. Sprobuj ponownie." << endl;
            }
        } else {
            cout << "Zla komenda. Przykladowy poprawny ruch to: e2e4." << endl;
        }
    }

    return 0;
}
