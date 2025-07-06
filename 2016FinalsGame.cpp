#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
using namespace std;


class Player{
    private:
        string name;
        int fg;
        int threePt;
        float freq;
        float threePtRate;
        float specRange;
        int pts;

    public:
        Player(string name = "", int fg = 0, int threePt = 0, float freq = 0, float threePtRate = 0, float specRange = 0, int pts = 0){
            this->name = name;
            this->fg = fg;
            this->threePt = threePt;
            this->freq = freq;
            this->threePtRate = threePtRate;
            this->specRange = specRange;
        }

        void setName(string name){
            this-> name = name;
        }
        void setFG(int fg){
            this->fg = fg;
        }
        void setThree(int threePt){
            this->threePt = threePt;
        }
        void setFreq(float freq){
            this->freq = freq;
        }
        void setThreePtRate(float threePtRate){
            this->threePtRate = threePtRate;
        }
        void setSpecRange(float specRange){
            this->specRange = specRange;
        }
        void updatePts(int num){
            this->pts += num;
        }
        string getName(){
            return name;
        }
        int getFG(){
            return fg;
        }
        int getThree(){
            return threePt;
        }
        float getFreq(){
            return freq;
        }
        float getThreePtRate(){
            return threePtRate;
        }
        float getSpecRange(){
            return specRange;
        }
        int getPts(){
            return pts;
        }
        void printPlayer(){
            cout << name << ": FG% = " << fg << ", 3pt% = " << threePt << ", Frequency = " << freq << ", 3pt rate = " 
            << threePtRate << ", Specified range = " << specRange << endl;
        }

};


class Team{
    private:
        string teamName;
        vector <Player> players;
        int points = 0;
    public:
        Team(string teamName = "", vector <Player> players = {}){
             this->teamName = teamName; 
             this->players = players;
        }

        void setName(string teamName){
            this->teamName = teamName;
        }
        void setPlayers(vector <Player> players){
            this->players = players;
        }
        string getName(){
            return teamName;
        }
        vector <Player> getPlayers(){
            return players;
        }
        int getScore(){
            return points;
        }
        void printScore(){
            cout << "\t" << teamName << "\'s score is " << points << endl;
        }
        void printTeam(){
            cout << teamName << ":" << endl;
            for(unsigned int i=0; i<5; i++){
                players[i].printPlayer();
            }
        }
        void setAllSpecRanges(){
            players[0].setSpecRange(1 - players[0].getFreq()); 
            players[1].setSpecRange(players[0].getSpecRange() - players[1].getFreq());
            players[2].setSpecRange(players[1].getSpecRange() - players[2].getFreq());
            players[3].setSpecRange(players[2].getSpecRange() - players[3].getFreq());
        }
        void generateShot(bool needPrint){
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0.0, 1.0);

            float n = dis(gen);
            int selectedPlayer;
            bool three = false;

            if(n >= players[0].getSpecRange()){
                selectedPlayer = 0;
            } else if(n >= players[1].getSpecRange()){
                selectedPlayer = 1;
            } else if(n >= players[2].getSpecRange()){
                selectedPlayer = 2;
            } else if(n >= players[3].getSpecRange()){
                selectedPlayer = 3;
            } else selectedPlayer = 4;

            n = dis(gen);
            if(n <= players[selectedPlayer].getThreePtRate()) three = true;

            n = dis(gen) * 100;
            bool scored = false;
            if(three == true){
                if(n <= players[selectedPlayer].getThree()){
                    points += 3;
                    players[selectedPlayer].updatePts(3);
                    scored = true;
                }
            } else {
                if(n <= players[selectedPlayer].getFG()){
                    points += 2;
                    players[selectedPlayer].updatePts(2);
                    scored = true;
                }
            }
            
            if(needPrint == true){
                cout << "\t" << players[selectedPlayer].getName() << " shoots a " << flush;
                if(three == true) cout << "3" << flush;
                else cout << "2" << flush;
                if(scored == true) cout << " and makes it!" << endl;
                else cout <<", but misses it" << endl;
            }

        }
        void shotAndOutput(){
            generateShot(true);
            cout << "\t" << teamName << "\'s score is " << points << endl;
        }
        void printLeadingScorer(){
            int max = 0;
            int selectedIndex = 0;
            for(unsigned int i=0; i<players.size(); i++){
                if(players[i].getPts() >= max){
                    max = players[i].getPts();
                    selectedIndex = i;
                } 
            }
            cout << "\t" << players[selectedIndex].getName() << " leads the " << teamName << " with " << players[selectedIndex].getPts() << endl;
        }
};


void printGameClock(int clock){
    string minutes = "";
    if((clock % 60) < 10) minutes += "0";
    minutes += to_string(clock % 60);
    cout << "\t" << to_string(clock / 60) << ":" << minutes <<
    " left in the game!" << endl;
}
void detectEnter(){
    cout << "\tPress enter to continue" << endl;
    cin.get();
}
void bothTeamPossession(Team &t1, Team &t2, bool output){
    t1.generateShot(output);
    t2.generateShot(output);
}
void printBothScores(Team t1, Team t2){
    t1.printScore();
    t2.printScore();
    cout << endl;
}
void quarter(Team &t1, Team &t2, bool output, int n){
    cout << "\tQuarter " << n << " is beginning..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    for(unsigned int i=0; i<25; i++){
        bothTeamPossession(t1, t2, output);
    }
    cout << "\tQuarter " << n << " is done!" << endl;
    printBothScores(t1, t2);
    if(n != 4) detectEnter();
    
}
void fiveMins(Team &t1, Team &t2, bool output, int &time){
    for(unsigned int i=0; i<9; i++){
        bothTeamPossession(t1, t2, output);
    }
    time -= 300;
    printGameClock(time);
    printBothScores(t1, t2);
    detectEnter();
}
void finalTwoMinutes(Team &t1, Team &t2, int &time){
    while(time > 40){
        for(unsigned int i=0; i<2; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0.0, 1.0);

            float n = dis(gen);
            if(n > 0.8) time -= 20;
            else if (time > 0.6) time -= 17;
            else if (time > 0.4) time -= 14;
            else if (time > 0.2) time -= 11;
            else time -= 8;
            if((i % 2) == 0) t1.generateShot(true);
            else t2.generateShot(true);
            this_thread::sleep_for(chrono::seconds(2));
        }
        cout << endl;
        printBothScores(t1, t2);
        this_thread::sleep_for(chrono::seconds(3));
        printGameClock(time);
        cout << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
}
void finalPossession(Team &t1, Team &t2, int &time){
    if(time > 24){
        cout << "\tFinal Cavs Posession:" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        t2.generateShot(true);
        this_thread::sleep_for(chrono::seconds(2));
        time = 20;
        printGameClock(time);

    }
    cout << "\tFinal Warriors Possession:" << endl << endl;
    this_thread::sleep_for(chrono::seconds(2));
    t1.generateShot(true);
    this_thread::sleep_for(chrono::seconds(2));
    printBothScores(t1, t2);
}
string determineWinner(Team t1, Team t2){
    if(t1.getScore() > t2.getScore()) return t1.getName();
    if(t1.getScore() < t2.getScore()) return t2.getName();
    else return "(Unfinished: Need overtime feature)";
}
void printStats(Team t1, Team t2, int n){
    if(n==1) cout << "\t" << "Here are some halftime stats: " << endl;
    else cout << "\t" << "Here are some final stats: " << endl;
    t1.printLeadingScorer();
    t2.printLeadingScorer();
    cout << endl;
    if(n==1) detectEnter();
}
void setTeams(Team &goldenState, Team &cleveland){
    Player stephCurry("Steph Curry", 50, 45, 0.330, 0.554);
    Player klayThompson("Klay Thompson", 47, 43, 0.283, 0.468);
    Player harrisonBarnes("Harrison Barnes", 47, 38, 0.157, 0.330);
    Player draymondGreen("Draymond Green", 49, 39, 0.165, 0.317); 
    Player andrewBogut("Andrew Bogut", 63, 12, 0.065, 0);

    Player kyrieIrving("Kyrie Irving", 45, 38, 0.259, 0.295);
    Player lebronJames("Lebron James", 56, 33, 0.291, 0.120);
    Player kevinLove("Kevin Love", 42, 36, 0.198, 0.449);
    Player jrSmith("JR Smith", 42, 40, 0.172, 0.6); 
    Player tristinThompson("Trisan Thompson", 59, 0, 0.080, 0);

    vector <Player> temp;

    temp.push_back(stephCurry);
    temp.push_back(klayThompson);
    temp.push_back(harrisonBarnes);
    temp.push_back(draymondGreen);
    temp.push_back(andrewBogut);
    goldenState.setName("Golden State Warriors");
    goldenState.setPlayers(temp);
    goldenState.setAllSpecRanges();

    temp.clear();
    temp.push_back(kyrieIrving);
    temp.push_back(lebronJames);
    temp.push_back(kevinLove);
    temp.push_back(jrSmith);
    temp.push_back(tristinThompson);
    cleveland.setName("Cleveland Cavaliers");
    cleveland.setPlayers(temp);
    cleveland.setAllSpecRanges();
}





int main() {
    cout << "\n\n" << flush;
    Team warriors;
    Team cavs;
    setTeams(warriors, cavs);

    // warriors.printTeam();
    // cout << endl;
    // cavs.printTeam();
    // cout << endl << endl << endl;

    cout << "\tWelcome to the 2016 NBA Finals simulator!" << endl;
    cout << "\tThis program will simulate a game from this iconic series" << endl;
    cout << "\tbetween LeBron and Steph!" << endl;
    cout << "\tPress ENTER/RETURN when you are ready to start!" << endl;
    cin.get();

    quarter(warriors, cavs, false, 1);
    quarter(warriors, cavs, false, 2);
    printStats(warriors, cavs, 1);
    quarter(warriors, cavs, false, 3);

    cout << "\tWould you like to " << endl;
    cout << "\t(1) Automatically simulate the entire 4th quarter" << endl;
    cout << "\tOR" << endl;
    cout << "\t(2) Slowly watch the end of the game play out" << endl;

    bool autoFourth = true;
    string answer = "";

    while(1){
        if(answer == "2"){
            autoFourth = false;
            break;
        }
        else if(answer == "1"){
            break;
        } else {
            cout << "\tPlease enter \'1\' or \'2\'" << endl;
            cin >> answer;
        }
    }
    if(autoFourth == true){
        quarter(warriors, cavs, false, 4);
        cout << "\t" << determineWinner(warriors, cavs) << " won the game!" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
        printStats(warriors, cavs, 2);
        this_thread::sleep_for(chrono::seconds(2));
    } else {
        int gameClock = 720;
        cout << "\tQuarter 4 is beginning..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //This line is needed to flush cin and make sure it is clear
        fiveMins(warriors, cavs, false, gameClock);

        cout << "\tQuarter 4 continuing..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        // cin.ignore(numeric_limits<streamsize>::max(), '\n'); //This line is needed to flush cin and make sure it is clear
        fiveMins(warriors, cavs, false, gameClock);

        cout <<"\tFinal two minutes!" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        finalTwoMinutes(warriors, cavs, gameClock);
        finalPossession(warriors, cavs, gameClock);
        this_thread::sleep_for(chrono::seconds(2));
        cout << endl;
        cout << "\t" << determineWinner(warriors, cavs) << " won the game!" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
        printStats(warriors, cavs, 2);
        this_thread::sleep_for(chrono::seconds(2));


        

    }

    cout << endl;
    return 0;
} 
