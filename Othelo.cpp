#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <random>
#include <fstream>
using namespace std;

void start_screen();
void create_board();
void start_game();
void update_board();
void play_turn();
void valid_plays(string move);
void turn_pieces(string start,string end,string type,string turn);
void result();
int randomInt(int N);
void show_valid_moves();
void save_game();
void load_game();
void save_scoreboard();
void load_scoreboard();
int check_duplicate(string move);
int data();
bool black_turn,computer=false,save;

struct row {
    string winner_name;
    int win_count;
};

vector<string> valid_moves;
vector<row> winners;

// a 2D Array containing the status of the board's blocks  
string positions[8][8];

int main()
{
    char c;

    // sets console's output encoding to UTF8 so the unicode chars encode correctly.
    SetConsoleOutputCP(CP_UTF8);

    load_scoreboard();
    start_screen();

    while(true)
    {
        c = _getch();
        if(c == '0')
        {
            start_game();
            system("cls");
            start_screen();
        }
        else if(c == '1')
        {
            if(data())
                load_game();

            system("cls");
            start_screen();
        }
        else if(c == '2')
        {
            if(computer)
                computer = false;
            else
                computer = true;

            system("cls");
            start_screen();
        }
        else if(c == '3')
        {
            if(save)
                save = false;
            else
                save = true;

            system("cls");
            start_screen();
        }
        else if(c == '4')
            break;

    }
    return 0;
}

void start_screen()
{
    string mode,save_game;
    if(save)
        save_game = "Enabled";
    else
        save_game = "Disabled";
    if(computer)
        mode = "1 vs Computer";
    else
        mode = "1 vs 1";
    for(int i = 0;i < 3;i++)
    {
        if (i == 0)
            cout << "\u250C\u2500\u2500\u2500\u2500\u2500\u2500Othello\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
        else if(i == 2)
            cout << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        else
        {
            cout << "\u2502     [0]: PLAY     \u2502\n";
            cout << "\u2502  [1]: Load Game   \u2502\n";
            cout << "\u2502  [2]: Change Mode \u2502\n";
            cout << "\u2502  [3]: Save Game   \u2502\n";
            cout << "\u2502     [4]: EXIT     \u2502\n";
        }
    }

    cout << "Mode: " << mode << "\n";
    cout << "Save Game: " << save_game << "\n";

    if(data())
        cout << "A previously saved game can be loaded!\n";
    else
        cout << "There isn't any saved game to load!\n";

    for(int i = 0;i < winners.size()+2;i++)
    {
        if (i == 0)
            cout << "\u250C\u2500\u2500\u2500\u2500\u2500\u2500Score Board\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
        else if(i == winners.size()+1)
            cout << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        else
        {
            cout << "\u2502    "<< winners[i-1].winner_name << " " << winners[i-1].win_count << "    \u2502\n";
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
    update_board();
    valid_plays("");
    play_turn();
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

void valid_plays(string move)
{
    valid_moves.clear();
    int count;
    string turn,outturn;
     
    if(black_turn)
    {
        turn = "\u26AB";
        outturn = "\u26AA";
    }
    else
    {
        turn = "\u26AA";
        outturn = "\u26AB";
    }

    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
        {
            if(positions[i][j] == turn)
            {
                //left
                if(j - 1 >= 0)
                {
                    if(positions[i][j-1] == outturn)
                    {
                        count = 2;
                        while(j-count >= 0)
                        {
                            if(positions[i][j-count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1, char('a' + j - count)) + to_string(i+1));
                                if(move != "" && string(1, char('a' + j - count)) + to_string(i+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i)+to_string(j-count),"left",turn);
                                }
                                break;
                            }
                            else if(positions[i][j-count] == turn)
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
                    if(positions[i][j+1] == outturn)
                    {
                        count = 2;
                        while(j+count <= 7)
                        {
                            if(positions[i][j+count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j + count)) + to_string(i+1));
                                if(move != "" && string(1,char('a' + j + count)) + to_string(i+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i)+to_string(j+count),"right",turn);
                                }
                                break;
                            }
                            else if(positions[i][j+count] == turn)
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
                    if(positions[i-1][j] == outturn)
                    {
                        count = 2;
                        while(i-count >= 0)
                        {
                            if(positions[i-count][j] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j)) + to_string(i-count+1));
                                if(move != "" && string(1,char('a' + j)) + to_string(i-count+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i-count)+to_string(j),"up",turn);
                                }
                                break;
                            }
                            else if(positions[i-count][j] == turn)
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
                        if(positions[i+1][j] == outturn)
                        {
                            count = 2;
                            while(i+count <= 7)
                            {
                                if(positions[i+count][j] == " ")
                                {
                                    if(!check_duplicate(move))
                                        valid_moves.push_back(string(1,char('a' + j)) + to_string(i+count+1));
                                    if(move != "" && string(1,char('a' + j)) + to_string(i+count+1) == move)
                                    {
                                        turn_pieces(to_string(i)+to_string(j),to_string(i+count)+to_string(j),"down",turn);
                                    }
                                    break;
                                }
                                else if(positions[i+count][j] == turn)
                                {
                                    break;
                                }
                                count++;
                            }
                        }
                    }
                }
                //diameter
                if(i-1 >= 0 && j+1 <= 7)
                {
                    if(positions[i-1][j+1] == outturn)
                    {
                        count = 2;
                        while(i-count >= 0 && j+count <= 7)
                        {
                            if(positions[i-count][j+count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j + count)) + to_string(i-count+1));
                                if(move != "" && string(1,char('a' + j + count)) + to_string(i-count+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i-count)+to_string(j+count),"diamup_right",turn);
                                }
                                break;
                            }
                            else if(positions[i-count][j+count] == turn)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                }
                if(i-1 >= 0 && j-1 >= 0)
                {
                    if(positions[i-1][j-1] == outturn)
                    {
                        count = 2;
                        while(i-count >= 0 && j-count >= 0)
                        {
                            if(positions[i-count][j-count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j - count)) + to_string(i-count+1));
                                if(move != "" && string(1,char('a' + j - count)) + to_string(i-count+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i-count)+to_string(j-count),"diamup_left",turn);
                                }
                                break;
                            }
                            else if(positions[i-count][j-count] == turn)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                }
                if(i+1 <= 7 && j+1 <= 7)
                {
                    if(positions[i+1][j+1] == outturn)
                    {
                        count = 2;
                        while(i+count <= 7 && j+count <= 7)
                        {
                            if(positions[i+count][j+count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j + count)) + to_string(i+count+1));
                                if(move != "" && string(1,char('a' + j + count)) + to_string(i+count+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i+count)+to_string(j+count),"diamdown_right",turn);
                                }
                                break;
                            }
                            else if(positions[i+count][j+count] == turn)
                            {
                                break;
                            }
                            count++;
                        }
                    }
                }
                if(i+1 <= 7 && j-1 >= 0)
                {
                    if(positions[i+1][j-1] == outturn)
                    {
                        count = 2;
                        while(i+count <= 7 && j-count >= 0)
                        {
                            if(positions[i+count][j-count] == " ")
                            {
                                if(!check_duplicate(move))
                                    valid_moves.push_back(string(1,char('a' + j - count)) + to_string(i+count+1));
                                if(move != "" && string(1,char('a' + j - count)) + to_string(i+count+1) == move)
                                {
                                    turn_pieces(to_string(i)+to_string(j),to_string(i+count)+to_string(j-count),"diamdown_left",turn);
                                }
                                break;
                            }
                            else if(positions[i+count][j-count] == turn)
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

void turn_pieces(string start,string end,string type,string turn)
{
    if(type == "up")
    {
        for(int i = (start[0] - '0')-1;i > end[0] - '0';i--)
        {
            positions[i][start[1]-'0'] = turn;
        }
    }
    else if(type == "down")
    {
        for(int i = (start[0] - '0')+1;i < end[0] - '0';i++)
        {
            positions[i][start[1]-'0'] = turn;
        }
    }
    else if(type == "right")
    {
        for(int i = (start[1] - '0')+1;i < end[1] - '0';i++)
        {
            positions[start[0]-'0'][i] = turn;
        }
    }
    else if(type == "left")
    {
        for(int i = (start[1] - '0')-1;i > end[1] - '0';i--)
        {
            positions[start[0]-'0'][i] = turn;
        }
    }
    else if(type == "diamup_right")
    {
        for(int i = (start[0] - '0')-1,j = (char(start[1])-'0')+1;i > end[0] - '0';i--)
        {
            positions[i][j++] = turn;
        }
    }
    else if(type == "diamup_left")
    {
        for(int i = (start[0] - '0')-1,j = (char(start[1])-'0')-1;i > end[0] - '0';i--)
        {
            positions[i][j--] = turn;
        }
    }
    else if(type == "diamdown_right")
    {
        for(int i = (start[0] - '0')+1,j = (char(start[1])-'0')+1;i < end[0] - '0';i++)
        {
            positions[i][j++] = turn;
        }
    }
    else if(type == "diamdown_left")
    {
        for(int i = (start[0] - '0')+1,j = (char(start[1])-'0')-1;i < end[0] - '0';i++)
        {
            positions[i][j--] = turn;
        }
    }
}

void play_turn()
{
    bool GameOver=false,valid,black_out=false,white_out=false;
    while(!GameOver)
    {
        valid = false;
        valid_plays("");
        show_valid_moves();
        string move;
        if(black_turn && !black_out)
        {
            cout << "black moves: ";
            if(valid_moves.size() != 0)
            {
                while(!valid)
                {
                    cin >> move;
                    for(int i = 0;i < valid_moves.size();i++)
                    {
                        if(move == valid_moves[i])
                        {
                            valid = true;
                            valid_plays(move);
                            positions[char(move[1]-'0')-1][char(move[0]) - 97] = "\u26AB";
                            system("cls");
                        }
                    }
                    if(!valid)
                        cout << "Not a valid move!\nblack moves: ";
                }
            }
            else
            {
                black_out = true;
                cout << "Out Of Moves!\n";
            }
            black_turn = false;
        }
        else
        {
            if(computer)
            {
                cout << "Computer moves: ";
                if(valid_moves.size() != 0)
                {
                    move = valid_moves[randomInt(valid_moves.size())];
                    valid_plays(move);
                    positions[char(move[1]-'0')-1][char(move[0]) - 97] = "\u26AA";
                    cout << move;
                }
                else
                {
                    white_out = true;
                    cout << "Out Of Moves!\n";
                }
            }
            else
            {
                cout << "white moves: ";
                if(valid_moves.size() != 0)
                {
                    while(!valid)
                    {
                        cin >> move;
                        for(int i = 0;i < valid_moves.size();i++)
                        {
                            if(move == valid_moves[i])
                            {
                                valid = true;
                                valid_plays(move);
                                positions[char(move[1]-'0')-1][char(move[0]) - 97] = "\u26AA";
                                system("cls");
                            }
                        }
                        if(!valid)
                            cout << "Not a valid move!\nwhite moves: ";
                    }
                }
                else
                {
                    white_out = true;
                    cout << "Out Of Moves!\n";
                }

            }
            
            black_turn = true;
        }
        if(save)
            save_game();
        update_board();

        if(black_out && white_out)
            GameOver = true;
    }

    result();
}

void result()
{
    int white = 0,black = 0;
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
        {
            if(positions[i][j] == "\u26AB")
                black++;
            else
                white++;
        }
    string winner;
    if(white > black)
    {
        cout << "White Wins!\nPlease Enter Winner's Name: ";
        if(computer)
            winner = "Computer";
        else
            cin >> winner;
    }
    else
    {
        cout << "Black Wins!\nPlease Enter Winner's Name: ";
        cin >> winner;
    }

    for(int i = 0;i < winners.size();i++)
    {
        if(winner == winners[i].winner_name)
        {
            winners[i].win_count++;
            return ;
        }
    }
    row winner_data;
    winner_data.winner_name = winner;
    winner_data.win_count = 1;
    winners.push_back(winner_data);

    save_scoreboard();
}

int randomInt(int N) {
    static mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, N-1);
    return dist(gen);
}

void show_valid_moves()
{
    cout << "Valid Moves: ";
    for(int i = 0;i < valid_moves.size();i++)
    {
        cout << valid_moves[i];
        if(i == valid_moves.size()-1)
            break;
        cout << " -- ";
    }
    cout << "\n";
}

void save_game()
{
    ofstream out("save.txt");
    
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
            out << positions[i][j] << '\n';

    out << black_turn << '\n';
    out << computer << '\n';
}

void load_game()
{
    ifstream in("save.txt");

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            getline(in, positions[i][j]);

    in >> black_turn;
    in >> computer;

    save = true;

    update_board();
    play_turn();
}

int data()
{
    ifstream in("save.txt");

    if(in.good() && in.peek() != ifstream::traits_type::eof())
        return 1;
    else
        return 0;
}

void save_scoreboard()
{
    ofstream out("score.txt");

    out << winners.size() << '\n';
    for(int i = 0;i < winners.size();i++)
    {
        out << winners[i].winner_name << '\n';
        out << winners[i].win_count << '\n';
    }
}

void load_scoreboard()
{
    ifstream in("score.txt");

    int size;
    in >> size;
    for(int i = 0;i < winners.size();i++)
    {
        getline(in,winners[i].winner_name);
        in >> winners[i].win_count;
    }
}

int check_duplicate(string move)
{
    for(int i = 0;i < valid_moves.size();i++)
    {
        if(move == valid_moves[i])
            return 1;
    }
    return 0;
}