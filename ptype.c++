#include <iostream>
#include <string>
#include <conio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

void ShowConsoleCursor(int showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set cursor visibility

    SetConsoleCursorInfo(out, &cursorInfo);
}

void deleteCharacter(std::string& str, char ch) {
    size_t pos;
    while ((pos = str.find(ch)) != std::string::npos) {
        str.erase(pos, 1);
    }
}

int main() {
    std::string text;
    int index = 0;
    char ch;

    std::string cursor(1, char(219));
    ShowConsoleCursor(0);
    while (true)
    {
        system("cls");
        std::cout << text;
        deleteCharacter(text, char(219));

        ch = getch();

        if (ch == 27) // ESC
        {
            deleteCharacter(text, char(219));
            system("cls");
            std::cout << text << "\n\n";

            std::cout << "ESC";
            ShowConsoleCursor(1);
            return 0;
        } else if (ch == 13) // ENTER
        {
            text.insert(index, "\n");
            index++;
            text.insert(index, cursor);
        } else if (ch == '\b') // BACKSPACE
        {
            if (!text.empty()) {
                size_t _index = index;
                text.erase(index-1, 1);
                index--;
            }
            text.insert(index, cursor);
        } else if (ch == 0 || ch == 224 || ch == -32)
        {
            switch (getch())
            {
                case 75: // LEFT
                    if (index - 1 >= 0) 
                    {
                        index--;
                    }
                    text.insert(index, cursor);
                    break;
                case 77: // RIGHT
                    if (index + 1 <= text.length())
                    {
                        index++;
                    }
                    text.insert(index, cursor);
                    break;

                default:
                    break;
            }
        } else
        {
            std::string character(1, ch);
            text.insert(index, character);
            text.insert(index+1, cursor);
            index++;
        }
        std::cout.flush();
    }

    ShowConsoleCursor(1);
    return 0;
}