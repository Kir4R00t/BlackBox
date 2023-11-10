//      ____  __           __   ____            
//     / __ )/ /___ ______/ /__/ __ )____  _  __
//    / __  / / __ `/ ___/ //_/ __  / __ \| |/_/
//   / /_/ / / /_/ / /__/ ,< / /_/ / /_/ />  <  
//  /_____/_/\__,_/\___/_/|_/_____/\____/_/|_|  
//
// Przemys³aw Sadowski | 197696 | EiT1                                              

#include <iostream>
#include <locale.h>
#include <iomanip>
#include <conio.h>

using namespace std;

// displaying app controls
void display_controls() {
    cout << "===== Obs³uga programu =====:\n"
        << "# (w,s,a,d) - poruszanie sie\n"
        << "# (q,Q) - wyjœcie do menu\n"
        << "# u, U – undo (cofnij ruch);\n"
        << "# r, R – redo (powtórz cofniêty ruch);\n"
        << "# spacja oddanie strza³u (gdy kursor jest na dowolnej œcianie);\n"
        << "# o - na planszy umo¿liwia zaznaczenie przypuszczalnego po³o¿enia atomu;\n"
        << "# k – koñczy rozgrywkê i umo¿liwia wyœwietlenie rozwi¹zania i liczby uzyskanych punktów\n"
        << "#   (poprawnie znalezionych atomów);\n"
        << "# p – umo¿liwia wyœwietlenie rozwi¹zania (przerywa etap gry, brak mo¿liwoœci kontynuowania\n"
        << "#   tego etapu gry);\n"
        << "# H – Help/Pomoc – pokazuje na chwilê umieszczenie atomów na planszy\n"
        << "=============================\n"
        << "Wciœnij Enter aby kontynuowaæ...";
    cin.get();
    system("cls");
}

// drawing game board
void draw_board(unsigned size) {
    // 5x5 - 3 atomy
    // 8x8 - 5 atomów
    // 10x10 - 8 atomów
    // opcja zrobienia customowej mapy 
    // np. rozmiar AxA + X atomów
    
    if (size < 5 || size > 20) {
        cout << "Nieprawid³owy rozmiar planszy [5-20]!" << endl;
        return;
    }

    cout << endl;
    cout << "    ";
    for (char label = 'A'; label < 'A' + size; ++label) {
        cout << "|  " << label << "  ";
    }
    cout << '|' << endl;
    cout << "    "; 
    for (unsigned i = 0; i < size; i++) {
        cout << "------";
    }
    cout << '-' << endl;

    for (unsigned i = 0; i < size; i++) {
        cout << setw(2) << i + 1 << "  ";
        for (unsigned j = 0; j < size; j++)
            cout << "|     "; 
        cout << '|' << setw(2) << i + 1 << endl;
        cout << "    ";
        for (unsigned j = 0; j < size; j++)
            cout << "------";
        cout << '-' << endl;
    }

    cout << "    ";
    for (char label = 'A'; label < 'A' + size; ++label) {
        cout << "|  " << label << "  ";
    }
    cout << '|' << endl;

    cout << endl;
    cout << "Wciœnij Enter aby wróciæ do menu..." << endl;
    cin.get();
    system("cls");
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
        draw_board(5);
        break;
    case 2:
        draw_board(8);
        break;
    case 3:
        draw_board(10);
        break;
    case 4:
        cout << "Podaj rozmiar planszy:" << endl;
        cin >> choice;
        cin.ignore();
        system("cls");
        draw_board(choice);
        // iloœæ atomów
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
    cout << "1. Wybór planszy" << endl;
    cout << "2. Sterowanie" << endl;
    cout << "3. Wyjœcie z programu" << endl;
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
        main_menu();
        break;
    case 'u':
    case 'U':
        // Undo (cofnij ruch)
        break;
    case 'r':
    case 'R':
        // Redo (powtórz cofniêty ruch)
        break;
    case ' ':
        // Oddanie strza³u (gdy kursor jest na dowolnej œcianie)
        break;
    case 'o':
        // Na planszy umo¿liwia zaznaczenie przypuszczalnego po³o¿enia atomu
        break;
    case 'k':
        // Zakoñczenie rozgrywki i wyœwietlenie rozwi¹zania
        break;
    case 'p':
        // Wyœwietlenie rozwi¹zania (przerywa etap gry, brak mo¿liwoœci kontynuowania tego etapu gry)
        break;
    case 'H':
        // Help/Pomoc – pokazuje na chwilê umieszczenie atomów na planszy
        break;
    default:
        // Invalid action
        break;
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
            << "\n\nPrzemys³aw Sadowski | 197696 | EiT1\n\n";

        main_menu();
    } while (true);
	

	return 0;
}