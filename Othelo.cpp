#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

void start_screen();
void create_board();
void start_game();
void update_board();
void valid_plays();
bool black_turn,white_turn;

vector<string> valid_moves;

// a 2D Array containing the status of the board's blocks  
string positions[8][8];

int main()
{
    char c;

    // sets console's output encoding to UTF8 so the unicode chars encode correctly.
    SetConsoleOutputCP(CP_UTF8);

    start_screen();

    while(true)
    {
        c = _getch();
        if(c == '0')
        {
            start_game();
            break;
        }
        else if(c == '1')
            return 0;
    }
    return 0;
}

void start_screen()
{
    for(int i = 0;i < 3;i++)
    {
        if (i == 0)
            cout << "\u250C\u2500\u2500\u2500\u2500\u2500\u2500Othello\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
        else if(i == 2)
            cout << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        else
        {
            cout << "\u2502     [0]: PLAY     \u2502\n";
            cout << "\u2502     [1]: EXIT     \u2502\n";
        }
    }
}

void start_game()
{
    // fill the positions array with empty position and the starting 4 pieces
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
        {
            if((i == 3 && j == 3) || (i == 4 && j == 4))
                positions[i][j] = "\u26AA";
            else if((i == 3 && j == 4) || (i == 4 && j == 3))
                positions[i][j] = "\u26AB";
            else
                positions[i][j] = " ";
        }

    black_turn = true;
    white_turn = false;
    update_board();
    valid_plays();
}

void update_board()
{
    int x=0,y=0,skip=0,column=97,row=1;
    for(int i = 0;i < 10;i++)
    {
        for(int j = 0;j < 64;j++)
        {
            if(i == 0)
                break;
            else if(i == 1)
            {
                if(j == 0)
                    cout << "\u250C";
                else if(j == 63)
                    cout << "\u2510";
                else
                {
                    if(j % 8 == 0)
                        cout << "\u252C";
                    else
                        cout << "\u2500";
                }
            }
            else if(i == 9)
            {
                if(j == 0)
                    cout << "\u2514";
                else if(j == 63)
                    cout << "\u2518";
                else
                {
                    if(j % 8 == 0)
                        cout << "\u2534";
                    else
                        cout << "\u2500";
                }
            }
            else
            {
                if(j == 0)
                    cout << "\u251C";
                else if(j == 63)
                    cout << "\u2524";
                else
                {
                    if(j % 8 == 0)
                        cout << "\u253C";
                    else
                        cout << "\u2500";
                }
            }
        }
        cout << "\n";
        
        if(i != 9)
        {
            for(int j = 0;j < 64;j++)
            {
                if(i == 0)
                {
                    if(j % 4 == 0 && j % 8 != 0)
                    {
                        cout << char(column++);
                    }
                    else
                    {
                        cout << " ";
                    }
                }
                else
                {
                    if(j == 0)
                        cout << "\u2502";
                    else if(j == 63)
                        cout << "\u2502";
                    else
                    {
                        if(j % 8 == 0)
                            cout << "\u2502";
                        else
                        {
                            if(j % 4 == 0)
                            {
                                if(positions[x][y] != " ")
                                    skip = 1;
                                cout << positions[x][y++];
                            }
                            else if(skip)
                            {
                                cout << "";
                                skip = 0;
                            }
                            else
                            {
                                cout << " ";
                            }
                        }
                    }
                }
            }
            if(i > 0)
            {
                cout << row++;
                x++;
            }
            cout << "\n";
            y=0;
        }
    }
}

void valid_plays()
{
    int count;
    if(black_turn)
    {
        for(int i = 0;i < 8;i++)
            for(int j = 0;j < 8;j++)
            {
                //black
                if(positions[i][j] == "\u26AB")
                {
                    //left
                    if(j - 1 >= 0)
                    {
                        if(positions[i][j-1] == "\u26AA")
                        {
                            count = 2;
                            while(j-count >= 0)
                            {
                                if(positions[i][j-count] == " ")
                                {
                                    valid_moves.push_back(string(1, char('a' + j - count)) + to_string(i+count-1));
                                    break;
                                }
                                else if(positions[i][j-count] == "\u26AB")
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                    }
                    //right
                    if(j+1 <= 7)
                    {
                        if(positions[i][j+1] == "\u26AA")
                        {
                            count = 2;
                            while(j+count <= 7)
                            {
                                if(positions[i][j+count] == " ")
                                {
                                    valid_moves.push_back(string(1,char('a' + j + count)) + to_string(i+count-1));
                                    break;
                                }
                                else if(positions[i][j-count] == "\u26AB")
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                    }
                    //up
                    if(i-1 >= 0)
                    {
                        if(positions[i-1][j] == "\u26AA")
                        {
                            count = 2;
                            while(i-count >= 0)
                            {
                                if(positions[i-count][j] == " ")
                                {
                                    valid_moves.push_back(string(1,char('a' + j)) + to_string(i-count+1));
                                    break;
                                }
                                else if(positions[i-count][j] == "\u26AB")
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                    }
                    //down
                    if(i+1 <= 7)
                    {
                        if(i+1 <= 7)
                        {
                            if(positions[i+1][j] == "\u26AA")
                            {
                                count = 2;
                                while(i+count >= 0)
                                {
                                    if(positions[i+count][j] == " ")
                                    {
                                        valid_moves.push_back(string(1,char('a' + j)) + to_string(i+count+1));
                                        break;
                                    }
                                    else if(positions[i+count][j] == "\u26AB")
                                    {
                                        break;
                                    }
                                    count++;
                                }
                            }
                        }
                    }
                }
            }
    }

    for(int i = 0;i < valid_moves.size();i++)
        cout << valid_moves[i];
}