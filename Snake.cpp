#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "randgen.h"
#include "strutils.h"
using namespace std;

struct coord
{
    int x;
    int y;
    coord* next;

    coord(int givenX, int givenY) : x(givenX), y(givenY), next(nullptr) {}
    bool operator== (coord rhs) 
    {
        if (x == rhs.x && y == rhs.y)
            return true;
        return false;
    }
};

struct Que
{
    coord* front;
    coord* tail;
    int size;

    Que() : front(nullptr), tail(nullptr), size(0) {}
    
    void Add(coord* NewLoc)
    {
        if (front == nullptr)
        {
            front = NewLoc;
            tail  = NewLoc;
        }
        else
        {
            NewLoc->next = front;
            front = NewLoc;
        }
        size++;
    }

    void Remove()
    {
        if (front == nullptr)
        {
            return;
        }
        if (front == tail)
        {
            tail = nullptr;
            delete front;
            front = nullptr;
            size--;
            return;
        }

        coord* temp = front;
        while (temp->next != tail)
        {
            temp = temp->next;
        }
        delete tail;
        temp->next = nullptr;
        tail = temp;
     
        size--;
    }

    ~Que()
    {
        coord* temp;
        while (front != nullptr)
        {
            temp = front;
            front = front->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }
};

bool IsThereSnake(const Que& Snake, int x, int y)
{
    coord* temp = Snake.front;
    for (int i = 0; i < Snake.size; i++)
    {
        if ((temp->x * 2 == x && temp->y == y) || (temp->x * 2 + 1 == x && temp->y == y ))
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void draw(const int BoardWidth, const int BoardLength, const Que &Snake, const coord &Food, const char dir, int Score)
{
    system("cls");
    string Board = "";


    for (int i = 0; i < ((BoardWidth + 2) * 2); i++)
    {
        Board += (char)178;
    }
    Board += "\n";


    for (int i = 1; i < BoardLength + 1; i++)
    {
        for (int j = 0; j < ((BoardWidth + 2) * 2); j++)
        {
            if (i == BoardLength / 2 && j == 43)
            {
                Board += (char)178;
                Board += "    Your score: ";
                Board += itoa(Score);
            }
            else if (j == 0 || j == 1 || j == 42 || j == 43)
            {
                Board += (char)178;
            }
            else if (IsThereSnake(Snake, j, i))
            {
                if (((Snake.front->y == i && (Snake.front->x * 2) == j) || (Snake.front->y == i && ((Snake.front->x * 2) + 1) == j)) && (dir == 'w' || dir == NULL))
                {
                    Board += "^";
                }
                else if (((Snake.front->y == i && (Snake.front->x * 2) == j) || (Snake.front->y == i && ((Snake.front->x * 2) + 1) == j)) && dir == 'a')
                {
                    Board += "<";
                }
                else if (((Snake.front->y == i && (Snake.front->x * 2) == j) || (Snake.front->y == i && ((Snake.front->x * 2) + 1) == j)) && dir == 's')
                {
                    Board += "Y";
                }
                else if (((Snake.front->y == i && (Snake.front->x * 2) == j) || (Snake.front->y == i && ((Snake.front->x * 2) + 1) == j)) && dir == 'd')
                {
                    Board += ">";
                }
                else
                {
                    Board += "#";
                }
            }
            else if ((Food.y == i && (Food.x *2) == j) || (Food.y == i && ((Food.x * 2) + 1) == j))
            {
                Board += (char)184;
            }
            else
            {
                Board += " ";
            }
        }
        Board += "\n";
    }

    for (int i = 0; i < ((BoardWidth + 2) * 2); i++)
    {
        Board += (char)178;
    }
    
    cout << Board;
}

coord CreateFood(int BoardWith, int BoardLength, const Que &Snake)
{
    RandGen random;
    bool InSnake = true;

    int FoodX = random.RandInt(1, BoardWith);
    int FoodY = random.RandInt(1, BoardLength);
    coord Food = coord(FoodX, FoodY);

    while (InSnake)
    {
        InSnake = false;
        FoodX = random.RandInt(1, BoardWith);
        FoodY = random.RandInt(1, BoardLength);
        Food = coord(FoodX, FoodY);

        coord* temp = Snake.front;
        for (int i = 0; i < Snake.size; i++)
        {
            if (*temp == Food)
            {
                InSnake = true;
                break;
            }
            temp = temp->next;
        }
    }

    return Food;
}

bool IsGameOver(int BoardWidth, int BoardLength, const Que& Snake)
{
    if (Snake.front->x < 1 || Snake.front->x > BoardWidth || Snake.front->y < 1 || Snake.front->y > BoardLength)
    {
        return true;
    }
    if (Snake.size > 4)
    {
        coord* temp = Snake.front->next;
        for (int i = 1; i < Snake.size; i++)
        {
            if (*Snake.front == *temp)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    return false;
}

coord setup(int BoardWidth, int BoardLength, Que &Snake)
{
    
    coord FirstFood = CreateFood(BoardWidth, BoardLength, Snake);
    coord* Head = new coord(BoardWidth / 2, BoardLength / 2);
    
    while (*Head == FirstFood)
    {
        coord FirstFood = CreateFood(BoardWidth, BoardLength, Snake);
    }
    
    Snake.Add(Head);

    return FirstFood;
}

void GetInput(char &dir, bool GameOver)
{
    char dummy;
    while (!GameOver)
    {
        cin >> dummy;

        if (dummy == 'w' && dir != 's')
        {
            dir = dummy;
        }
        else if (dummy == 'a' && dir != 'd')
        {
            dir = dummy;
        }
        else if (dummy == 's' && dir != 'w')
        {
            dir = dummy;
        }
        else if (dummy == 'd' && dir != 'a')
        {
            dir = dummy;
        }
        else if (dir == NULL)
        {
            dir = dummy;
        }
    }
}


int main()
{
    bool GameOver = false;
    int BoardWidth = 20;
    int BoardLength = 20;

    Que Snake;
    int Score = 0;
    char dir = NULL;

    
    coord Food = setup(BoardWidth, BoardLength, Snake);
    draw(BoardWidth, BoardLength, Snake, Food, dir, Score);
    thread Input(GetInput, ref(dir), GameOver);


    double fps = 2;
    double ElapsedTime = 0;
    int PreviousRound = 0;
    chrono::steady_clock::time_point StartingTime = chrono::steady_clock::now();
    while(!GameOver)
    {
        if ((int)(ElapsedTime / (1000 / fps)) > PreviousRound)
        {
            PreviousRound = (int)(ElapsedTime / (1000 / fps));
            
           
            if (*Snake.front == Food)
            {
                Score++;
                Food = CreateFood(BoardWidth, BoardLength, Snake);

            }
           
            if (dir == 'w')
            {
                coord* Head = new coord(Snake.front->x, Snake.front->y - 1);
                Snake.Add(Head);
                if (!(*Head == Food))
                {
                    Snake.Remove();
                }
            }
            else if (dir == 'a')
            {
                coord* Head = new coord(Snake.front->x - 1, Snake.front->y);
                Snake.Add(Head);
                if (!(*Head == Food))
                {
                    Snake.Remove();
                }
            }
            else if (dir == 's')
            {
                coord* Head = new coord(Snake.front->x, Snake.front->y + 1);
                Snake.Add(Head);
                if (!(*Head == Food))
                {
                    Snake.Remove();
                }
            }
            else if (dir == 'd')
            {
                coord* Head = new coord(Snake.front->x + 1, Snake.front->y);
                Snake.Add(Head);
                if (!(*Head == Food))
                {
                    Snake.Remove();
                }
            }      
            

            draw(BoardWidth, BoardLength, Snake, Food, dir, Score);
            
            GameOver = IsGameOver(BoardWidth, BoardLength, Snake);
        }
        chrono::steady_clock::time_point EndingTime = chrono::steady_clock::now();
        ElapsedTime = chrono::duration_cast<chrono::milliseconds>(EndingTime - StartingTime).count();
    }



    Input.detach();

    system("cls");
    cout << "Your score: " << Score << endl;
    cout << "Press ENTER to end the game." << endl;
    
    //char dummy;
    //cin >> dummy;

    return 0;
}