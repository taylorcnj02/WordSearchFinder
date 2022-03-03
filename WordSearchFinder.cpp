// Taylor Cheung
// Name of Program: Word Search
/* The program first asks the user to type the name of the file and the program will tell the user to
 retype or quit if it cannot be found. The program gets the number of rows and collumns and takes in the
 word search grid appropriately. The program then searches and tells the word found, coordinate location,
 and direction. It will output the words not found at the end.
 Change Log:
    2/15/21 - User friendly file input, get rid of all comments and blank lines, and take in the rows and
            collumns
    2/16/21 - Take in the matrix and used pattern search to find the words, iterate through the words using
            for loop and putting movie names in a vector
    2/17/21 - Uses if statements in search 2D function to determine the direction and print out, puts titles
            not found, in a vector and print out at the end.
 Special Features: usage of the peek function allowing to see the first char of the next line, push_back()
            function to add elements to vectors.
   
*/
#include <iostream>
#include<bits/stdc++.h>
#include<string>
#include <fstream>
using namespace std;

int rowNum, colNum;
bool wordFound;
//int coord[][];
vector <string> wordsNotFound;
bool openForReading(ifstream &infile, string fileName)
{
    infile.open(fileName);
    if (infile.good()) //then the file opened just fine
        return true;
    else
    {
        perror("Can't open the specified input file");
        cout << "The file name used was: " << fileName << endl;
        cout << "Enter another file name to use (or type 'quit'): ";
        getline(cin,fileName);
        if (fileName != "quit")
        {
            //maybe check for QUIT too
            cout << "--- The new file name is: " << fileName << endl;
            //note the use of recursion below
            bool wasItOpened = openForReading(infile,fileName);
            return wasItOpened;
        }
        else
        {
            cout << "Quitting at user's request." << endl;
            return false;
        }
    }
} //openForReading

// For searching in all 8 direction
int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

// This function searches in all 8-direction from point
// (row, col) in grid[][]
bool search2D(char grid[100][100], int row, int col, string word, string& direction)
{
    // If first character of word doesn't match with
    // given starting point in grid.
    if (grid[row][col] != word[0])
      return false;

    int len = word.length();
    

    // Search word in all 8 directions starting from (row,col)
    for (int dir = 0; dir < 8; dir++)
    {
        // Initialize starting point for current direction
        int k, rd = row + x[dir], cd = col + y[dir];

        // First character is already checked, match remaining
        // characters
        for (k = 1; k < len; k++) {
            // If out of bound break
            if (rd >= rowNum || rd < 0 || cd >= colNum || cd < 0)
                break;

            // If not matched,  break
            if (grid[rd][cd] != word[k])
                break;

            //  Moving in particular direction
            rd += x[dir], cd += y[dir];
            
            
        }
        // If all character matched, then value of must
        // be equal to length of word
        if (k == len)
        {
            // Using if else to determine the direction of which the movie title is found
            if(x[dir] == -1 && y[dir] == -1)
            {
                direction = "NW"; //SW
            }
            else if(x[dir] == -1 && y[dir] == 0)
            {
                direction = "N"; //W
            }
            else if(x[dir] == -1 && y[dir] == 1)
            {
                direction = "NE"; //NW
            }
            else if(x[dir] == 0 && y[dir] == -1)
            {
                direction = "W"; //S
            }
            else if(x[dir] == 0 && y[dir] == 1)
            {
                direction = "E"; // N
            }
            else if(x[dir] == 1 && y[dir] == -1)
            {
                direction = "SW"; //SE
            }
            else if(x[dir] == 1 && y[dir] == 0)
            {
                direction = "S"; //E
            }
            else if(x[dir] == 1 && y[dir] == 1)
            {
                direction = "SE"; //NE
            }
             return true;
        }
    }
    return false;
}

//  Searches given word in a given matrix in all 8 directions
void patternSearch(char grid[][100], string word,string originalTitle)
{
    string direction;
    // Consider every point as starting point and search
    // given word
    wordFound = false;
    for (int row = 0; row < rowNum; row++)
       for (int col = 0; col < colNum; col++)
          if (search2D(grid, row, col, word,direction))
          {
            cout << originalTitle <<" found at " << row+1 << ", "<< col +1 << ": (direction = " << direction << ") " <<endl;
            wordFound = true;
          }
}

//Removes the spaces in the movie titles
string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

//Function gets rid of comments and blank lines
void skipAnyCommentLines(ifstream &infile)
{
    char ch;
    string textLine;
    //showflow("skipAnyCommentLines");
    
    ch = infile.peek();
    
    //check for both comment lines and blank lines
    while (ch == '#' || ch == '\n')
    { //check for both comment lines and blank lines
        getline(infile,textLine); //ignore this line - just toss it
        ch = infile.peek(); //peek at first character of next line
    }
}

//Prints out the original file with comments and blank lines
void showTheFile(ifstream &inputFile)
{
    string oneLine;
    while (inputFile.good())
    {
        if (inputFile.peek() == '#')
        {
            //cout << "  -- here comes a comment line." << endl;
        }
        if (inputFile.eof()) return;
        getline(inputFile,oneLine);
        cout << oneLine << endl;
    }
}

//Changes the string to uppercase, this is used later for friendly user interface
void convertToUpper(string& s)
{
    for(int i = 0; i < s.length(); i++)
    {
        s[i] = toupper(s[i]);
    }
}




int main()
{
    ifstream infile;
    
    string fileName, getRidOf, currentTitle;
    string rowStr, col, row;
    string line;
    
    vector <string> movieTitles;
    vector <string> originalMovieTitles;
    
    cout << "Please enter the name of the file: " << endl;
    cin >> fileName;
    if (openForReading(infile,fileName))
    {
        //cout << "We opened the file just fine.\n";
        
        skipAnyCommentLines(infile);
        getline(infile, rowStr);
        
        int rowLocat = rowStr.find(" ");
        //getting the rows and collumn numbers in strings
        row = rowStr.substr(0,rowLocat);
        col = rowStr.substr(rowLocat);
        //converts the strings to numbers
        rowNum = stoi(row);
        colNum = stoi(col);
        //prints out the number of rows and collumns
        cout <<"Rows:" <<rowNum << " ";
        cout <<"Col:" << colNum << endl;
        
        //creates array to get word search grid
        char matrix [100][100];
        //gets rid of the next 2 blank lines
        getline(infile,getRidOf);
        getline(infile, getRidOf);
        for(int i = 0; i < rowNum; i++)
        {
            getline(infile,line);
            for(int j = 0; j < colNum; j++)
            {
                matrix[i][j] = line[j];
            }

        }
        skipAnyCommentLines(infile);
        string array[]= {};
        
        //Prints out the matrix for the word search part of the wordsearch
        for(int i = 0; i < rowNum; i++)
        {
            //getline(infile,line);
            //skipAnyCommentLines(infile);
            for(int j = 0; j < colNum; j++){
                //skipAnyCommentLines(infile);
                cout << matrix[i][j];
        }
            cout << endl;
        }
            cout << endl;

        //if it is not a comment take it in bc it is a movietitle
        while (infile.good())
        {
            if ( infile.peek() != '#'  )
            {
                getline(infile, currentTitle );
                movieTitles.push_back(currentTitle);
            }
            else
            //throw away the line bc it is a comment or blank line
            getline(infile, getRidOf);
            skipAnyCommentLines(infile);
        }
        //save the original format of the movie title for when it prints that it is found or not
        originalMovieTitles = movieTitles;
        for(int i = 0; i < movieTitles.size(); i++)
        {
            //takes spaces out of the titles and makes it uppercase
            movieTitles[i] = removeSpaces(movieTitles[i]);
            convertToUpper(movieTitles[i]);
            //cout << movieTitles[i] << endl;
            patternSearch(matrix, movieTitles[i],originalMovieTitles[i]);
            
            //if the word is not found get its unaltered name and save it in the array of words not found
            if (wordFound == false)
            {
                wordsNotFound.push_back(originalMovieTitles[i]);
            }
        }

        //Prints the words not found
        cout << "Couldn't find these movies:" << endl;
        for(int i = 0; i < wordsNotFound.size(); i++)
        {
            cout << wordsNotFound[i] << endl;
        }
    }
    // closes the file
    infile.close();
}

