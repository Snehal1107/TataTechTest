#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class Frame {
  private:
  int bonusRoll;
  vector<int>roll;
  public:
  Frame():bonusRoll(0){}
  void addRoll(int p)
 {
    if(p < 0 || p > 10)
    {
       throw invalid_argument("Invalid number of pins.");
    }
    roll.push_back(p);
 }
 bool strike() const
 {
    bool s;
     s = roll.size() >= 1 && roll[0] == 10;
     return s;
 }
 bool spare() const 
 {
    bool sp;
    sp = roll.size() >= 2 && (roll[0] + roll[1] == 10);
    return sp;
}
bool complete(int fIdx) const {
    if (fIdx == 9) {
        if (strike() || spare())
            return roll.size() == 3;
        else
            return roll.size() == 2;
    }
    return strike() || roll.size() == 2;
}
int getScore() const {
    int sum = 0;
    for (int pins : roll) {
        sum += pins;
    }
    return sum;
}
int firstRoll() const {
    if(roll.size() > 0)
       return roll[0];
    else
       return 0;
}

int secondRoll() const {
   if(roll.size() > 1)
     return roll[1];
   else
     return 0;
}

int thirdRoll() const {
    if(roll.size() > 2)
      return roll[2];
    else 
      return 0;
}
};
class Player
{
    private:
    string name;
    vector<Frame> frame;
public:
    Player(const string& playerName) : name(playerName) {}
 
    void addRoll(int pins) {
        if (frame.empty() || frame.back().complete(frame.size() - 1)) {
            if (frame.size() >= 10)
                throw logic_error("Game is already completed");
            frame.push_back(Frame());
        }
        frame.back().addRoll(pins);
    }
    int getTotalScore() const {
        int score = 0;
        for (size_t i = 0; i < frame.size(); ++i) {
            score += frame[i].getScore();
            if (i < 9) {
                if (frame[i].strike()) {
                    score += strikeBonus(i);
                }
                else if (frame[i].spare()) {
                    score += spareBonus(i);
                }
            }
        }
        return score;
    }
 
    bool isGameComplete() const {
        return frame.size() == 10 && frame.back().complete(9);
    }
 
private:
    int strikeBonus(size_t frameIndex) const {
        int bonus = 0;
        if (frameIndex + 1 < frame.size()) {
            bonus += frame[frameIndex + 1].firstRoll();
            if (frame[frameIndex + 1].strike() && (frameIndex + 2 < frame.size())) {
                bonus += frame[frameIndex + 2].firstRoll();
            } else {
                bonus += frame[frameIndex + 1].secondRoll();
            }
        }
        return bonus;
    }

    int spareBonus(size_t frameIndex) const {
        if (frameIndex + 1 < frame.size()) {
            return frame[frameIndex + 1].firstRoll();
        }
        return 0;
    }
};
class BowlingGame {
    private:
        Player player;
    public:
        BowlingGame(const string& playerName) : player(playerName) {}
     
        void roll(int pins) {
            try {
                player.addRoll(pins);
            } catch (const exception& ex) {
                cerr << "Error: " << ex.what() << endl;
            }
        }
     
        int getScore() const {
            return player.getTotalScore();
        }
     
        bool isGameOver() const {
            return player.isGameComplete();
        }
};

void runTestCase() {
    {
        BowlingGame game("Test1");
 
        for (int i = 0; i < 20; ++i)
            game.roll(1);
 
        cout << "Test Case 1 (All 1s): " << (game.getScore() == 20 ? "PASS" : "FAIL") << endl;
    }
 
    {
        BowlingGame game("Test2");
 
        for (int i = 0; i < 12; ++i)
            game.roll(10);
 
        cout << "Test Case 2 (Perfect Game): " << (game.getScore() == 300 ? "PASS" : "FAIL") << endl;
    }
 
    {
        BowlingGame game("Test3");
 
        for (int i = 0; i < 21; ++i)
            game.roll(5);
 
        cout << "Test Case 3 (All spares with 5): " << (game.getScore() == 150 ? "PASS" : "FAIL") << endl;
    }
 
    {
        BowlingGame game("Test4");
 
        game.roll(10);
        game.roll(9);
        game.roll(1);
        game.roll(5);
        game.roll(5);
        game.roll(7);
        game.roll(2);
        for (int i = 0; i < 12; ++i)
            game.roll(0);
 
        cout << "Test Case 4 (Mixed): " << (game.getScore() == 10 + 9 + 9 + 5 + 5 + 7 + 2 ? "PASS" : "FAIL") << endl;
    }
}

int main()
{
    cout << "Hello world." << endl;
    runTestCase();
    return 0;
    
} // namespace std;


