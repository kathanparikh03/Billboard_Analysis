/*--------------------------------------------
    Program 4: Billboard Analysis
    CS 141, Spring 2022, UIC

    // This program will analyze weekly Billboard data spanning from 1960 - 2020.
    // The dataset covers the top 50 songs from "The Hot 100" charts over the time period of 1960-2020.
    // It is broken up into three different files by time period.
    // This program will have options to analyze this data in different ways.

    Course: CS 141, Spring 2022
    System: Windows and Mac using Visual Studio Code
    Author: Kathan Parikh, Sagar Patel
 ---------------------------------------------*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <vector>     // Input file is stored in a vector
#include <cassert>    // For the assert statement used to validate file open
#include <cctype>     // Allows using the tolower() function
#include <string>     // Includes string types, character traits and a set of converting functions
#include <algorithm>  // Defines functions that operates on ranges of elements
#include <sstream>    // Sort into strings
#include <cmath>      // Declares functions for mathematical operations and transformations
using namespace std;

// Class Songs for creating different objects on different data set based on timeframes
class Songs {
    // Class variables representing each coloumn set of data
    public:
        string fullDate; 
        int year; 
        int rank;
        string songName;
        string artistName;
        int lastWeek;
        int peakRank;
        int weeksOnBoard;
        
        // Parametrized constructor to set the value of each variable representing coloumns of data by casting it appropriately
        Songs(vector <string> rowData) 
        {
            int yearIntParsed = stoi(rowData.at(0).substr(rowData.at(0).size() - 4, 4));
            int rankIntParsed = stoi(rowData.at(1));
            int lastWeekIntParsed = stoi(rowData.at(4));
            int peakRankIntParsed = stoi(rowData.at(5));
            int weeksOnBoardIntParsed = stoi(rowData.at(6));

            // Refers to initial variables and sets the value appropriately for their purpose after passing vector (rowData)
            this -> fullDate = rowData.at(0); 
            this -> year = yearIntParsed; 
            this -> rank = rankIntParsed;
            this -> songName = rowData.at(2);
            this -> artistName = rowData.at(3);
            this -> lastWeek = lastWeekIntParsed; 
            this -> peakRank = peakRankIntParsed; 
            this -> weeksOnBoard = weeksOnBoardIntParsed;
        }
}; // end class Songs


// readFile function of type vector of class type for reading csv files.
// This function has one parameter taht takes the name of file.
// This function returns the contentData of that file passed by reading it.
vector <Songs> readFile(string fileName) 
{
    vector <Songs> contentData; 
    vector <string> rowData; 
    string line, element; 
    int i = 0; 
    fstream file(fileName, ios::in);

    if (file.is_open()) 
    {
        while (getline(file, line)) 
        {
            if ( i == 0) 
            {
                i++;
                continue;
            }
            rowData.clear();

            stringstream str(line);

            while (getline(str, element, ',')) 
            {
                if(element == "") 
                {
                    rowData.push_back("-1");
                }
                else 
                {
                    rowData.push_back(element);
                }
            }
            Songs insertNew = Songs(rowData);
            contentData.push_back(insertNew);
        }
    }
    return contentData;
} // end readFile


// This function displays six analysis options after selecting a file.
// This function has no parameters and return nothing.
void displayAnalyzeOptionMenu() 
{   
    cout << "Select a menu option: \n";
    cout << "   1. Display overall information about the data\n";
    cout << "   2. Display the Billboard info of the song with the most #1 occurrences for a given decade or year\n";
    cout << "   3. Display the Billboard info for the most long-lasting song on the charts for a given decade or year\n";
    cout << "   4. Display the Billboard info for the song with the greatest weekly climb on the charts given a decade or year\n";
    cout << "   5. Artist Search - Return the Billboard info for records that matches user input for artist search\n";
    cout << "   6. Exit\n";
    cout << "Your choice --> "; 
} // end displayAnalyzeOptionMenu


// This function pushes appropriate song name and artist name into vector based on year/decade.
// This function takes dataRecord vector, songnameVector, artistNameVector, decade, year, minYear, and maxYear as parameters.
// This function makes changes and modifies variables through pass by reference.
void pushSongName(vector <Songs> dataRecord, vector <string>& songNameVector, vector <string>& artistNameVector, int decade, int year, int minYear, int maxYear) 
{
    for(int i = 0; i < dataRecord.size(); i++) 
    {
        if((decade >= minYear) && (decade <= maxYear)) 
        {
            int remainderDateNum = decade % 10;
            minYear = decade - remainderDateNum; 
            maxYear = minYear + 9; 
            if((dataRecord.at(i).year >= minYear) && (dataRecord.at(i).year <= maxYear)) 
            {
                if(dataRecord.at(i).rank == 1) 
                {
                    songNameVector.push_back(dataRecord.at(i).songName);
                    artistNameVector.push_back(dataRecord.at(i).artistName);
                }     
            }
        }
        else if((year >= minYear) && (year <= maxYear)) 
        {
            if(dataRecord.at(i).year == year) 
            {
                if(dataRecord.at(i).rank == 1) 
                {
                    songNameVector.push_back(dataRecord.at(i).songName);
                    artistNameVector.push_back(dataRecord.at(i).artistName);
                }
            }
        }
    }
} // end pushSongName()


// This function returns the highest occurance (purpose) of a song (how many times a song is in vector).
// This function takes the parameter of songNameVector.
int returnHighestOccurance(vector <string> songNameVector) 
{
  int count = 0;
  string element = ""; 
  for(int j = 0; j < songNameVector.size(); j++) 
  {
    string tempElement = songNameVector.at(j);
    int tempCount = 0;
    for(int p = 0; p < songNameVector.size(); p++) 
    {
      if(songNameVector.at(p) == tempElement) 
      {
        tempCount++;
      }
    }
    if(tempCount > count) 
    {
      element = tempElement;
      count = tempCount;
    }
  }
  return count;
} // end returnHighestOccurance


// This function (purpose) prints the highest song name and occurance of a selected file.
// This function takes the parameter of songNamevector and artistNameVector.
void printSongNameAndOccurance(vector <string> songNameVector, vector <string> artistNameVector) 
{
        int highest_count = returnHighestOccurance(songNameVector);
        int count_song = 0; 
        vector <int> count_num_song;
        vector <string> new_song_list;
        vector <string> new_artist_list;
        
        // This loop pushes number of songs, appends current song into new song, and current artist into new artist vector.
        for(int j = 0; j < songNameVector.size(); j++) 
        {
            string current_song = songNameVector.at(j);
            string current_artist = artistNameVector.at(j);
            //If current song is already in the list continue to else append the song and artist name
            if((find(new_song_list.begin(), new_song_list.end(), current_song) != new_song_list.end()) && (find(new_artist_list.begin(), new_artist_list.end(), current_artist) != new_artist_list.end())) 
            {
                continue;
            }
            else 
            {
                int count_song = count(songNameVector.begin(), songNameVector.end(), current_song);
                count_num_song.push_back(count_song);
                new_song_list.push_back(current_song);
                new_artist_list.push_back(current_artist);
            }
        }

        // This loop prints the song title, artist, and #of occurance
        int statmentCount = 0;
        for(int i = 0; i < new_song_list.size(); i++) 
        {
            string song_artist = "";
            if(count_num_song.at(i) == highest_count) 
            {
                if(statmentCount == 0) 
                {
                  cout << endl << endl; 
                  cout << "The song(s) with the most #1 occurrences for selected time period is: " << endl;
                  statmentCount++;
                }
                string song_passed = new_song_list.at(i);
                song_artist = new_artist_list.at(i);
                cout << "        Song Title: " << song_passed << endl;
                cout << "        Artist: " << song_artist << endl;
                cout << "        # of occurrences: " << highest_count;
                cout << endl << endl;
            }
        }
} // end printSongNameAndOccurance


// This function (purpose) displays long lasting song within a decade or a year.
// The function takes dataRecord vector, decade, year, min year, and max year as parameters.
void displayLongLastingSong(vector <Songs> dataRecord, int decade, int year, int minYear, int maxYear) 
{
    int max = 0;
    string artistNameVector;
    string songNameVector;
    for(int i = 0; i < dataRecord.size(); i++) 
    {
        //Run if decade is selected
        if((decade >= minYear) && (decade <= maxYear)) 
        {
            int remainderDateNum = decade % 10;
            minYear = decade - remainderDateNum; 
            maxYear = minYear + 9; 
            if((max < dataRecord.at(i).weeksOnBoard) && (minYear <= dataRecord.at(i).year) && (maxYear >= dataRecord.at(i).year)) 
            {
                max = dataRecord.at(i).weeksOnBoard;
                songNameVector = dataRecord.at(i).songName;
                artistNameVector = dataRecord.at(i).artistName;
            }
        }

        //Run if year is selected
        else if((year >= minYear) && (year <= maxYear)) 
        {
            if(dataRecord.at(i).year == year) 
            {
                if(max < dataRecord.at(i).weeksOnBoard) 
                {
                    max = dataRecord.at(i).weeksOnBoard;
                    songNameVector = dataRecord.at(i).songName;
                    artistNameVector = dataRecord.at(i).artistName;  
                }
            }
        }
    }
    cout << "The song with the highest number of weeks on the charts is:" << endl;
    cout << "        "<<  songNameVector << " by " << artistNameVector << " with " << max << " weeks on the chart." << endl << endl;

} // end displayLongLastingSong()


// This function (purpose) displays a song with the greatest weekly clim within a decade or a year.
// This function takes dataRecord, decade, year, minYear, and maxYear as parameters. 
void greatestWeeklyClimb(vector <Songs> dataRecord, int decade, int year, int minYear, int maxYear) 
{
    vector <int> weeklyClimbDifference;
    vector <int> weeklyClimbIndex;
    vector <string> songNameVector_list;
    vector <string> artistNameVector_list;
    vector <string> date_list;
    vector <int> rank_list;
    vector <int> last_week_list;

    for(int i = 0; i < dataRecord.size(); i++) 
    {
        // Runs if user selects decade 
        if((decade >= minYear) && (decade <= maxYear)) 
        {
            int remainderDateNum = decade % 10;
            minYear = decade - remainderDateNum; 
            maxYear = minYear + 9; 
            if((dataRecord.at(i).year >= minYear) && (dataRecord.at(i).year <= maxYear)) 
            {
                int lastWeekRank = dataRecord.at(i).lastWeek; 
                int rank = dataRecord.at(i).rank;
                int difference = lastWeekRank - rank;
                weeklyClimbDifference.push_back(difference);
                songNameVector_list.push_back(dataRecord.at(i).songName);
                artistNameVector_list.push_back(dataRecord.at(i).artistName);
                date_list.push_back(dataRecord.at(i).fullDate);
                rank_list.push_back(dataRecord.at(i).rank);
                last_week_list.push_back(dataRecord.at(i).lastWeek);
            }
        }

        // Runs if user selects year and appends corresponding values to its vector
        else if((year >= minYear) && (year <= maxYear)) 
        {
            if(dataRecord.at(i).year == year) 
            {
                int lastWeekRank = dataRecord.at(i).lastWeek; 
                int rank = dataRecord.at(i).rank;
                int difference = lastWeekRank - rank;
                weeklyClimbDifference.push_back(difference);
                songNameVector_list.push_back(dataRecord.at(i).songName);
                artistNameVector_list.push_back(dataRecord.at(i).artistName); 
                date_list.push_back(dataRecord.at(i).fullDate);
                rank_list.push_back(dataRecord.at(i).rank);
                last_week_list.push_back(dataRecord.at(i).lastWeek);
            }
        }
    }
    // Check for the highest difference
    int biggestDifference = 0; 
    int index = 0;
    string songName;
    string artistName;
    string Weekof;
    //This loop finds the greatest weekly climb difference for the song
    for(int i = 0; i < weeklyClimbDifference.size(); i++) 
    {
        if(biggestDifference < weeklyClimbDifference.at(i)) 
        {
            biggestDifference = weeklyClimbDifference.at(i);
        }
    }

    //This loop prints out information if the greatest weekly climb found above matches
    int statmentCount = 0; 
    for(int i = 0; i < weeklyClimbDifference.size(); i++) 
    {
        if(biggestDifference == weeklyClimbDifference.at(i)) 
        {
            index = i;
            if(statmentCount == 0) 
            {
                cout << "The song(s) with the greatest climb from previous week to current week position: " << endl;
                statmentCount++;
            }
            cout << "       Song Title: " << songNameVector_list.at(index) << endl;
            cout << "        Artist: " << artistNameVector_list.at(index) << endl;
            cout << "        Week of: " << date_list.at(index) << endl;
            cout << "        Previous Week Position: " << last_week_list.at(index) << endl;
            cout << "        Current Week Position: " << rank_list.at(index) << endl;
            cout << "        Difference of " << biggestDifference << " between previous week and current week position" << endl;
        }
    }
} // end greatestWeeklyClimb


// This function (purpose) displays top 10 songs from a record for a particular artist.
// This function takes dataRecord as parameter.
void topTenArtistRecords(vector <Songs> dataRecord)
{
    string searchArtistName;
    string artistName; 
    string tempArtistName; 
    
    cout << "Enter search text to retrieve first 10 records of #1 songs that match by artist name: " << endl;

    cin.ignore();
    getline(cin, searchArtistName);
    transform(searchArtistName.begin(), searchArtistName.end(), searchArtistName.begin(), ::tolower);

    int count = 1;

    //This loop iterates over each row in the dataset.
    for (int i = 0; i < dataRecord.size(); i++) 
    {
        artistName = dataRecord.at(i).artistName;
        tempArtistName = artistName;
        transform(tempArtistName.begin(), tempArtistName.end(), tempArtistName.begin(), ::tolower); 
        
        // If artist name matches, rank equals 1 and count is less then 10 then print #1 songs upto 10. 
        if ((tempArtistName.find(searchArtistName) != string::npos) && (dataRecord.at(i).rank == 1)  && (count <= 10)) 
        {
            if (count == 1) 
            {
                cout << "Retrieval of first 10 #1 records found based upon search by artist name:" << endl; 
            }
            cout  << count << ") " << "Song: " << dataRecord.at(i).songName << endl;
            cout << "Artist: " << dataRecord.at(i).artistName << endl;
            cout << "Week of: " << dataRecord.at(i).fullDate << endl;
            cout << "Week Position: " << dataRecord.at(i).rank << endl << endl;
            count++;
        }
    }
    
    if(count == 1) 
    {
        cout << "No matching artists were found." << endl;
    }
} // end of topTenArtistRecords


// This function asks for year or decade input when called. If correct char is typed, valid char is set to true.
// This function takes decade, year, minYear, maxYear, validChar as parameters. 
void askForDecadeOrYear(int& decade, int& year, int minYear, int maxYear, bool& valid_char) 
{
    char timeFrameSelection;
    cout << "Enter D to select a decade or Y to select a year. " << endl;
    cout << "Your choice --> ";
    cin >> timeFrameSelection;
    while((toupper(timeFrameSelection) != 'D') && (toupper(timeFrameSelection) != 'Y')) 
    {
        cout << "Invalid entry. Try again. " << endl;
        cout << "Enter D to select a decade or Y to select a year. " << endl;
        cout << "Your choice --> ";
        cin >> timeFrameSelection;
    }
    if(toupper(timeFrameSelection) == 'D') 
    {
        cout << "Enter the decade you would like to choose, as a multiple of 10 (e.g. 2010)." << endl;
        valid_char = true;
        cin >> decade;
    }
    else if(toupper(timeFrameSelection) == 'Y') 
    {
        cout << "Enter the year you would like to choose (e.g. 2001)." << endl;
        valid_char = true;
        cin >> year; 
    }
    while (valid_char && ((decade < minYear) || (decade > maxYear)) && (year < minYear || year > maxYear)) 
    {
        cout << "No songs found in that timeframe. Be sure that the timeframe you enter is within the one covered by the dataset chosen." << endl << endl;
        break;
    }
} // end of askForDecadeOrYear


int main() 
{
    // Vector for storing data from 1960 to 1980
    vector <Songs> dataRecord1;
    dataRecord1 = readFile("charts_top50_1960_1980.csv"); 
    // Vector for storing data from 1981 to 2000
    vector <Songs> dataRecord2;
    dataRecord2 = readFile("charts_top50_1981_2000.csv"); 
    // Vector for storing data from 2001 to 2020
    vector <Songs> dataRecord3;
    dataRecord3 = readFile("charts_top50_2001_2020.csv"); 

    // User enters choice between 1 to 3 to run files accordingly 
    int choice; 
    cout << "Program 4: BillBoard Analysis \n";
    cout << "CS 141, Spring 2022, UIC \n";
    cout << " "<< endl;
    cout << "This program will analyze weekly Billboard data spanning from 1960 - 2020.\n";
    cout << endl;
    cout << "Select file option:\n";
    cout << "   1. To analyze charts top 50: 1960 - 1980 data file\n";
    cout << "   2. To analyze charts top 50: 1981 - 2000 data file\n";
    cout << "   3. To analyze charts top 50: 2001 - 2020 data file\n";
    cout << "Your choice --> ";
    cin >> choice;
    cout << endl;
    
    // Run if user selects file option #1
    if (choice == 1) 
    {
        int analyzeOptionFirst;
        int minYear = 1960;
        int maxYear = 1980;

        // Runs until users enters 6
        while (analyzeOptionFirst != 6) 
        {
            cout << endl;
            displayAnalyzeOptionMenu();
            cin >> analyzeOptionFirst;
            if (analyzeOptionFirst != 6) 
            {
                cout << endl;
            }
            
            // Checks if value is between 1 and 6, else prints an error message
            while((analyzeOptionFirst > 6) || (analyzeOptionFirst < 1)) 
            {
                cout << "Invalid value.  Please re-enter a value from the menu options below." << endl;
                cout << endl;
                displayAnalyzeOptionMenu();
                cin >> analyzeOptionFirst;
                if(analyzeOptionFirst != 6) 
                {
                    cout << endl;
                }
            }

            // Runs if user selects Anaylze option #1 in the first file
            if(analyzeOptionFirst == 1) 
            {
                vector <string> unique_song;
                for(int i = 0; i < dataRecord1.size(); i++) 
                {
                    string song = dataRecord1.at(i).songName;
                    if (find(unique_song.begin(), unique_song.end(), song) == unique_song.end()) 
                    {
                        unique_song.push_back(song);
                    }
                }
                cout << endl;
                cout << "Total number of rows: " << dataRecord1.size() << endl;
                cout << "Number of unique songs represented in the dataset: " << unique_song.size() << endl;
                cout << endl;
            } // Analyze 1 end

            // Runs if user selects Anaylze option #2 in the first file
            else if(analyzeOptionFirst == 2) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                vector <string> songNameVector;
                vector <string> artistNameVector;
                
                // Place Artist Name and Song name in vector
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    pushSongName(dataRecord1, songNameVector, artistNameVector, decade, year, minYear, maxYear);
                    printSongNameAndOccurance(songNameVector, artistNameVector);
                }
            
            } // Anaylze 2 end

            // Runs if user selects Anaylze option #3 in the first file
            else if(analyzeOptionFirst == 3) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    displayLongLastingSong(dataRecord1, decade, year,  minYear, maxYear);
                }
            } // Anaylze 3 end

            // Runs if user selects Anaylze option#4 in the first file
            else if(analyzeOptionFirst == 4) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    greatestWeeklyClimb(dataRecord1, decade, year, minYear, maxYear); 
                }
            } // Anaylze 4 end

            // Runs if user selects Anaylze option#5 in the first file
            else if (analyzeOptionFirst == 5) 
            {
                topTenArtistRecords(dataRecord1);
            } // Anaylze 5 end
        }
    } // end choice 1
    
    // Run if user selects file option #2
    else if (choice == 2) 
    {
        int analyzeOptionTwo;
        int minYear = 1980;
        int maxYear = 2000;
        while (analyzeOptionTwo != 6) 
        {
            displayAnalyzeOptionMenu();
            cin >> analyzeOptionTwo;
            cout << endl;
            while((analyzeOptionTwo > 6) || (analyzeOptionTwo < 1)) 
            {
            cout << "Invalid value.  Please re-enter a value from the menu options below." << endl;
            cout << endl;
            displayAnalyzeOptionMenu();
            cin >> analyzeOptionTwo;
            cout << endl;
            }
            
            // Runs if user selects Anaylze option #1 in the second file
            if(analyzeOptionTwo == 1) 
            {
                vector <string> unique_song;
                for(int i = 0; i < dataRecord2.size(); i++) 
                {
                    string song = dataRecord2.at(i).songName;
                    if (find(unique_song.begin(), unique_song.end(), song) == unique_song.end()) 
                    {
                        unique_song.push_back(song);
                    }
                }
                cout << endl;
                cout << "Total number of rows: " << dataRecord2.size() << endl;
                cout << "Number of unique songs represented in the dataset: " << unique_song.size() << endl;
                cout << endl;
            } // end Analyze 1

            // Runs if user selects Anaylze option #2 in the second file
            else if(analyzeOptionTwo == 2) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                vector <string> songNameVector;
                vector <string> artistNameVector;
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    //Function calls pushes song name and then prints song name and total occurance
                    pushSongName(dataRecord2, songNameVector, artistNameVector, decade, year, minYear, maxYear);
                    printSongNameAndOccurance(songNameVector, artistNameVector);
                }
            } // end Analyze 2

            // Runs if user selects Anaylze option #3 in the second file
            else if(analyzeOptionTwo == 3) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    // Function calls displays long lasting song based on year or decade.
                    displayLongLastingSong(dataRecord2, decade, year,  minYear, maxYear);
                }

            } // Anaylze 3 end

            // Runs if user selects Anaylze option #4 in the second file
            else if(analyzeOptionTwo == 4) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    //Function calls displays greatest weekly climb based off year or decade.
                    greatestWeeklyClimb(dataRecord2, decade, year, minYear, maxYear); 
                }
            } // Anaylze 4 end

            // Runs if user selects Anaylze option #5 in the second file
            else if (analyzeOptionTwo == 5) 
            {
                topTenArtistRecords(dataRecord2);
            } // end Analyze 5
        }
    } // end choice 2

    // Run if user selection file option #3
    else if (choice == 3) 
    {
        int analyzeOptionThree;
        int minYear = 2001;
        int maxYear = 2020;
        while (analyzeOptionThree != 6) 
        {
            displayAnalyzeOptionMenu();
            cin >> analyzeOptionThree;
            while((analyzeOptionThree > 6) || (analyzeOptionThree < 1)) 
            {
                cout << "Invalid value.  Please re-enter a value from the menu options below." << endl;
                cout << endl;
                displayAnalyzeOptionMenu();
                cin >> analyzeOptionThree;
                cout << endl;
            }
            
            // Runs if user selects Anaylze option #1 in the third file
            if(analyzeOptionThree == 1) 
            {
                vector <string> unique_song;
                for(int i = 0; i < dataRecord3.size(); i++) 
                {
                    string song = dataRecord3.at(i).songName;
                    if (find(unique_song.begin(), unique_song.end(), song) == unique_song.end()) 
                    {
                        unique_song.push_back(song);
                    }
                }
                cout << endl;
                cout << "Total number of rows: " << dataRecord3.size() << endl;
                cout << "Number of unique songs represented in the dataset: " << unique_song.size() << endl;
                cout << endl;
            } // end Analyze 1

            // Runs if user selects Anaylze option #2 in the third file
            else if(analyzeOptionThree == 2) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                vector <string> songNameVector;
                vector <string> artistNameVector;
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    pushSongName(dataRecord3, songNameVector, artistNameVector, decade, year, minYear, maxYear);
                    printSongNameAndOccurance(songNameVector, artistNameVector);
                }
            } // end Analyze 2

            // Runs if user selects Anaylze option #3 in the third file
            else if(analyzeOptionThree == 3) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    displayLongLastingSong(dataRecord3, decade, year,  minYear, maxYear);
                }
            } // Anaylze 3 end

            // Runs if user selects Anaylze option #4 in the third file
            else if(analyzeOptionThree == 4) 
            {
                char timeFrameSelection;
                int decade = 0;
                int year = 0;
                bool valid_char = false;
                askForDecadeOrYear(decade, year,  minYear, maxYear, valid_char);
                if((decade >= minYear && decade <= maxYear) || (year >= minYear && year <=maxYear)) 
                {
                    greatestWeeklyClimb(dataRecord3, decade, year, minYear, maxYear); 
                }
            } // Anaylze 4 end

            // Runs if user selects Anaylze option #5 in the third file
            else if (analyzeOptionThree == 5) 
            {
                topTenArtistRecords(dataRecord3);
            } // end Analyze 5
        }
    } // end choice 3
    
    // If use mistypes file option print invaid value
    else 
    {
        cout << "Invalid value. Exiting Program.";
        exit(0);
    }
    return 0; 
} // end main