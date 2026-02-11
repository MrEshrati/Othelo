#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <thread>
#include <chrono>
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
void save_game(int mode);
void load_game();
void save_scoreboard();
void load_scoreboard();
int data();
void game_timer();
bool black_turn,computer=false,save,running=true;
int seconds,minutes,color_index=0;

// a struct for the scoreboard array(winners) including the winner's name and the number of times that player won
struct row {
    string winner_name;
    int win_count;
};

// a struct for the valid_moves array including the valid move_name and the color assigned to that valid move
struct valid_move {
    string move_name;
    string move_color;
};

// a vector containing all of the valid moves for the piece that it is it's turn to play
vector<valid_move> valid_moves;
// a vector containing all of the winners and the win count of each of them
vector<row> winners;
// a vector containing the ANSI color values to change cout color of the terminal
string colors[5] = {"\033[31m","\033[32m","\033[34m","\033[35m","\033[36m"};

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
        // Waits for the player to press one of the valid keys and it will be executed
        c = _getch();
        if(c == '0')
        {
            system("cls");
            start_game();
            system("cls");
            start_screen();
        }
        else if(c == '1')
        {
            if(data())
            {
                system("cls");
                load_game();
            }

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

/* This function creates the start section of the game indicating the options and modes and how to start the game
        and it will also create the scoreboard */
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
            cout << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
        else
        {
            cout << "       "<< winners[i-1].winner_name << " \u2502 " << winners[i-1].win_count << "\n";
        }
    }
}

/* This function is called once at the start of the game and fills the positions array with default pieces
        and it will also create the thread for the game_timer */
void start_game()
{
    running = true;
    seconds = 0;
    minutes = 0;
    thread t(game_timer);
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
    valid_plays("");
    play_turn();
    t.join();
}

/* This function will update the game board each time it is called and fill the game board with valid moves and places
        the pieces on the correct positions */
void update_board()
{
    cout << "\n\n";
    int x=0,y=0,skip=0,column=97,row=1,valid_m=0;
    for(int i = 0;i < 10;i++)
    {
        for(int j = 0;j < 64;j++)
        {
            if(i == 0)
                break;
            else if(i == 1)
            {
                if(j == 0)
                    cout << "\033[33m" << "\u250C" << "\033[0m";
                else if(j == 63)
                    cout << "\033[33m" << "\u2510" << "\033[0m";
                else
                {
                    if(j % 8 == 0)
                        cout << "\033[33m" << "\u252C" << "\033[0m";
                    else
                        cout << "\033[33m" << "\u2500" << "\033[0m";
                }
            }
            else if(i == 9)
            {
                if(j == 0)
                    cout << "\033[33m" << "\u2514" << "\033[0m";
                else if(j == 63)
                    cout << "\033[33m" << "\u2518" << "\033[0m";
                else
                {
                    if(j % 8 == 0)
                        cout << "\033[33m" << "\u2534" << "\033[0m";
                    else
                        cout << "\033[33m" << "\u2500" << "\033[0m";
                }
            }
            else
            {
                if(j == 0)
                    cout << "\033[33m" << "\u251C" << "\033[0m";
                else if(j == 63)
                    cout << "\033[33m" << "\u2524" << "\033[0m";
                else
                {
                    if(j % 8 == 0)
                        cout << "\033[33m" << "\u253C" << "\033[0m";
                    else
                        cout << "\033[33m" << "\u2500" << "\033[0m";
                }
            }
        }
        cout << "\n";
        
        if(i != 9)
        {
            for(int t = 0;t < 64;t++)
            {
                if(i == 0)
                {
                    if(t % 4 == 0 && t % 8 != 0)
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
                    if(t == 0)
                        cout << "\033[33m" << "\u2502" << "\033[0m";
                    else if(t == 63)
                        cout << "\033[33m" << "\u2502" << "\033[0m";
                    else
                    {
                        if(t % 8 == 0)
                            cout << "\033[33m" << "\u2502" << "\033[0m";
                        else
                        {
                            if(t % 4 == 0)
                            {
                                for(int k = 0;k < valid_moves.size();k++)
                                {
                                    if(x == char(valid_moves[k].move_name[1]-'0')-1 && y == char(valid_moves[k].move_name[0]) - 97)
                                    {
                                        y++;
                                        cout << valid_moves[k].move_color << valid_moves[k].move_name << "\033[0m";
                                        valid_m = 1;
                                        skip = 1;
                                        break;
                                    }
                                }

                                if(!valid_m)
                                {
                                    if(positions[x][y] != " ")
                                        skip = 1;
                                
                                    cout << positions[x][y++];
                                }
                            }
                            else if(skip)
                            {
                                int count = 0;
                                cout << "";
                                skip = 0;
                                valid_m = 0;
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
    cout << "Valid Moves: Shown on Board\n";
}

/* This function does 2 things,if the move is empty it will only fill up the valid_moves array 
    but if the move containes the player's move not only it will fill up the valid_moves,it will also
        check if the move is valid and if it is valid it will call the turn_pieces function to turn the surronded pieces */
void valid_plays(string move)
{
    valid_moves.clear();
    color_index = 0;
    int count;
    string turn,outturn;
    valid_move temp_valid;
     
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

                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1, char('a' + j - count)) + to_string(i+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
                                
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

                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j + count)) + to_string(i+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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

                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j)) + to_string(i-count+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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

                                    if(color_index > 4)
                                        color_index = 0;
                                    temp_valid.move_name = string(1,char('a' + j)) + to_string(i+count+1);
                                    temp_valid.move_color = colors[color_index++];
                                    valid_moves.push_back(temp_valid);
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

                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j + count)) + to_string(i-count+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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
                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j - count)) + to_string(i-count+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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

                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j + count)) + to_string(i+count+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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
                                if(color_index > 4)
                                    color_index = 0;
                                temp_valid.move_name = string(1,char('a' + j - count)) + to_string(i+count+1);
                                temp_valid.move_color = colors[color_index++];
                                valid_moves.push_back(temp_valid);
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

// Every time the player moves and it is a valid move, this function will be called to turn the color of the surounded pieces 
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

// This function will be called after the game starts and the loop inside it will continue the turns between two players until the game is over
void play_turn()
{
    bool GameOver=false,valid,black_out=false,white_out=false;
    while(!GameOver)
    {
        valid = false;
        valid_plays("");
        update_board();
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
                        if(move == valid_moves[i].move_name)
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
                    move = valid_moves[randomInt(valid_moves.size())].move_name;
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
                            if(move == valid_moves[i].move_name)
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
            save_game(1);

        if(black_out && white_out)
        {
            GameOver = true;
            running = false;
        }
    }

    result();
}

// After the game is over this function will count the pieces to determine the winner and then submit the winner's name in the scoreboard
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
            save_game(0);
            save_scoreboard();
            return ;
        }
    }
    row winner_data;
    winner_data.winner_name = winner;
    winner_data.win_count = 1;
    winners.push_back(winner_data);

    save_game(0);
    save_scoreboard();
}

// Generates a random number to use as the valid_playes index for the computer to play
int randomInt(int N) {
    static mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, N-1);
    return dist(gen);
}

// Saves the game into save.txt file which then can be loaded to continue the game from the save file
void save_game(int mode)
{
    ofstream out("save.txt");
    
    if(mode)
    {
        for(int i = 0;i < 8;i++)
            for(int j = 0;j < 8;j++)
                out << positions[i][j] << '\n';

        out << black_turn << '\n';
        out << computer << '\n';
        out << seconds << '\n';
        out << minutes << '\n';
    }
}

// Loads the game's previous save data to continue playing from where you left off
void load_game()
{
    ifstream in("save.txt");

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            getline(in, positions[i][j]);

    in >> black_turn;
    in >> computer;
    in >> seconds;
    in >> minutes;

    save = true;

    thread t(game_timer);
    play_turn();
    t.join();
}

// checks if the save.txt file exist and is not empty
int data()
{
    ifstream in("save.txt");

    if(in.good() && in.peek() != ifstream::traits_type::eof())
        return 1;
    else
        return 0;
}

// Saving winners to score.txt save file
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

// Loading saved winners from recent score.txt save file
void load_scoreboard()
{
    ifstream in("score.txt");

    int size;
    in >> size;

    row winner_data;
    for(int i = 0;i < size;i++)
    {
        in >> winner_data.winner_name;
        in >> winner_data.win_count;
        winners.push_back(winner_data);
    }

}

// Game Timer that runs on another thread which makes the program responsive even when the timer loop runs
void game_timer()
{
    while(running)
    {
        this_thread::sleep_for(chrono::seconds(1));
        if(seconds != 59)
            seconds++;
        else
        {
            minutes++;
            seconds = 0;
        }

        cout << "\033[s";
        cout << "\033[?25l";
        cout << "\033[2;30H";
        if(seconds >= 10)
            cout << "\033[34m" << minutes << " : " << seconds << "\033[0m";
        else
            cout << "\033[34m" << minutes << " : 0"<< seconds << "\033[0m";
        cout << "\033[u";
        cout << "\033[?25h";

    }
}