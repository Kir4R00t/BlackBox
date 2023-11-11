//      ____  __           __   ____            
//     / __ )/ /___ ______/ /__/ __ )____  _  __
//    / __  / / __ `/ ___/ //_/ __  / __ \| |/_/
//   / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <  
//  /_____/_/\__,_/\___/_/|_/_____/\____/_/|_|  
//
// Przemys�aw Sadowski | 197696 | EiT1                                              

#include <iostream>
#include <locale.h>
#include <iomanip>
#include <conio.h>

using namespace std;

// constexpr char board[40][40] = { 0 };

// displaying app controls
void display_controls() {
    cout << "===== Obs�uga programu =====:\n"
        << "# (w,s,a,d) - poruszanie sie\n"
        << "# (q,Q) - wyj�cie do menu\n"
        << "# u, U � undo (cofnij ruch);\n"
        << "# r, R � redo (powt�rz cofni�ty ruch);\n"
        << "# spacja oddanie strza�u (gdy kursor jest na dowolnej �cianie);\n"
        << "# o - na planszy umo�liwia zaznaczenie przypuszczalnego po�o�enia atomu;\n"
        << "# k � ko�czy rozgrywk� i umo�liwia wy�wietlenie rozwi�zania i liczby uzyskanych punkt�w\n"
        << "#   (poprawnie znalezionych atom�w);\n"
        << "# p � umo�liwia wy�wietlenie rozwi�zania (przerywa etap gry, brak mo�liwo�ci kontynuowania\n"
        << "#   tego etapu gry);\n"
        << "# H � Help/Pomoc � pokazuje na chwil� umieszczenie atom�w na planszy\n"
        << "=============================\n"
        << "Wci�nij Enter aby wr�ci� do menu ...";
    cin.get();
    system("cls");
}

// drawing board
void draw_board(int cursorRow, int cursorColumn, int gridSize, char board[][40]) {
    system("cls"); // Clear the console screen (Windows specific)

    int size = gridSize + 2; // Total size includes the corner squares

    cout << endl;

    // Display top border
    cout << "   ";
    for (int i = 1; i <= size; i++) {
        cout << "+-----";
    }
    cout << '+' << endl;

    for (int i = 1; i <= size; i++) {
        // Display side label
        cout << "   |";

        for (int j = 1; j <= size; j++) {
            if ((i == 1 && j == 1) || (i == 1 && j == size) || (i == size && j == 1) || (i == size && j == size)) {
                cout << " XXX |";
            }
            else if (i == 1 || i == size) {
                cout << "  " << setw(2) << j - 1 << " |";
            }
            else if (j == 1 || j == size) {
                cout << setw(4) << i + size - 3 << " |";
            }
            else if (i == cursorRow && j == cursorColumn) {
                cout << " ### |";
            }
            else {
                cout << " ";
                if (board[i][j] == 'O') {
                    cout << setw(2) << 'O' << ' ';
                }
                else if (board[i][j] == 'L') {
                    cout << setw(2) << 'L' << ' ';
                }
                else {
                    cout << "   ";
                }
                cout << " |";
            }
        }

        cout << endl;

        // Display side border
        if (i < size) {
            cout << "   +";
            for (int j = 1; j <= size; j++) {
                cout << "-----+";
            }
            cout << endl;
        }
    }
    // bottom border
    cout << "   ";
    for (int i = 1; i <= size; i++) {
        cout << "+-----";
    }
    cout << '+' << endl;
}

// fill board with atoms
void placeAtoms(char board[][40], int gridSize, int numAtoms) {
    for (int i = 0; i < numAtoms; i++) {
        int x = 1 + rand() % gridSize;
        int y = 1 + rand() % gridSize;

        board[x][y] = 'O';
    }
}

// priunt out cursor postion
void printCursorStatus(int cursorRow, int cursorColumn, char board[][40]) {
    cout << "Current cursor coordinates - (" << cursorRow << ", " << cursorColumn << ") " << endl;

    if (board[cursorRow][cursorColumn] == 'O') {
        cout << "Cursor is on an atom" << endl;
    }
    else if (board[cursorRow][cursorColumn] == 'L') {
        cout << "Cursor is on a 'L'" << endl;
    }
    else {
        cout << "Cursor is on an empty tile'" << endl;
    }
}

// controlling user input
void controls(char input, int& cursorRow, int& cursorColumn, char board[][40], int gridSize) {
    switch (input) {
    case 'w':
        if (cursorRow > 1) {
            cursorRow--;
        }
        break;
    case 's':
        if (cursorRow < gridSize + 2) {
            cursorRow++;
        }
        break;
    case 'a':
        if (cursorColumn > 1) {
            cursorColumn--;
        }
        break;
    case 'd':
        if (cursorColumn < gridSize + 2) {
            cursorColumn++;
        }
        break;
    case 'o':
        board[cursorRow][cursorColumn] = 'O';
        break;
    case 'p':
        // Place 'L' at the current cursor position
        board[cursorRow][cursorColumn] = 'L';
        break;
    case 'i':
        board[cursorRow][cursorColumn] = ' ';
        break;
    case 'q':
        // Exit the program
        exit(0);
    default:
        // Handle other keys if needed
        break;
    }
}

// displaying board choice menu
void board_choice() {
    cout << "Wyb�r planszy:" << endl;
    cout << "1. 5x5" << endl;
    cout << "2. 8x8" << endl;
    cout << "3. 10x10" << endl;
    cout << "4. Niestandardowy rozmiar planszy" << endl;
    unsigned choice;
    cout << "Wybierz opcje: ";
    cin >> choice;
    cin.ignore();
    system("cls");

    switch (choice) {
    case 1:
        game(5, 3);
        break;
    case 2:
        game(8, 5);
        break;
    case 3:
        game(10, 8);
        break;
    case 4:
        int size, atoms;
        cout << "Podaj rozmiar planszy:" << endl;
        cin >> size;
        cout << "Podaj liczb� atom�w:" << endl;
        cin >> atoms;
        game(size, atoms);
        cin.ignore();
        system("cls");
        // ilo�� atom�w
        break;
    default:
        cout << "Nie ma takiej opcji!" << endl;
        break;
    }

}

// displaying main menu
void main_menu() {
    unsigned choice;
    cout << "===== MENU =====" << endl;
    cout << "1. Wyb�r planszy" << endl;
    cout << "2. Sterowanie" << endl;
    cout << "3. Wyj�cie z programu" << endl;
    cout << "Wybierz opcje: ";
    cin >> choice;
    cin.ignore();
    system("cls");

    switch (choice) {
    case 1:
        board_choice();
        break;
    case 2:
        display_controls();
        break;
    case 3:
        exit(0);
        break;
    default:
        cout << "!!! Nie ma takiej opcji !!!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }
}

// app controls
void controls() {
    char action;
    cin >> action;
    switch (action) {
    case 'w':
        // Move up
        break;
    case 's':
        // Move down
        break;
    case 'a':
        // Move left
        break;
    case 'd':
        // Move right
        break;
    case 'q':
    case 'Q':
        //main_menu();
        break;
    case 'u':
    case 'U':
        // Undo (cofnij ruch)
        break;
    case 'r':
    case 'R':
        // Redo (powt�rz cofni�ty ruch)
        break;
    case ' ':
        // Oddanie strza�u (gdy kursor jest na dowolnej �cianie)
        break;
    case 'o':
        // Na planszy umo�liwia zaznaczenie przypuszczalnego po�o�enia atomu
        break;
    case 'k':
        // Zako�czenie rozgrywki i wy�wietlenie rozwi�zania
        break;
    case 'p':
        // Wy�wietlenie rozwi�zania (przerywa etap gry, brak mo�liwo�ci kontynuowania tego etapu gry)
        break;
    case 'H':
        // Help/Pomoc � pokazuje na chwil� umieszczenie atom�w na planszy
        break;
    default:
        // Invalid action
        break;
    }
}

// game function
void game(int gridSize, int numAtoms) {
    int cursorRow = 1;    // Initial cursor row position
    int cursorColumn = 1; // Initial cursor column position

    char board[40][40] = { 0 }; // Initialize the board with zeros
    
    placeAtoms(board, gridSize, numAtoms);

    while (true) {
        draw_board(cursorRow, cursorColumn, gridSize, board);
        cout << endl;

        printCursorStatus(cursorRow, cursorColumn, board);

        char input = _getch(); // Use _getch() for simplicity (Windows specific)

        controls(input, cursorRow, cursorColumn, board, gridSize);
    }

}

// main function
int main() {
    // allow polish symbols
    setlocale(LC_CTYPE, "Polish");

    do {
        // displaying figel art
        cout << "    ____  __           __   ____"
            << "\n   / __ )/ /___ ______/ /__/ __ )____  _  __"
            << "\n  / __  / / __ `/ ___/ //_/ __  / __ \\| |/_/"
            << "\n / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <"
            << "\n/_____/_/\\__,_/\\___/_/|_/_____/\\____/_/|_|"
            << "\n\nPrzemys�aw Sadowski | 197696 | EiT1\n\n";

        main_menu();
    } while (1);
	

	return 0;
}