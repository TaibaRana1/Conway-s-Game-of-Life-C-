#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

int x = 20;
int y = 20;
int sec_size = 0;
int sec_array_x[100];
int sec_array_y[100];
int nei_size = 300;
int nei_array[300][2];








void clearScreen()
{
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}






// This function counts the number of live neighbors for a given cell in the grid.
// it is essential for determining the next state of a cell

int countliveneighbour(int grid[][100],int x1, int y1)
{
    int liveneicount = 0;
    int nx[] = { -1,-1,- 1,0,0,1,1,1 };
    int ny[] = { -1,0,1,-1,1,-1,0,1 };


    for (int i = 0; i < 8; i++)
    {

            int neix = x1 + nx[i];
            int neiy = y1 + ny[i];
        

            if (neix >= 0 && neiy >= 0 && neix < x && neiy<x)
            {
                if (grid[neix][neiy] == 1)
                {
                    liveneicount++;
                }
            }

    }



    return liveneicount;
}







 void updatesecarray(int i, int j)    // This function updates the secondary array with live cell coordinates.
 {                                    //  This function helps in keeping track of live cells efficiently.
 
     sec_array_x[sec_size] = i;
     sec_array_y[sec_size] = j;
     sec_size++;
 
 
 }






 // This function deletes an element from the secondary array.

 void deletefromsecarray(int index)     
 {
     int x1 = sec_array_x[index];
     int y1 = sec_array_y[index];


     for (int i = index; i < sec_size-1; i++)
     {
         sec_array_x[i] = sec_array_x[i+1];
         sec_array_y[i] = sec_array_x[i+1];
         

     }
     sec_size--;





     // Remove dead neighboring cells from the nei_array
     for (int i = 0; i < nei_size; i++)
     {
         if (nei_array[i][0] == x1 && nei_array[i][1] == y1)
         {
             for (int j = i; j < nei_size - 1; j++)
             {
                  nei_array[j][0] = nei_array[j + 1][0];
                 nei_array[j][1] = nei_array[j + 1][1];
             }
             nei_size--;
         }
         else { i++; }

     }





 }


 




int checklivecell(int grid[][100], int x1, int y1)    // This function checks if a cell is alive or dead and updates the secondary array accordingly.
{                                                    //This function helps in managing the state of live cells efficiently.
                                                    

    if (grid[x1][y1] == 1)
    {
        updatesecarray(x1, y1);
    }

    else 
    {
        for (int i = 0; i < sec_size; i++)
        {
            if ((sec_array_x[i] == x1) && (sec_array_y[i] == y1))
            {
                deletefromsecarray(i);
                break;
            }

        }



    }



    return 1;
}







void displayboard(int grid[][100], int count)         // This function displays the game board/grid.
{
    cout << "Generation:" << count << endl;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < x; j++)
        {
            if (grid[i][j] == 1)
            {
                cout << "1 ";
            }
            else cout << ". ";
        }
        cout << endl;
    }




}








// This function resizes the grid to accommodate new live cells.
//  This function solves the problem of grid overflow when live cells exceed the current grid size.
void largergrid(int grid[][100], int grid2[][100], int& newsize)
{
    int newGrid[100][100] = { 0 };
    for (int i = 0; i < newsize; i++)
    {
        for (int j = 0; j < newsize; j++)
        {
            newGrid[i][j] = grid2[i][j];
        }
    }
   
    x = newsize;               //updating the grid size
    y = newsize;
   
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            grid[i][j] = newGrid[i][j];              // Copying new grid to the original grid
        }
    }
}

    

    










//This function updates the grid to the next generation based on the rules of Conway's Game of Life.
//This function calculates the next state of each cell and updates the grid accordingly.

void updategrid(int grid[][100], int grid2[][100])
{

    int newgrid = x;


    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            int livecell = checklivecell(grid, i, j);
            int liveneighbours = countliveneighbour(grid, i, j);

            if (grid[i][j] == 1)                                // if cells are alive
            {
                if (liveneighbours > 3 || liveneighbours < 2)        
                {
                    grid2[i][j] = 0;
                }
                else
                {
                    grid2[i][j] = 1;
                }

            }
            else            //if cells are dead
            {

                if (liveneighbours == 3)
                {
                    grid2[i][j] = 1;
                }
                else
                {
                    grid2[i][j] = 0;
                }

            }




            if (i == 0 || j == 0 || i == x - 1 || j == y - 1)    //checking if the cells touch the boundries
            {
                if (grid2[i][j] == 1)
                {
                    newgrid = newgrid + 2;
                }

            }


        }





    }



    if (newgrid > x)                           
    {    
       int newsize = newgrid + 5;
        largergrid(grid, grid2, newsize);           //calling function if grid needs to be expanded
        x = newsize;
        y = newsize;
    }
    else
    {
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < x; j++)
            {
                grid[i][j] = grid2[i][j];
            }
        }


    }




}






void generations(int grid[][100], int noofgen)         // This function simulates multiple generations of the game
{

    int grid2[100][100] ;
    int count = 0;

    for (int i = 0; i < noofgen; i++)
    {
        count++;                                 //generation count
        displayboard(grid,count);
        cout << endl;
        Sleep(500);                            //delay for 500 milliseconds
        clearScreen();                         //clearscreen function to clear the console
        updategrid(grid, grid2);

    }



}




void nei(int grid[][100])
{
    for (int k = 0; k < sec_size; k++)
    {
        int i = sec_array_x[k];
        int j = sec_array_y[k];




        int liveneicount = 0;
        int nx[] = { -1,-1,- 1,0,0,1,1,1 };
        int ny[] = { -1,0,1,-1,1,-1,0,1 };


    for (int i = 0; i < 8; i++)
    {

            int nx = i + sec_array_x[i];
            int ny = j + sec_array_y[i];
        

            if (!(nx == 0 && ny == 0) && nx >= 0 && nx < x && ny >= 0 && ny < x)
            {
                    if (grid[nx][ny] == 0)
                    {
                        bool zero = false;                                       //zero present
                        for (int k= 0; k < nei_size; k++)
                        {
                            if (nei_array[k][0] == nx && nei_array[k][1] == ny)
                            {
                                zero = true;
                                break;
                            }
                        }
                        if (!zero)
                        {
                            nei_array[nei_size][0] = nx;
                            nei_array[nei_size][1] = ny;
                            nei_size++;
                        }
                    }
            }

    }



    }

}






//This function initializes the grid with live cells based on input from a file.
// This function reads cell coordinates from a file and initializes the grid accordingly.

void initialgrid(int grid[][100])
{
    ofstream my_file("file.txt");               //file handling to initialize the grid

    if (!my_file)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    int noofgen, noofcells=5;
    cout << "Enter Number of Generations:";
    cin >> noofgen;

    my_file << noofgen << endl;
    my_file << noofcells << endl;

    my_file << 4 << " " << 2 << endl;
    my_file <<4 << " " << 3 << endl;
    my_file <<3 << " " << 4 << endl;
    my_file <<2 << " " <<3 << endl;
    my_file <<4<< " " << 4 << endl;

    my_file.close();

    ifstream file("file.txt");
    if (!file.is_open())
    {
        cout << "Unable to open file " << endl;
        return;
    }

    int gen , num_cells;
    file >> gen >> num_cells;


    int cx, cy;
    while (file >> cx >> cy)
    {
        cout << cx << " " << cy << endl;
        grid[cx][cy] = 1;

        

    }

     file.close();


     nei(grid);
    generations(grid, noofgen);



}



// This function serves as the entry point for the game. 
//This function initializes the grid and triggers the simulation of generations.

void  start()
{
    int grid[100][100] = {0};
    initialgrid(grid);           
}




int main()
{
    start();
    return 0;

}

 