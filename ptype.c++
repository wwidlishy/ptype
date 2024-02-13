#include <iostream>
#include <string>
#include <conio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <fstream>
#include <regex>

void ShowConsoleCursor(int showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set cursor visibility

    SetConsoleCursorInfo(out, &cursorInfo);
}

// Blah Blah Blah

void deleteCharacter(std::string& str, char ch) {
    size_t pos;
    while ((pos = str.find(ch)) != std::string::npos) {
        str.erase(pos, 1);
    }
}

int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while( cnt != nth )
    {
        pos+=1;
        pos = str.find(findMe, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}

int strCount(std::string s, char ch)
{
    int count = 0;
    for (int i = 0; i < s.size(); i++) 
    {
       if (s.at(i) == ch)    
           count++;
    }
    return count;
}

#include <dirent.h>
#include <vector>

std::string current_working_directory()
{
    char* cwd = _getcwd( 0, 0 ) ; // **** microsoft specific ****
    std::string working_directory(cwd) ;
    std::free(cwd) ;
    return working_directory ;
}

int clamp(int val, int min, int max)
{
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

std::string repeatString(const std::string& str, int times) {
    std::string result;
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

int godown(int index, std::string text, int columns)
{
    if (index < text.length() - 2) return clamp(index+text.substr(index+1, text.length() - 1).find('\n')+1, 0, text.length() - 1);
}

std::string color(std::string line) {
    std::string line2 = line;

    std::regex C1("\\b(while|if)\\b");
    std::string c1 = "\e[0;35m$1\e[0m";
    line = std::regex_replace(line2, C1, c1);

    return line;
}

void printable(const std::string& text, int length, int cursorIndex, int lines) {
    std::vector<std::string> spittenText;
    std::string line = "";

    int counter = 1;
    int index = 0;

    for (char ch : text)
    {
        if (counter == length || ch == '\n') {
            spittenText.push_back(line);
            line = "";
            counter = 1;
            continue;
        }
        if (index == text.length() - 1) {
            spittenText.push_back(line);
            break;
        }

        line += ch;

        counter++;
        index++;
    }

    cursorIndex = 0;
    for (std::string str : spittenText)
    {
        if (str.find(char(219)) != std::string::npos)
        {
            break;
        }
        cursorIndex++;
    }

    int halfLines = (lines - 1) / 2;
    int add = 0;

    int startIndex;
    int endIndex;

    // std::cout << "hl " << halfLines << " cin" << cursorIndex <<  "\n";
    if (cursorIndex >= halfLines) {
        startIndex = cursorIndex - halfLines;
        add = halfLines - 1;
    } if (cursorIndex < halfLines) {
        startIndex = 0;
        add = halfLines - 1;
    }

    startIndex = clamp(startIndex, 0, spittenText.size() - 1);
    endIndex = clamp(startIndex + halfLines + add, 0, spittenText.size() - 1);

    // std::cout << std::to_string(text.find(219) != std::string::npos) << "\n";

    // std::cout << startIndex << " " << endIndex << "\n";
    if (startIndex == endIndex) std::cout << spittenText[0];
    else {
        for (int i = startIndex; i <= endIndex; i++) {
            std::cout << color(spittenText[i]) << "\n";
        }
    }
}


int main() {
    std::string text;
    std::string command;
    int index = 0;
    char ch;
    std::string filepath = "";

    std::string cursor(1, char(219));
    bool esc = false;
    ShowConsoleCursor(0);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    char highlighting = ' '; // p python, c c
    int last;

    while (true)
    {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if (!esc)
        {
            system("cls");

            std::string bar = "[" + filepath + "]"
                                 + " Pos: " + std::to_string(index - int(text.substr(0, index).rfind("\n")))
                                 + " Ln: " + std::to_string(strCount(text.substr(0, index), '\n') + 1);
            std::cout << "\033[47m\033[30m" 
                         << bar << repeatString(" ", columns - bar.length())
                         << "\033[0m" << "\n";
            if (text.back() != '\n') text += "\n";

            printable(text, columns, index, rows);
            deleteCharacter(text, char(219));

            ch = getch();

            if (ch == 27) // ESC
            {
                deleteCharacter(text, char(219));
                system("cls");
                std::cout << text << "\n\n";

                esc = !esc;
                command = "";
            } else if (ch == 13) // ENTER
            {
                text.insert(index, "\n");
                index++;
                text.insert(index, cursor);
            } else if (ch == '\b') // BACKSPACE
            {
                if (!text.empty() && index != 0) {
                    size_t _index = index;
                    text.erase(index-1, 1);
                    index--;
                }
                text.insert(index, cursor);
            } else if (ch == 0 || ch == 224 || ch == -32)
            {
                switch (getch())
                {
                    case 72: // UP
                        last = index;
                        index = clamp(text.substr(0, index).find_last_of('\n'), 0, text.length() - 1);
                        text.insert(index, cursor);

                        break;
                    case 80: // DOWN
                        if (index < text.length() - 2)
                        {
                            index = clamp(godown(index, text, columns), 0, text.length() - 1);
                            text.insert(index, cursor);
                        }

                        break;
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
            } else if (ch == '\t')
            {
                text.insert(index, "    ");
                text.insert(index+1, cursor);
                index += 4;
            } else
            {
                std::string character(1, ch);
                text.insert(index, character);
                text.insert(index+1, cursor);
                index++;
            }
            std::cout.flush();
        } else
        {
            command = "";
            system("cls");
            std::string bar = "[" + filepath + "]"
                                 + " Pos: " + std::to_string(index - int(text.substr(0, index).rfind("\n")))
                                 + " Ln: " + std::to_string(strCount(text.substr(0, index), '\n') + 1);
            std::cout << "\033[47m\033[30m" 
                         << bar << repeatString(" ", columns - bar.length())
                         << "\033[0m" << "\n" << "Command: ";

            std::string input;
            std::cin >> input;

            if (input == "qq") 
            {
                system("cls");
                ShowConsoleCursor(1);
                return 0;
            }
            if (input == "q")
            {
                std::cout << "Do you want to save?" << "\n" << "(yn) ";
                std::string choice;
                std::cin >> choice;

                while (choice != "y" && choice != "n")
                {
                    system("cls");
                    std::cout << "\033[47m\033[30m" 
                                << bar << repeatString(" ", columns - bar.length())
                                << "\033[0m" << "\n";
                    std::cout << "Command: " << input << "\n";
                    std::cout << "Do you want to save?" << "\n" << "(yn) ";
                    std::cin >> choice;
                }

                if (choice == "y")
                {
                    if (filepath == "")
                    {
                        std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                        std::cin >> filepath;
                    }
                    std::ofstream save(filepath);
                    save << text;
                    save.close();
                }

                system("cls");
                ShowConsoleCursor(1);
                return 0;

            }
            if (input == "ww")
            {
                if (filepath == "")
                {
                    std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                    std::cin >> filepath;
                }
                std::ofstream save(filepath);
                save << text;
                save.close();
            }
            if (input == "wq")
            {
                if (filepath == "")
                {
                    std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                    std::cin >> filepath;
                }
                std::ofstream save(filepath);
                save << text;
                save.close();
                ShowConsoleCursor(1);
                system("cls");
                return 0;
            }
            if (input == "w")
            {
                std::string path;
                std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                std::cin >> path;
                std::ofstream save(path);
                save << text;
                save.close();
            }

            if (input == "oo")
            {
                std::string path;
                std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                std::cin >> path;

                std::ifstream file;
                file.open(path);

                if (!file)
                {
                    continue;
                } else
                {
                    std::stringstream buffer;
                    buffer << file.rdbuf(); // Read the file buffer into the stringstream
                    text = buffer.str(); // Convert the stringstream to a string


                    filepath = path;
                    index = 0;
                }
            }
            if (input == "o")
            {
                std::string path;
                std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                std::cin >> path;

                std::ifstream file;
                file.open(path);

                if (!file)
                {
                    continue;
                } else
                {
                    std::cout << "Do you want to save?" << "\n" << "(yn) ";
                    std::string choice;
                    std::cin >> choice;

                    while (choice != "y" && choice != "n")
                    {
                        system("cls");
                        std::cout << "\033[47m\033[30m" 
                                    << bar << repeatString(" ", columns - bar.length())
                                    << "\033[0m" << "\n";
                        std::cout << "Command: " << input << "\n";
                        std::cout << "Do you want to save?" << "\n" << "(yn) ";
                        std::cin >> choice;
                    }

                    if (choice == "y") {
                        if (filepath == "")
                        {
                            std::cout << "Current path: " << current_working_directory() << "\n" << "Enter filename: ";
                            std::cin >> filepath;
                        }
                        std::ofstream save(filepath);
                        save << text;
                        save.close();
                    }

                    std::stringstream buffer;
                    buffer << file.rdbuf(); // Read the file buffer into the stringstream
                    text = buffer.str(); // Convert the stringstream to a string


                    filepath = path;
                    index = 0;
                }
            }
            esc = !esc;
        }
    }

    ShowConsoleCursor(1);
    return 0;
}

