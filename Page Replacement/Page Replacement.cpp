#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int rfr[100], frm[100];
int n,pg,frame ;
string infile  = "input.dat"; // Input file
string outfile = "page Replacement.dat"; // Output file
fstream f;
ofstream fout; // to write on file
ifstream fin; // to read from file

// print the total page fault
void printPageFault(int pg)
{
    fout.open (outfile, ios::app);

    if (fout.is_open())
    {
        fout <<"\nPage Fault: "<< pg<<endl;
        fout.close();
        cout <<"\nPage Fault: "<< pg<<endl;
    }
}
// print and write on file on every page miss
int printFrame(int rfr, int frm[])
{
    fstream f;
    ofstream fout;
    fout.open (outfile, ios::app);
    fout <<"Page miss: " <<rfr<< " ----   ";
    cout <<"Page miss: " <<rfr<< " ----   ";
    for(int i=0; i<frame; i++)
    {
        if(frm[i] != -1)
        {
            fout <<frm[i] <<" ";
            cout <<frm[i] <<" ";
        }
    }
    fout<<endl;
    cout<<endl;
    fout.close();
}
// initialize an array with a single value
void initArray(int arr[], int length, int value)
{
    for(int i = 0; i<length; i++)   // assign value -1 to check
        arr[i] = value;
}
void fillFrame()  // insert the first pages into the empty frames
{
    for(int j=0; j<frame; j++)
    {
        frm[j] = rfr[j];
        printFrame(rfr[j],frm);
        pg++;
    }
}
// check if the page is already in the frames of not
// if present return true else false
bool alreadyPresent(int i)
{
    for(int j=0; j<frame; j++)
    {
        if(rfr[i] == frm[j])
            return true;
    }
    return false;
}
// select a page came earlier in the memory and
// replace it with new page
void fifo()
{
    initArray(frm, frame, -1); // initialize the frame to show valid output purposes
    int last = -1;
    pg=0;
    fillFrame(); // insert the first pages into the empty frames

    for(int i=frame; i<n; )
    {
        for(int j=0; j<frame; j++)
        {
            if(i==n)
                break;
            if(alreadyPresent(i))  // check if the page is already in the frames of not
            {
                i++;
                j--;
                continue;
            }
            frm[j] = rfr[i];
            pg++;

            printFrame(rfr[i],frm); // print on every page fault
            i++;
        }
    }
    printPageFault(pg); // print the page fault count
}
// select the page that will either not be used or used
// later compared to the available pages in the memory and
// replace that page with the new one.
void optimal()
{
    initArray(frm, frame, -1); // initialize the frame to show valid output purposes
    int last = -1;
    pg=0;

    fillFrame(); // insert the first pages into the empty frames

    for(int i=frame; i<n; i++)
    {
        if(alreadyPresent(i)) // check if the page is already in the frames of not
            continue;

        int value = 99999;
        int pos[frame], grater = -1;
        initArray(pos, frame, value); // initialize pos array with value 99999

        for(int j=0; j<frame; j++)  // to search the position of each frame value into the pages
        {
            for(int k=i; k<n; k++)
            {
                if(frm[j] == rfr[k] && pos[j] == value)
                {
                    pos[j] = k; // save the position of the frame once if it is found in the pages
                }
            }
            if(pos[j] > grater)
            {
                grater = pos[j];
                last = j;
            }
        }
        frm[last] = rfr[i]; //replace the frame value
        pg++;

        printFrame(rfr[i],frm); // print on every page fault
    }

    printPageFault(pg); // print the page fault count
}
// select the page that has not been used recently
// compared to the available pages in the memory and
// replace that page with the new one
void lru()
{
    initArray(frm, frame, -1); // initialize the frame to show valid output purposes
    pg=0;
    fillFrame(); // insert the first pages into the empty frames

    for(int i=frame; i<n; i++)
    {
        if(alreadyPresent(i)) // check if the page is already in the frames of not
        {
            continue;
        }
        for(int j=0; j<frame-1; j++)  // insert the last two pages into the frames
        {
            frm[j] = rfr[i-(frame-(j+1))];
        }
        frm[frame-1] = rfr[i]; // insert the new page into the last frame
        pg++;

        printFrame(rfr[i],frm); // print on every page fault
    }
    printPageFault(pg); // print the page fault count

}
// Clear  the output file to use it
void clearFile(string file)
{
    fout.open (file, ios::out | ios::trunc);
    fout.close();
}
// Reading inputs from a file
bool input()
{
    string data;
    fin.open(infile);
    fin >> data;
    if(data == "")
    {
        cout << "Unable to open file for reading \n" << endl;
        char s;
        cout<<"To solve this problem click 'S' this will create a file with a default input"<<endl;
        cout<<"or create a new text file named 'input.dat' or click any key to go to options and" <<endl;
        cout<<"write inputs 'Page references count, frame count and reference strings' on that file"<<endl;
        cout<<"Example 20 3\n7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1"<<endl;
        cin>>s;
        if(s == 's' || s == 'S')
        {
            clearFile(infile);
            fout.open(infile, ios::app);
            fout<<"20 3\n7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1"<<endl;
            fout.close();
            input();
        }
        else
            return false;
    }
    else
    {
        n = stoi(data);
        fin >> data;
        frame = stoi(data);
        for(int i=0; i<n; i++)
        {
            fin >> data;
            rfr[i] = stoi(data);
        }
        cout<<endl;
        fin.close();
        return true;
    }
}
int main()
{
    int choice;
    bool skip_first = false;
    int test_count = 0;
    clearFile(outfile);  // Clear  the output file to use it
    while(1)
    {
        cout<<"\nOptions \n1.Fifo \n2.Optimal \n3.LRU \n4.End \nEnter your choice: ";
        cin>>choice;
        if(choice != 4)
        {
            fout.open (outfile, ios::app); // Write on the start of a page replacement function call
            fout <<"\n----------------- Running test "<<test_count++<<" -----------------"<<endl;
            fout.close();
            cout <<"\n----------------- Running test "<<test_count<<" -----------------"<<endl;

            if(input())   // Read inputs from a file
            {
                if(choice == 1)
                    fifo();
                else if(choice == 2)
                    optimal();
                else
                    lru();
            }
        }
        else
            break;
    }
}
/*
20 3
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
*/

