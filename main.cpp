#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <array>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <bits/stdc++.h>
#include <conio.h>

#define inRed "\033[31m" //set the cout color to red
#define inGreen "\33[32m" //set the cout color to green
#define cReset "\033[0m" //reset the cout color


using namespace std;


int ratings[51][101]; //stores ratings(50 users and up to 100 movies). those unrated have the value 0
float ratingsCom[51][101]; //complementary array of ratings array(rated movies are valued 0 and unrated movies have predicted values)
float ratingsComDuplicate[51][101]; //ratingsCom copied to be used in calculations
int movieCount; //number of available movies
string moviename[101]; //stores movie names
string movieyear[101]; //stores movie years
string movieid[101]; //stores movie ids
int neighborsArray[9]; //storing neighbors' ids(for each user)
int topNArray[4]; //storing top 3 movie ids with maximum predicted ratings(for each user)
int cnt; //maximum amount of movie ids(any added movie's id is cnt+1)
string cunt; //string amount of integer cnt

void del_line(const char, int);
void inputMovies(); //inputs the Movies.txt file into the program
void inputRatings(); //inputs the Ratings.txt file into the program
void addMoviesText(string, string); //adds the added movie to the Movies.txt file, and reinputs the Movies.txt file
void changeRatingsText(); //updates the Ratings.txt file(by deleting the file, and recreating it by new figures)
void removeMoviesText(string ); //updates the Movies.txt file(by copying the file into another, skipping the line of the deleted movie, and renaming it to Movies.txt)
float sim(int , int ); //calculates the similarity between two users and returns it
void simDisplay(); //the screen for showing similarities(option 3 of main menu)
void neighbors(int ); //finds 8 neighbors of user and stores them in neighborsArray
void predictRates(); //finds predicted ratings of all users and stores them in ratingsCom[50][100]
void topN(int ); //finds 3 movies with maximum predicted ratings for recommendation to the user and storing them in topNArray[3]
void recDisplay(); //the screen for showing the 3 recommendations to the user and all predictions for user's unseen movies(option 4 of main menu)
void rateMovieDisplay(); //the screen for the user to change a movie's rate or rate the unseen movies(option 5 of main menu)
void ratingsDisplay(); //the screen for showing a user's movie ratings(option 2 of main menu)
void moviesDisplay(); //the screen for showing present movies(option 1 of main menu)
void addMovieDisplay(); //the screen for adding a movie(with it's name and year)
void removeMovieDisplay(); //the screen for removing a movie(by it's id or name)
void addremDisplay(); //the screen for choosing to add or remove movies(option 6 of main menu - directing to addMovieDisplay or removeMovieDisplay)

void mainmenuDisplay() {

    system("cls");
    int mainmenuOptionn;
    int mainmenuOption;
    cout << "\n\t\t\t\t\t  MOVIE RECOMMENDATION SYSTEM\n";
    cout << "\t\t\t\t\t      by Milad Rabiei\n\n";
    cout << "\t\t\t\t********** WELCOME TO THE MAINMENU **********\n\n";
    cout << "\t1 . Display Movies\n\n";
    cout << "\t2 . Display Ratings\n\n";
    cout << "\t3 . Display Similarity Between Two Users\n\n";
    cout << "\t4 . Generate Recommendation\n\n";
    cout << "\t5 . Rate a Movie\n\n";
    cout << "\t6 . Add/Remove a Movie\n\n";
    cout << "\t7 . Exit\n\n\n\n";
    while (true) {
        cout << "\n\tEnter Your Choice: ";
        mainmenuOptionn = _getch();
        mainmenuOption = mainmenuOptionn - 48;
        switch (mainmenuOption){
            case 1:{moviesDisplay();return;}
            case 2:{ratingsDisplay();return;}
            case 3:{simDisplay();return;}
            case 4:{recDisplay();return;}
            case 5:{rateMovieDisplay();return;}
            case 6:{addremDisplay();return;}
            case 7:{exit(0);}
            default:{cout << inRed << "\n\tThis option is invalid. Try Again..." << cReset;}
        }
    }

}


int main(){

    inputMovies();
    std::istringstream(movieid[movieCount]) >> cnt; //initial value
    inputRatings();

    mainmenuDisplay();

    return 0;
}


void del_line(const char *file_name, int n){
    ifstream fin(file_name);
    ofstream fout;
    fout.open("temp.txt", ios::out);
    char ch;
    int line = 1;
    while(fin.get(ch)){
        if(ch == '\n'){
          line++;
        }
        if(line != n){
          fout<<ch;
        }
    }
    fout.close();
    fin.close();
    remove(file_name);
    rename("temp.txt", file_name);
}

void inputRatings (){

    int part,i,j;
    int n=1;
    for(int u=0;u<=50;u++){
        for(int uu=0;uu<=100;uu++){
           ratings[u][uu]=0;
        }
    }
    for(int u=0; u<=50; u++){
        for(int uu=0; uu<=100; uu++){
            ratingsCom[u][uu]=0;
        }
    }
    fstream ratingfile;
    ratingfile.open ("Ratings.txt", ios::out | ios::in );
    while(ratingfile>>part){
        switch(n){
            case 1:{
                i = part;
                n++;
                break;
            }
            case 2:{
                j = part;
                n++;
                break;
            }
            case 3:{
                ratings[i][j] = part;
                n = 1;
                break;
            }
        }
    }
    ratingfile.close();
    predictRates();
}

void inputMovies(){

    string part;
    fstream moviefile;
    moviefile.open ("Movies.txt", ios::out | ios::in );
    int n=0, k=0;
    for(int i=0; i<=100; i++){
        movieid[i] = "";
        moviename[i] = "";
        movieyear[i] = "";
    }
    while(getline(moviefile,part)){
        n=0;
        k++;
        stringstream line(part);
        while (getline(line,part,'\t')){

            if(n==0){
                movieid[k]=part;
            }
            if(n==1){
                moviename[k]=part;
            }
            if(n==2){
                movieyear[k]=part;
            }

            n++;
        }
        movieCount = k;
    }
    moviefile.close();
}

void ratingsDisplay(){

    system("cls");
    int user;
    char ratingsDisplayOption;
    cout << "\n----------------\n";
    cout << "Display Ratings\n";
    cout << "----------------\n";
    while(true){
        cout << "\n\tEnter user id: ";
        cout << inGreen;
        cin >> user;
        cout << cReset;
        if(user>0&&user<51){
            break;
        }
        else{
            cout << inRed << "\n\tThis userid does not exist. Try Again...\n" << cReset;
        }
    }
    cout << "\n\tRatings of User " << inGreen <<  user << cReset << " are:\n\n";
    cout << "Movie id and name" << "\t\t\t\t\t\t" << "    Rating\n";
    cout << "-----------------------------" << "\t\t\t\t\t" << "  ----------\n";
    for (int i=1; i<=movieCount; i++){
        if (ratings[user][i]!=0){
                cout << movieid[i] << "\t" << left << setw(56) << moviename[i] << "\t" << left << setw(20) << ratings[user][i] << "\n";
        }
    }
    while(true){
        cout << "\n\n\tPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
        ratingsDisplayOption = _getch();
        switch (ratingsDisplayOption){
            case 'r':ratingsDisplay();return;break;
            case 'm':mainmenuDisplay();return;break;
            case 'q':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
}

float sim(int u, int v){

    float similarity=0.0, sim1=0.0, sim2=0.0, sim3=0.0;
    for (int b=1; b<=movieCount; b++){

        if(ratings[u][b]!=0 && ratings[v][b]!=0){

            sim1 = sim1 + (ratings[u][b] * ratings[v][b]);
            sim2 = sim2 + (ratings[u][b] * ratings[u][b]);
            sim3 = sim3 + (ratings[v][b] * ratings[v][b]);
        }
    }
    similarity = (sim1) / ((sqrt(sim2))*(sqrt(sim3)));
    return similarity;
}

void predictRates(){

    float predictedAmount = 0.0;
    float sum1=0.0, sum2=0.0;
    for (int i=1; i<=50; i++){
        neighbors(i);
        for (int j=1; j<=movieCount; j++){
            if(ratings[i][j]==0){
                sum1 = 0.0; sum2 = 0.0;
                for (int r=1; r<=8; r++){
                    if (ratings[neighborsArray[r]][j]!=0){
                        sum1 = sum1 + ratings[neighborsArray[r]][j]*sim(i,neighborsArray[r]);
                        sum2 = sum2 + sim(i,neighborsArray[r]);
                    }
                }
                predictedAmount = sum1 / sum2;
                ratingsCom[i][j] = predictedAmount;
                predictedAmount = 0.0;
            }
        }
    }
}

void neighbors(int u){

    float userSimilarities[51];
    for (int i=0; i<=8; i++){
        neighborsArray[i] = 0;
    }
    int r=0;
    float maximum;
    for (int i=0; i<=50; i++){
        userSimilarities[i]=0;
    }
    for (int j=1; j<=50; j++){
        userSimilarities[j] = sim(u,j);
    }
    userSimilarities[u] = 0;
    userSimilarities[0] = 0;
    for (int ii=1; ii<=8; ii++){
        for (int i=1; i<=50; i++){
            if(userSimilarities[i]>maximum){
                maximum = userSimilarities[i];
                r = i;
            }
        }
        neighborsArray[ii] = r;
        userSimilarities[r] = 0;
        maximum = 0;
    }
}

void topN(int u){

    predictRates();
    int selectedUser = 0;
    for(int y=0; y<=3; y++){
        topNArray[y] = 0;
    }
    for (int x=0; x<=50; x++){
        for (int y=0; y<=movieCount; y++){
            ratingsComDuplicate[x][y] = 0;
        }
    }
    for (int x=0; x<=50; x++){
        for (int y=0; y<=movieCount; y++){
            ratingsComDuplicate[x][y] = ratingsCom[x][y];
        }
    }
    float maximum = 0;
    topNArray[0] = 0;
    for (int i=1; i<=3; i++){
        for (int r=1; r<=movieCount; r++){
            if(ratingsComDuplicate[u][r]>maximum){
                maximum = ratingsComDuplicate[u][r];
                selectedUser = r;
            }
        }
        topNArray[i] = selectedUser;
        ratingsComDuplicate[u][selectedUser] = 0;
        maximum = 0;
        selectedUser = 0;
    }
}

void simDisplay(){

    system("cls");
    char simDisplayOption;
    int u, v;
    cout << "------------------------------------\n";
    cout << "Display Similarity Between Two Users\n";
    cout << "------------------------------------\n\n";
    while(true){
        cout << "\n\tEnter first user id: ";
        cout << inGreen;
        cin >> u;
        cout << cReset;
        if(u>0&&u<51){
            break;
        }
        else{
            cout << inRed << "\n\tThis userid does not exist. Try Again...\n" << cReset;
        }
    }
    while(true){
        cout << "\n\tEnter second user id: ";
        cout << inGreen;
        cin >> v;
        cout << cReset;
        if(v>0&&v<51){
            break;
        }
        else{
            cout << inRed << "\n\tThis userid does not exist. Try Again...\n" << cReset;
        }
    }
    cout << "\t\nSimilarity between User " << inGreen << u << cReset << " and User " << inGreen << v << cReset << " is: " << setprecision(3) << sim(u,v) << " ( "  << sim(u,v)*100 << " %" << " )" << "\n\n";
    while(true){
        cout << "\t\nPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
        simDisplayOption = _getch();
        switch (simDisplayOption){
            case 'r':simDisplay();return;break;
            case 'm':mainmenuDisplay();return;break;
            case 'q':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
}

void recDisplay(){

    system("cls");
    int user;
    char recDisplayOption;
    cout << "------------------------\n";
    cout << "Generate Recommendations\n";
    cout << "------------------------\n";
    while(true){
        cout << "\n\tEnter user id: ";
        cout << inGreen;
        cin >> user;
        cout << cReset;
        if(user>0&&user<51){
            break;
        }
        else{
            cout << inRed << "\n\tThis userid does not exist. Try Again...\n" << cReset;
        }
    }
    topN(user);
    cout << "\nTop three recommendations for User " << inGreen << user << cReset << " are: \n\n";
    cout << "movie id"<< "\t"<<"movie name"<< "\t\t\t\t\t"<< "year" << "\t" << "predicted rating\n";
    cout << "--------"<< "\t"<<"----------"<< "\t\t\t\t\t"<< "----" << "\t" << "----------------\n";
    for (int i=1; i<=3; i++){
        cout << topNArray[i]
        << "\t\t"<<left<<setw(48) << moviename[topNArray[i]]
        <<  movieyear[topNArray[i]]
        << "\t     " <<setprecision(3)<< ratingsCom[user][topNArray[i]] << "\n";
    }
    int c = 1;
    while(true){
        if(c==1){
            cout << "\n====what do you want to do?====\n";
            cout << "a.Display all predictions for this user\n";
            cout << "r.Retry\n";
            cout << "m.Back to main menu\n";
            cout << "q.exit\n";
            cout << "\nYour choice: ";
            recDisplayOption = _getch();
            switch(recDisplayOption){
                case 'a':{
                    cout << inGreen << recDisplayOption << cReset;
                    c = 0;
                    cout << "\n\n\tpredictions of unseen movies by User " << inGreen << user << cReset << " :\n\n";
                    cout << "\tmovie id\tpredicted rating\n";
                    cout << "\t--------\t----------------\n";
                    for (int i=1; i<=movieCount; i++){
                        if(ratingsCom[user][i]==0){
                            if(ratings[user][i]==0){

                                cout << "\t" << movieid[i] << "\t\t" << "Not Predictable\n";
                            }
                        }
                        else{
                            cout << "\t" << movieid[i] << "\t\t     " << setprecision(3)<<ratingsCom[user][i] << "\n";
                        }
                    }
                    break;
                }
                case 'r':recDisplay();return;break;
                case 'm':mainmenuDisplay();return;break;
                case 'q':exit(0);
                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
            }
        }
        else{
            cout << "\n====what do you want to do?====\n";
            cout << "r.Retry\n";
            cout << "m.Back to main menu\n";
            cout << "q.exit\n";
            cout << "\nYour choice: ";
            recDisplayOption = _getch();
            switch(recDisplayOption){
                case 'r':recDisplay();return;break;
                case 'm':mainmenuDisplay();return;break;
                case 'q':exit(0);
                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
            }
        }
    }
}

void moviesDisplay(){

    inputMovies();
    system("cls");
    char moviesDisplayOption;
    cout << "--------------\n";
    cout << "Display Movies\n";
    cout << "--------------\n\n\n";
    cout << "Movie id" << "\t" << "Movie name" << "\t\t\t\t\t\t\t" << "Year\n";
    cout << "--------" <<"\t" <<  "----------" << "\t\t\t\t\t\t\t" << "----\n";
    for (int i=1; i<=movieCount; i++){
        cout << movieid[i] <<"\t\t"<< left<<setw(56) << moviename[i] << "\t" << movieyear[i] << "\n";
    }
    while (true){
        cout << "\n\n\tPress 'm' to Main menu, 'q' to Quit ";
        moviesDisplayOption = _getch();
        switch (moviesDisplayOption){
            case 'm':mainmenuDisplay();return;break;
            case 'q':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again..." << cReset;}
        }
    }
}

void rateMovieDisplay(){

    system("cls");
    int sit=1;
    int suserid, smovieid, srate;
    char yesno, rateMovieOption;
    cout << "\n--------------\n";
    cout << "Rate A Movie\n";
    cout << "--------------\n";
    cout << "\nenter the information below:\n";
    cout << "============================\n";
    cout << "Userid: ";
    cout << inGreen;
    cin >> suserid;
    cout << cReset;
    cout << "\nMovieid: ";
    cout << inGreen;
    cin >> smovieid;
    cout << cReset;
    cout << "\nRating(1-5): ";
    cout << inGreen;
    cin >> srate;
    cout << cReset;
    cout << "\n============================\n";
    if(suserid<0||suserid>50){
        cout << inRed << "\nThis user id does not exist.\n" << cReset;
        while(true){
            cout << "\n\tPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
            rateMovieOption = _getch();
            switch (rateMovieOption){
                case 'r':rateMovieDisplay();return;break;
                case 'm':mainmenuDisplay();return;break;
                case 'q':exit(0);
                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
            }
        }
    }
    else if(srate<0||srate>5){
        cout << inRed << "\nThis rating is not valid. (only between 1-5)\n" << cReset;
        while(true){
            cout << "\n\tPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
            rateMovieOption = _getch();
            switch (rateMovieOption){
                case 'r':rateMovieDisplay();return;break;
                case 'm':mainmenuDisplay();return;break;
                case 'q':exit(0);
                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
            }
        }
    }
    else if(smovieid<0||smovieid>cnt){
        for(int i=1; i<=movieCount; i++){
            if(movieid[i]==to_string(smovieid)){
                sit = 0;
            }
        }
        if(sit!=0){
               cout << inRed << "\nThis movie id does not exist.\n" << cReset;
            while(true){
                cout << "\n\tPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
                rateMovieOption = _getch();
                switch (rateMovieOption){
                    case 'r':rateMovieDisplay();return;break;
                    case 'm':mainmenuDisplay();return;break;
                    case 'q':exit(0);
                    default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                }
            }

        }

    }

    int q = 1;
    while (q==1){
        cout << "\nDo you want to save (y/n)? ";
        yesno = _getch();
        switch(yesno){
            case 'y':{
                cout << inGreen << yesno << cReset;
                ratings[suserid][smovieid] = srate;
                changeRatingsText();
                cout << "\nThe record was successfully saved.\n";
                q = 0;
                break;
            }
            case 'n':{
                cout << inGreen << yesno << cReset;
                q = 0;
                break;
            }
            default:{cout << inGreen << yesno << cReset;cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
    while(true){
        cout << "\n\tPress 'r' to Retry, 'm' to Main menu and 'q' to Quit ";
        rateMovieOption = _getch();
        switch (rateMovieOption){
            case 'r':rateMovieDisplay();return;break;
            case 'm':mainmenuDisplay();return;break;
            case 'q':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
}

void addMovieDisplay(){

    system("cls");
    string name, year;
    char addMovieOption, yesno;
    int sit = 1;
    cout << "\n--------------\n";
    cout << "Add A New Movie\n";
    cout << "--------------\n";
    cout << "\n\nEnter the information below";
    cout << "\n====================";
    cout << "\nName: ";
    cout << inGreen;
    cin >> name;
    cout << cReset;
    cout << "\nYear: ";
    cout << inGreen;
    cin >> year;
    cout << cReset;
    cout << "\n====================";

    while(true){
        for(int i=1; i<=movieCount; i++){
            if (moviename[i]==name){
                sit = 0;
            }
            break;
        }
        if(sit = 0){
            cout << inRed << "\nThis movie already exists.\n" << cReset;
            while(true){
                cout << "\n\tPress 'r' to Retry, 'p' to Previous, 'm' to Main menu and 'q' to Quit ";
                addMovieOption = _getch();
                switch (addMovieOption){
                    case 'r':addMovieDisplay();return;break;
                    case 'p':addremDisplay();return;break;
                    case 'm':mainmenuDisplay();return;break;
                    case 'q':exit(0);
                    default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                }
            }
        }
        else{
            int q=1;
            while (q==1){
                cout << "\nDo you want to save (y/n)? ";
                yesno = _getch();
                switch(yesno){
                    case 'y':{
                        cout << inGreen << yesno << cReset;
                        cnt = cnt + 1;
                        addMoviesText(name, year);
                        cout << "\nThe record was successfully saved.\n";
                        q=0;
                        break;
                    }
                    case 'n':{
                        cout << inGreen << yesno << cReset;
                        q=0;
                        break;
                    }
                    default:{cout << inGreen << yesno << cReset;cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                }
            }
        }
        break;
    }
    while(true){
        cout << "\n\tPress 'r' to Retry, 'p' to Previous, 'm' to Main menu and 'q' to Quit ";
        addMovieOption = _getch();
        switch (addMovieOption){
            case 'r':addMovieDisplay();return;break;
            case 'p':addremDisplay();return;break;
            case 'm':mainmenuDisplay();return;break;
            case 'q':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
}

void removeMovieDisplay(){

    system("cls");
    inputMovies();
    char removeMovieOption, removeMovieOption2/*for a or b*/, yesno;
    string name, id;
    cout << "\n---------------";
    cout << "\nRemove A Movie";
    cout << "\n---------------";
    cout << "\n\n=====remove by name or id?=====";
    cout << "\na. By name\nb. By id";
    int q=1, sit=1;
    while(q==1){
        cout << "\nYour Choice: ";
        removeMovieOption2 = _getch();
        cout << inGreen << removeMovieOption2 << cReset;
        switch(removeMovieOption2){
            case 'a':{
                cout << "\nEnter the name of the movie: ";
                cout << inGreen;
                cin >> name;
                cout << cReset;
                while(true){
                    for(int i=1; i<=movieCount; i++){
                        if(moviename[i]==name){
                            sit = 0;
                            break;
                        }
                    }
                    if(sit==1){
                        cout << inRed << "\nThis movie does not exist." << cReset;
                        while(true){
                            cout << "\n\tPress 'r' to Retry, 'p' to Previous, 'm' to Main menu and 'q' to Quit ";
                            removeMovieOption = _getch();
                            switch (removeMovieOption){
                                case 'r':removeMovieDisplay();return;break;
                                case 'p':addremDisplay();return;break;
                                case 'm':mainmenuDisplay();return;break;
                                case 'q':exit(0);
                                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                            }
                        }
                    }
                    break;
                }

                int qq=1;
                while (qq==1){
                    cout << "\nAre you sure you want to delete \"" << name << "\" (y/n)? ";
                    yesno = _getch();
                    switch(yesno){
                        case 'y':{
                            cout << inGreen << yesno << cReset;
                            for(int i=1; i<=movieCount; i++){
                                if(moviename[i]==name){
                                    removeMoviesText(movieid[i]);
                                    for(int j=1; j<=50; j++){
                                        ratings[j][i] = -1;
                                        ratingsCom[j][i] = -1;
                                    }
                                    changeRatingsText();
                                    break;
                                }
                            }
                            cout << "\nThe record was successfully removed.";
                            movieCount = movieCount - 1;
                            qq=0;
                            break;
                        }
                        case 'n':{
                            cout << inGreen << yesno << cReset;
                            qq=0;
                            break;
                        }
                        default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                    }
                q=0;
                }
                break;
            }
            case 'b':{

                cout << "\nEnter movie id: ";
                cout << inGreen;
                cin >> id;
                cout << cReset;
                while(true){
                    for(int i=1; i<=movieCount; i++){
                        if(movieid[i]==id){
                            sit = 0;
                            break;
                        }
                    }
                    if(sit==1){
                        cout << inRed << "\nThis movie does not exist." << cReset;
                        while(true){
                            cout << "\n\tPress 'r' to Retry, 'p' to Previous, 'm' to Main menu and 'q' to Quit ";
                            removeMovieOption = _getch();
                            switch (removeMovieOption){
                                case 'r':removeMovieDisplay();return;break;
                                case 'p':addremDisplay();return;break;
                                case 'm':mainmenuDisplay();return;break;
                                case 'q':exit(0);
                                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
                            }
                        }
                    }
                    break;
                }
                int qq=1;
                while (qq==1){
                    cout << "\nAre you sure you want to delete movie \"" << id << "\" (y/n)? ";
                    yesno = _getch();
                    switch(yesno){
                        case 'y':{
                            cout << inGreen << yesno << cReset;
                            for(int i=1; i<=movieCount; i++){
                                if(movieid[i]==id){
                                    removeMoviesText(id);
                                    for(int j=1; j<=50; j++){
                                        ratings[j][i] = -1;
                                        ratingsCom[j][i] = -1;
                                    }
                                    changeRatingsText();
                                    break;
                                }
                            }
                            cout << "\nThe record was successfully removed.";
                            movieCount = movieCount - 1;
                            qq=0;
                            break;
                        }
                        case 'n':{
                            cout << inGreen << yesno << cReset;
                            qq=0;
                            break;
                        }
                        default:{cout << inGreen << yesno << cReset;cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}}
                }
                q=0;
                break;
            }
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}}
        }
        while(true){
            cout << "\n\tPress 'r' to Retry, 'p' to Previous, 'm' to Main menu and 'q' to Quit ";
            removeMovieOption = _getch();
            switch (removeMovieOption){
                case 'r':removeMovieDisplay();return;break;
                case 'p':addremDisplay();return;break;
                case 'm':mainmenuDisplay();return;break;
                case 'q':exit(0);
                default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
            }
        }
}

void addremDisplay(){

    system("cls");
    char addremDisplayOption;
    cout << "\n-----------------------";
    cout << "\nAdd/Remove A Movie";
    cout << "\n-----------------------";
    cout << "\n=====what do you want to do?=====";
    cout << "\na. Add a new movie";
    cout << "\nb. Remove a movie";
    cout << "\nc. Back to main menu";
    cout << "\nd. Exit";
    while (true){

        cout << "\n\nYour choice? ";
        addremDisplayOption = _getch();
        switch(addremDisplayOption){
            case 'a':addMovieDisplay();return;break;
            case 'b':removeMovieDisplay();return;break;
            case 'c':mainmenuDisplay();return;break;
            case 'd':exit(0);
            default:{cout << inRed << "\n\tThis option is invalid. Try Again...\n" << cReset;}
        }
    }
}

void addMoviesText(string name, string year){

    ofstream file;
    file.open("Movies.txt", ios::app);
    cunt = to_string(cnt);
    file << "\n" << cunt << "\t" << name << "\t" << year
        << "\t" << "00-00-0000"
        << "\t" << "link"
        << "\t" << "genre";
    file.close();
    inputMovies();
}

void removeMoviesText(string id){

    int intid;
    string part;
    fstream efile;ofstream nfile;
    efile.open ("Movies.txt", ios::out | ios::in );
    nfile.open ("Movies2.txt");
    int n = 0, sit = 1;
    while(getline(efile, part)){
        n=0;
        stringstream line(part);
        while (getline(line, part, '\t')){
            if (n==0){
                if (part==id){
                	sit = 0;
                    break;
                }
            }
			nfile << part << "\t";
        }
        if(sit!=0){
        	nfile << "\n";
		}
        n++;
	}
    efile.close();
    nfile.close();
    remove("Movies.txt");
    rename("Movies2.txt","Movies.txt");
    std::istringstream(id) >> intid;
    if(intid==(movieCount)){
        del_line("Movies.txt",movieCount);
    }
    inputMovies();
}

void changeRatingsText(){

    remove("Ratings.txt");
    ofstream ratingfile;
    ratingfile.open("Ratings.txt");
    for(int i=1; i<=50; i++){
        for (int j=1; j<=movieCount; j++){
            if(ratings[i][j]!=0 || ratings[i][j]!=-1){
                ratingfile << i << "\t" << j << "\t" << ratings[i][j] << "\n";
            }
        }
    }
    ratingfile.close();
    inputRatings();
}






