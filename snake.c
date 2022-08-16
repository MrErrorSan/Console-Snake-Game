#include "random.h"
#include "terminal.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int height = 0;             //Height of the game board
int width = 0;              //Width of the game board
int food_x = 0;             //Position of snake food y-axis
int food_y = 0;             // Position of snake food x-axis
struct Node *head = NULL;   //Stores the pointer of snake Link List
char snake_head = '\0';     //Keeps the head Symbol of snake in Link List
int food_eaten = 0;         //Keeps track of food eaten by snake
int reqiured_food = 0;      //Keeps track of food required to eat for win
int game_flag = 0;          //Check for game win or loss or continue
int is_food_present = 0;    //Tells if food is available on game for snake
int play_flag = 1;

struct Node
{
    char symbol;
    int x;
    int y;
    struct Node *next;
};
int countNode(struct Node *h)   //for snake bite case
{
    int count = 0;
    while (h->next != NULL)
    {
        // Starting from the next node
        struct Node *ptr = h->next;
        while (ptr != NULL)
        {
            // If some duplicate node is found
            if (h->x == ptr->x && h->y == ptr->y)
            {
                count++;
                break;
            }
            ptr = ptr->next;
        }
        h = h->next;
    }
    // Return the count of duplicate nodes
    return count;
}
struct Node *dec_tail(struct Node *o)   //for the moving snake
{
    if (o != NULL)
    {
        // Move the head pointer to the next node
        struct Node *temp = o;
        o = o->next;
        o->symbol = '#';    //New 1st node is tail
        free(temp); //deleting 1st node 
    }
    return o;
}
void inc_head(struct Node *o, char **board) //for moving snake and increasing length after eating food
{
    //Moving to the head of snake
    while (o->next != NULL)
    {
        o = o->next;
    }
    if (snake_head == '>')
    {
        o->symbol = '-';    //Replacing head with body(removing old head)
        o->next = (struct Node *)malloc(sizeof(struct Node));
        o->next->x = o->x;
        o->next->y = o->y + 1;
        if (o->next->y >= width)
        {
            game_flag = 2;
        }
        o->next->symbol = '>';  //Adding new head
        o->next->next = NULL;
    }//Same Working as above for different directions of snake head
    else if (snake_head == '<')
    {
        o->symbol = '-'; // Replacing head with body(removing old head)
        o->next = (struct Node *)malloc(sizeof(struct Node));
        o->next->x = o->x;
        o->next->y = o->y - 1;
        if (o->next->y < 0)
        {
            game_flag = 2;
        }
        o->next->symbol = '<'; // Adding new head
        o->next->next = NULL;
    }
    else if (snake_head == '^')
    {
        o->symbol = '|'; // Replacing head with body(removing old head)
        o->next = (struct Node *)malloc(sizeof(struct Node));
        o->next->x = o->x - 1;
        if (o->next->x < 0)
        {
            game_flag = 2;
        }
        o->next->y = o->y;
        o->next->symbol = '^'; // Adding new head
        o->next->next = NULL;
    }
    else if (snake_head == 'v')
    {
        o->symbol = '|'; // Replacing head with body(removing old head)
        o->next = (struct Node *)malloc(sizeof(struct Node));
        o->next->x = o->x + 1;
        if (o->next->x >= height)
        {
            game_flag = 2;
        }
        o->next->y = o->y;
        o->next->symbol = 'v'; // Adding new head
        o->next->next = NULL;
    }
    else
    {
        printf("Error Increasing head of snake\n"); //For curreption
        game_flag = 2;
    }
    if (food_x == o->next->x && food_y == o->next->y)//Snake eating food check
    {
        food_eaten++;       //Adding the eatten food
        is_food_present = 0;
        if (reqiured_food <= food_eaten)    //Checking for win condition
        {
            game_flag = 1;  //Enabling the win check
        }
        inc_head(o, board); //increasing length of snake from head
    }
}
void create_food(char **board)  //Creates food at random empty space on board
{
    initRandom();
    do
    {
        food_x = Random(1, height - 2);
        food_y = Random(1, width - 2);
    } while (board[food_x][food_y] != ' '); //food must not be under snake check
    board[food_x][food_y] = '@';    //Adding food to game board
    is_food_present = 1;
}
char **create_empty_board()     //intialization of game board
{
    char **board = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
        board[i] = (char *)malloc(width * sizeof(char));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            board[i][j] = ' ';
        }
    }
    return board;
}
void clear_snake(char **board)      //Clears Snake from board (used for updating snake position)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] == '#' || board[i][j] == '|' || board[i][j] == '-' || board[i][j] == '>' || board[i][j] == '<' || board[i][j] == '^' || board[i][j] == 'v') //Clearing only snake symbols from game board
                board[i][j] = ' ';
        }
    }
}
void check_snake_data(char **board, struct Node *snake)
{
    while(snake!=NULL)
    {
        if (snake->x < 0 || snake->x >= height || snake->y < 0 || snake->y >= width)        //Out of boundry checks
        {
            game_flag = 3;
        }
        snake=snake->next;
    }
}
void add_snake(char **board, struct Node *snake)    //Adds snake on game board from the Link List
{
    while (snake != NULL)
    {
        if(game_flag!=3)
        {
            board[snake->x][snake->y] = snake->symbol;  //link list to game board
            snake = snake->next;
        }
    }
}
void draw_game(char **board)    //Draw the game board on terminal with score
{
    if (is_food_present == 0 && game_flag == 0) //if food is not present for snake and game is not over yet
    {
        create_food(board);
        is_food_present = 1;
    }
    system("clear");    //Clearing the terminal
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
            {
                printf("*");    //Printing boundary outside the game board
            }
            else
            {
                printf("%c", board[i - 1][j - 1]);  //Printing data from game board
            }
        }
        printf("\n");
    }
    printf("Food Eaten : %i/%d \n", food_eaten, reqiured_food); //Current food status
}
struct Node *play_game(char **board, char input, struct Node *o)    //Use the user input and mov the snake on the gaming board
{
    int p_flag = 0; // if input is correct the move check
    if (input == 'a')
    {
        if (snake_head != '>')  //checking if head is not in 180*
        {
            p_flag++;
            snake_head = '<';
        }
    }
    else if (input == 'd')
    {
        if (snake_head != '<') // checking if head is not in 180*
        {
            p_flag++;
            snake_head = '>';
        }
    }
    else if (input == 'w')
    {
        if (snake_head != 'v')  // checking if head is not in 180*
        {
            p_flag++;
            snake_head = '^';
        }
    }
    else if (input == 's')
    {
        if (snake_head != '^') // checking if head is not in 180*
        {
            p_flag++;
            snake_head = 'v';
        }
    }
    else
    {
    }
    if (p_flag != 0)
    {
        o = dec_tail(o);    //decrease 1 node from snake tail
        inc_head(o, board); //mov/increase according to snake head symbol
    }
    if (countNode(o) != 0)  //if snake bites it self
    {
        game_flag = 2;      //Game over check
    }
    return o;
}

int main(int argc, char *argv[])
{
    head = (struct Node *)malloc(sizeof(struct Node)); // head pointer to keep track of the snake tail
    struct Node *t_head = head;                        // Creating Temprary Head of the link List
    //Starting File reading
    FILE *file;
    file = fopen(argv[1], "r"); //Opening file given in comandline arguments
    if (file == NULL)
    {
        printf("file can't be opened \n");
        return -1;
    }
    char str[10];
    int flag = 1;   //To help read only the first line
    while (fgets(str, 10, file) != NULL)
    {
        if (flag == 1)
        {
            // Reading game boundary
            char Height[5];
            char Width[5];
            int i = 0;
            int index = 0;
            for (; str[i] != ' '; i++)
            {
                Height[index] = str[i];
                index++;
            }
            Height[index] = '\0';
            index = 0;
            for (; str[i] != '\0'; i++)
            {
                Width[index] = str[i];
                index++;
            }
            Width[index] = '\0';
            width = atoi(Width);   // Setting the Width of Table to Draw
            height = atoi(Height); // Setting the Height of Table to Draw
            flag = 0;
        }
        else
        {
            // Reading Snake from File and Storng it in Link List
            char row[3];
            char col[3];
            char syb;
            int i = 0;
            int index = 0;
            for (; str[i] != ' '; i++)
            {
                row[index] = str[i];
                index++;
            }
            row[index] = '\0';
            index = 0;
            i++;
            for (; str[i] != ' '; i++)
            {
                col[index] = str[i];
                index++;
            }
            col[index] = '\0';
            i++;
            syb = str[i];
            // Storing Snake in Link List
            t_head->x = atoi(row);
            t_head->y = atoi(col);
            t_head->symbol = syb;
            //if last part of snake body then next is NULL and voice versa
            if (t_head->symbol == '>' || t_head->symbol == '<' || t_head->symbol == '^' || t_head->symbol == 'v')
            {
                snake_head = t_head->symbol;
                t_head->next = NULL;
            }
            else
            {
                t_head->next = (struct Node *)malloc(sizeof(struct Node));
                t_head = t_head->next;
            }
        }
    }
    //End of File reading
    fclose(file);       //Closing file
    
    reqiured_food = atoi(argv[2]);  //Setting Required Food indecator
    if (reqiured_food == 0)
    {
        game_flag = 1;      //Game Win Check
    }
    else
    {
        game_flag = 0;      //Game Continue Check
    }
    char **b = NULL;    //Pointer to store game board
    b = create_empty_board();   //Storing game board in b
    char in = '\0';     //variable to get user input while playing game
    int i = 0;
    check_snake_data(b,head);
    while (game_flag == 0)  //Game continue check
    {
        i++;
        clear_snake(b);     //clears snake from game board if any
        add_snake(b, head); //adds the snake on game board      <<<====
        draw_game(b);       //Drawsthe game board on terminal with updates
        if (game_flag == 0) // Game continue check
        {
            disableBuffer();    
            scanf("%c", &in);       //Taking user input
            head = play_game(b, in, head);  //Moving according to given input
            enableBuffer();
        }
    }
    draw_game(b);
    if (game_flag == 2) //Game over check
    {
        printf("<<< GAME OVER >>>\n   YOU LOSE :( \n-->Snake died\n");
    }
    else if (game_flag == 1)    //Game Win Check
    {
    	clear_snake(b);     //clears snake from game board if any
        add_snake(b, head); //adds the snake on game board      <<<====
        draw_game(b);  
        printf("YOU WIN :)\n");
    }else if(game_flag==3)  //Wrong data in .txt File
    {
        printf("\n*** Data Given in File cannot be used ***\n   (Out of boundry values of snake)\n");
    }
    return 0;
}
