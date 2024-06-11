#include <iostream>
#include <iomanip>

using namespace std;

const unsigned MAX_SIZE = 40;
const int OPEN_MAIN_MENU = -1;


// Check for system so that it will use proper command
#ifdef _WIN32
const string clear_command = "cls";
#else
const string clear_command = "clear";
#endif

void clearConsole() {
    system(clear_command.c_str());
}

const int MAX_HISTORY_SIZE = 100;

struct atomCoordinates {
    int x;
    int y;
};

struct MoveData {
    char moveHistory[MAX_HISTORY_SIZE];
    char tempHistory[MAX_HISTORY_SIZE];
    int topIndex;
    int tempTopIndex;
};

void initializeMoveData(MoveData& moveData) {
    moveData.topIndex = -1;
    moveData.tempTopIndex = -1;
}

// Function placeholders
void controls(char input, int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize, atomCoordinates atomCoordinates[], MoveData& moveData);
void main_menu(atomCoordinates atomCoordinates[]);
void game(int gridSize, int numAtoms, atomCoordinates atomCoordinates[]);
void board_choice(atomCoordinates atomCoordinates[]);

void sleep(int seconds) {
    clock_t end_time = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < end_time) {}
}

void display_controls(atomCoordinates atomCoordinates[]) {
    cout << "===== Obsluga programu =====:\n"
        << "# (w,s,a,d) - poruszanie sie\n"
        << "# (q,Q) - wyjscie do menu\n"
        << "# u, U – undo (cofnij ruch);\n"
        << "# r, R – redo (powtorz cofniety ruch);\n"
        << "# spacja - oddanie strzalu (gdy kursor jest na dowolnej scianie);\n"
        << "# o - na planszy umozliwia zaznaczenie przypuszczalnego polozenia atomu;\n"
        << "# k – konczy rozgrywke i umozliwia wyswietlenie rozwiazania i liczby uzyskanych punktow\n"
        << "#   (poprawnie znalezionych atomow);\n"
        << "# H – Help/Pomoc – pokazuje na chwile umieszczenie atomow na planszy\n"
        << "=============================\n"
        << "Wcisnij Enter aby wrocic do menu ...";
    cin.get();
    main_menu(atomCoordinates);
}

void draw_board(int cursorRow, int cursorColumn, int gridSize, char board[MAX_SIZE][MAX_SIZE], bool show_atoms) {
    unsigned size = gridSize + 2;

    cout << endl;

    cout << "   ";
    for (int i = 1; i <= size; i++) cout << "+-----";
    cout << '+' << endl;

    for (int i = 1; i <= size; i++) {
        cout << "   |";

        char znaczek = 178;
        for (int j = 1; j <= size; j++) {
            if ((i == 1 && j == 1) || (i == 1 && j == size) || (i == size && j == 1) || (i == size && j == size)) cout << " XXX |";

            else if (i == cursorRow && j == cursorColumn) cout << " ### |";

            else if (i == 1 || i == size) {
                if (board[i][j] == 'H') cout << setw(3) << 'H' << "  |";
                else cout << setw(2) << znaczek << znaczek << znaczek << " |";
            }

            else if (j == 1 || j == size) {
                if (board[i][j] == 'H') cout << setw(3) << 'H' << "  |";
                else cout << setw(2) << znaczek << znaczek << znaczek << " |";
            }

            else {
                cout << " ";
                if (board[i][j] == 'A') {
                    if (show_atoms == false) cout << "   ";
                    else cout << setw(2) << 'A' << ' ';
                }

                else if (board[i][j] == 'X') cout << setw(2) << 'X' << ' ';
                else if (board[i][j] == 'O') cout << setw(2) << 'O' << ' ';
                else if (board[i][j] == 'o') cout << setw(2) << 'o' << ' ';
                else cout << "   ";
                cout << " |";
            }
        }
        cout << endl;
        if (i < size) {
            cout << "   +";
            for (int j = 1; j <= size; j++) cout << "-----+";
            cout << endl;
        }
    }

    cout << "   ";
    for (int i = 1; i <= size; i++) cout << "+-----";
    cout << '+' << endl;
}

void placeAtoms(char board[MAX_SIZE][MAX_SIZE], int gridSize, int numAtoms, atomCoordinates atomCoordinates[]) {
    for (int i = 0; i < numAtoms; i++) {
        int x, y;
        do {
            x = 1 + rand() % gridSize;
            y = 1 + rand() % gridSize;
        } while (x == 1 || y == 1);

        board[x][y] = 'A';

        atomCoordinates[i].x = x;
        atomCoordinates[i].y = y;
    }
}

void hint(int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize) {
    clearConsole();
    cout << "Podpowiedz" << endl;
    draw_board(cursorRow, cursorColumn, gridSize, board, true);
    sleep(5);
    clearConsole();
    return;
}

void endGame(int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize, atomCoordinates atomCoordinates[]) {
    int size = gridSize + 2;
    clearConsole();
    cout << "Koniec gry, powrot do Menu za 10 sekund ..." << endl;

    int poprawne = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'o') {
                for (int k = 0; k < size; k++) {
                    if (atomCoordinates[k].x == i && atomCoordinates[k].y == j) {
                        board[i][j] = 'O';
                        poprawne++;
                        break;
                    }
                    else board[i][j] = 'X';
                }
            }
        }
    }
    draw_board(cursorRow, cursorColumn, gridSize, board, true);

    cout << "Liczba poprawnie znalezionych atomow: " << poprawne << endl;
    sleep(10);
    clearConsole();
    main_menu(atomCoordinates);
}

void shoot_laser(int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize) {
    unsigned size = gridSize + 2;

    if (cursorRow == 1 or cursorRow == size) {
        for (int i = 0; i < size; i++) {
            if (board[i][cursorColumn] == 'A') {
                board[cursorRow][cursorColumn] = 'H';
                break;
            }
        }
    }

    else if (cursorColumn == 1 or cursorColumn == size) {
        for (int i = 0; i < size; i++) {
            if (board[cursorRow][i] == 'A') {
                board[cursorRow][cursorColumn] = 'H';
                break;
            }
        }
    }
}

void un_shoot_laser(int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize) {
    unsigned size = gridSize + 2;

    if (board[cursorRow][cursorColumn] == 'H') board[cursorRow][cursorColumn] = ' ';
}

void CursorStatus(int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE]) {
    cout << endl;
    cout << "Aktualna pozycja kursora - (" << cursorRow << ", " << cursorColumn << ") " << endl;
}

void addToHistory(MoveData& moveData, char move) {
    if (move == 'w' || move == 's' || move == 'a' || move == 'd' || move == 'o' || move == 'p' || move == 'l') {
        if (moveData.topIndex < MAX_HISTORY_SIZE - 1) {
            moveData.moveHistory[++moveData.topIndex] = move;
        }
    }
}

// So as you can see redoing/undoing is just getting last move stored in history and just doing the opposite move to it
// I think that it will work way better if I had history stored in a struct that would hold coordinates of all things on the map
// But since this approach is easier and this was just a simple project for uni I am okay with how things are working
void undoLastMove(MoveData& moveData, atomCoordinates atomCoordinates[], int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize) {
    if (moveData.tempHistory[moveData.tempTopIndex] == 'a') controls('d', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'd') controls('a', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'w') controls('s', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 's') controls('w', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'o') controls('p', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'p') controls('o', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'l') un_shoot_laser(cursorRow, cursorColumn, board, gridSize);

    moveData.tempHistory[++moveData.tempTopIndex] = moveData.moveHistory[moveData.topIndex--];
    cout << "Cofnieto ruch..." << endl;
}

void redoLastMove(MoveData& moveData, atomCoordinates atomCoordinates[], int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize) {
    if (moveData.tempHistory[moveData.tempTopIndex] == 'a') controls('a', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'd') controls('d', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'w') controls('w', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 's') controls('s', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'o') controls('o', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'p') controls('p', cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    else if (moveData.tempHistory[moveData.tempTopIndex] == 'l') shoot_laser(cursorRow, cursorColumn, board, gridSize);

    addToHistory(moveData, moveData.tempHistory[moveData.tempTopIndex--]);
    cout << "Przywrocono ruch..." << endl;
}

void menu(atomCoordinates atomCoordinates[], MoveData& moveData) {
    int gridSize = 0;
    int numAtoms = 0;
    
    cout << "1. Powrot do gry" << endl;
    cout << "2. Zacznij od nowa" << endl;
    unsigned choice;
    cout << "Wybierz opcje: ";

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> choice;

    if (choice == 1) {
        clearConsole();
        return;
    }
    else if (choice == 2) {
        clearConsole();
        initializeMoveData(moveData);
        
    }
    else {
        cout << "Nie ma takiej opcji!" << endl;
        menu(atomCoordinates, moveData);
    }
}

void controls(char input, int& cursorRow, int& cursorColumn, char board[MAX_SIZE][MAX_SIZE], int gridSize, atomCoordinates atomCoordinates[], MoveData& moveData) {
    switch (input) {
    case 'w':
        if (cursorRow > 1) cursorRow--;
        break;
    case 's':
        if (cursorRow < gridSize + 2) cursorRow++;
        break;
    case 'a':
        if (cursorColumn > 1) cursorColumn--;
        break;
    case 'd':
        if (cursorColumn < gridSize + 2) cursorColumn++;
        break;
    case 'q':
    case 'Q':
        menu(atomCoordinates, moveData);
        break;
    case 'H':
    case 'h':
        hint(cursorRow, cursorColumn, board, gridSize);
        break;
    case ' ':
        shoot_laser(cursorRow, cursorColumn, board, gridSize);
        break;
    case 'o':
        board[cursorRow][cursorColumn] = 'o';
        break;
    case 'p':
        board[cursorRow][cursorColumn] = ' ';
        break;
    case 'k':
    case 'K':
        endGame(cursorRow, cursorColumn, board, gridSize, atomCoordinates);
        break;
    case 'u':
        undoLastMove(moveData, atomCoordinates, cursorRow, cursorColumn, board, gridSize);
        break;
    case 'r':
        redoLastMove(moveData, atomCoordinates, cursorRow, cursorColumn, board, gridSize);
        break;
    default:
        break;
    }
}

void board_choice(atomCoordinates atomCoordinates[]) {
    cout << "Wybor planszy:" << endl;
    cout << "1. 5x5" << endl;
    cout << "2. 8x8" << endl;
    cout << "3. 10x10" << endl;
    cout << "4. Niestandardowy rozmiar planszy" << endl;
    unsigned choice;
    cout << "Wybierz opcje: ";
    cin >> choice;
    

    switch (choice) {
    case 1:
        clearConsole();
        game(5, 3, atomCoordinates);
        break;
    case 2:
        clearConsole();
        game(8, 5, atomCoordinates);
        break;
    case 3:
        clearConsole();
        game(10, 8, atomCoordinates);
        break;
    case 4:
        clearConsole();
        int size, atoms;
        cout << "Podaj rozmiar planszy:" << endl;
        cin >> size;
        cout << "Podaj liczbe atomow:" << endl;
        cin >> atoms;
        game(size, atoms, atomCoordinates);
        break;
    default:
        cout << "Nie ma takiej opcji!" << endl;
        break;
    }
}

void main_menu(atomCoordinates atomCoordinates[]) {
    cout << "    ____  __           __   ____"
        << "\n   / __ )/ /___ ______/ /__/ __ )____  _  __"
        << "\n  / __  / / __ `/ ___/ //_/ __  / __ \\| |/_/"
        << "\n / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <"
        << "\n/_____/_/\\__,_/\\___/_/|_/_____/\\____/_/|_|"
        << "\n\nPrzemyslaw Sadowski | 197696 | EiT1\n\n";

    unsigned choice;
    cout << "===== MENU =====" << endl;
    cout << "1. Wybor planszy" << endl;
    cout << "2. Sterowanie" << endl;
    cout << "3. Wyjscie z programu" << endl;
    cout << "Wybierz opcje: ";
    cin >> choice;

    switch (choice) {
    case 1:
        clearConsole();
        board_choice(atomCoordinates);
        break;
    case 2:
        clearConsole();
        display_controls(atomCoordinates);
        break;
    case 3:
        exit(0);
        break;
    default:
        cout << "Blad!";
        break;
    }
}

void game(int gridSize, int numAtoms, atomCoordinates atomCoordinates[]) {
    int cursorRow = 2;
    int cursorColumn = 2;
    bool show_atoms = false;
    char board[MAX_SIZE][MAX_SIZE] = { 0 };

    placeAtoms(board, gridSize, numAtoms, atomCoordinates);

    MoveData moveData;
    initializeMoveData(moveData);

    while (true) {
        draw_board(cursorRow, cursorColumn, gridSize, board, show_atoms);
        CursorStatus(cursorRow, cursorColumn, board);

        char input;
        cin >> noskipws >> input;
        addToHistory(moveData, input);

        clearConsole();
        controls(input, cursorRow, cursorColumn, board, gridSize, atomCoordinates, moveData);
    }
}

int main(int gridSize, int numAtoms) {
    srand(static_cast<unsigned>(time(nullptr)));
    atomCoordinates atomCoordinates[MAX_SIZE];

    do {
        main_menu(atomCoordinates);
    } while (true);

    return 0;
}
