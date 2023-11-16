//      ____  __           __   ____            
//     / __ )/ /___ ______/ /__/ __ )____  _  __
//    / __  / / __ `/ ___/ //_/ __  / __ \| |/_/
//   / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <  
//  /_____/_/\__,_/\___/_/|_/_____/\____/_/|_|  
//
// Przemysław Sadowski | 197696 | EiT1                                              
//
//
//  To Fix:
//  - Naprawić to że atomy respią się na label-ach
//  - Naprawić funkcję losujacą --> srand

#include <iostream>
#include <locale.h>
#include <iomanip>
#include <conio.h>

using namespace std;

// function prototypes
void game(int gridSize, int numAtoms);
void main_menu();

// displaying app controls
void display_controls() {
    cout << "===== Obsługa programu =====:\n"
        << "# (w,s,a,d) - poruszanie sie\n"
        << "# (q,Q) - wyjście do menu\n"
        << "# u, U – undo (cofnij ruch);\n"
        << "# r, R – redo (powtórz cofnięty ruch);\n"
        << "# spacja oddanie strzału (gdy kursor jest na dowolnej ścianie);\n"
        << "# o - na planszy umożliwia zaznaczenie przypuszczalnego położenia atomu;\n"
        << "# k – kończy rozgrywkę i umożliwia wyświetlenie rozwiązania i liczby uzyskanych punktów\n"
        << "#   (poprawnie znalezionych atomów);\n"
        << "# p – umożliwia wyświetlenie rozwiązania (przerywa etap gry, brak możliwości kontynuowania\n"
        << "#   tego etapu gry);\n"
        << "# H – Help/Pomoc – pokazuje na chwilę umieszczenie atomów na planszy\n"
        << "=============================\n"
        << "Wciśnij Enter aby wrócić do menu ...";
    cin.get();
    system("cls");
    main_menu();
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
        int x, y;
        do {
            x = 1 + rand() % gridSize;
            y = 1 + rand() % gridSize;
        } while (x == 1 || y == 1); // Repeat until a valid position is found

        board[x][y] = 'O';
    }
}

// priunt out cursor postion
void CursorStatus(int cursorRow, int cursorColumn, char board[][40]) {
    cout << "Current cursor coordinates - (" << cursorRow << ", " << cursorColumn << ") " << endl;

    if (board[cursorRow][cursorColumn] == 'O') {
        cout << "Cursor is on an atom" << endl;
    }
    else if (board[cursorRow][cursorColumn] == 'L') {
        cout << "Cursor is on a 'L'" << endl;
    }
    else {
        cout << "Cursor is on an empty tile" << endl;
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
        system("cls");
        main_menu();
        break;
    case 'h':
        system("cls");
        display_controls();
        break;
    default:
        // Handle other keys if needed
        break;
    }
}

// displaying board choice menu
void board_choice() {
    cout << "Wybór planszy:" << endl;
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
        cout << "Podaj liczbę atomów:" << endl;
        cin >> atoms;
        game(size, atoms);
        cin.ignore();
        system("cls");
        // ilość atomów
        break;
    default:
        cout << "Nie ma takiej opcji!" << endl;
        break;
    }

}

// displaying main menu
void main_menu() {
    // displaying figel art
    cout << "    ____  __           __   ____"
        << "\n   / __ )/ /___ ______/ /__/ __ )____  _  __"
        << "\n  / __  / / __ `/ ___/ //_/ __  / __ \\| |/_/"
        << "\n / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <"
        << "\n/_____/_/\\__,_/\\___/_/|_/_____/\\____/_/|_|"
        << "\n\nPrzemysław Sadowski | 197696 | EiT1\n\n";
    
    unsigned choice;
    cout << "===== MENU =====" << endl;
    cout << "1. Wybór planszy" << endl;
    cout << "2. Sterowanie" << endl;
    cout << "3. Wyjście z programu" << endl;
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

// game function
void game(int gridSize, int numAtoms) {
    // Inital cursor position
    int cursorRow = 1;   
    int cursorColumn = 1;

    // Initialize game board with zeros
    char board[40][40] = { 0 }; 
    
    placeAtoms(board, gridSize, numAtoms);

    while (true) {

        draw_board(cursorRow, cursorColumn, gridSize, board);

        CursorStatus(cursorRow, cursorColumn, board);

        char input = _getch(); // Use _getch() for simplicity (Windows specific)
        controls(input, cursorRow, cursorColumn, board, gridSize);
    }

}

// main function
int main() {
    // allow polish symbols
    setlocale(LC_CTYPE, "Polish");
    srand(static_cast<unsigned>(time(nullptr)));

    do {
        main_menu();
    } while (1);
	

	return 0;
}
