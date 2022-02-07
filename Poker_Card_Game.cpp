/********************************************************************
Course		:	CO2210 Programming Quest
Quest No	:	04
Title		:	Poker Card Game
Author		:	Kasthuriarachchi K.A.D.G.
Index No	:	19/ENG/053
Date		:	06-10-2021
********************************************************************/

#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>

using namespace std;

/*---------------------------------------------------------------------------------
Function            :	random_number
Description         :	generating a random number within a given inclusive range
Input Parameters    :	min - lower limit of the random number
						max - upper limit of the random number
Return              :	generated random number
---------------------------------------------------------------------------------*/
int random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

/*---------------------------------------------------------------------------------
Function            :	get_now
Description         :	generating a string with real time date and time
Input Parameters    :	-
Return              :	generated sting with date and time
---------------------------------------------------------------------------------*/
string get_now()
{
	struct tm dt;
	time_t now = time(0);
	localtime_s(&dt, &now);
	return to_string(1900 + dt.tm_year) + "." + to_string(1 + dt.tm_mon) + "." + to_string(dt.tm_mday) + " @ " + to_string(dt.tm_hour) + "." + to_string(dt.tm_min) + "." + to_string(dt.tm_sec);
}

//structure to handle card data
struct Card
{
	//valus considered as Spades = 0, Diamonds = 1, Hearts = 2, Clubs = 3
	int suit;
	int value;

	Card(int s, int v) :suit(s), value(v) {};

	/*---------------------------------------------------------------------------------
	Member Function		:	print_card
	Description         :	output stream the card details
	Input Parameters    :	output - output stream type
	Return              :	-
	---------------------------------------------------------------------------------*/
	void print_card(ostream& output)
	{
		if (value > 1 && value < 11)
			output << value;
		else if (value == 11)
			output << "J";
		else if (value == 12)
			output << "Q";
		else if (value == 13)
			output << "K";
		else if (value == 14)
			output << "A";
		output << "-";
		if (suit == 0)
			output << "S";
		else if (suit == 1)
			output << "D";
		else if (suit == 2)
			output << "H";
		else if (suit == 3)
			output << "C";
	}
};

//class to handle card pack data and functions
class Card_pack
{
private:
	vector<Card> card_pack;
public:

	/*---------------------------------------------------------------------------------
	Member Function		:	initialize_card_pack
	Description         :	initialize the card pack with all 52 cards
	Input Parameters    :	-
	Return              :	-
	---------------------------------------------------------------------------------*/
	void initialize_card_pack()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 2; j < 15; j++)
				card_pack.push_back(Card(i, j));
		}
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	shuffle
	Description         :	shuffle the card pack randomly
	Input Parameters    :	-
	Return              :	-
	---------------------------------------------------------------------------------*/
	void shuffle()
	{
		int size = int(card_pack.size());
		for (int i = 0; i < size; i++)
			swap(card_pack[i], card_pack[random_number(0, size - 1)]);
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	remove_two
	Description         :	remove last two cards from the card pack
	Input Parameters    :	-
	Return              :	-
	---------------------------------------------------------------------------------*/
	void remove_two()
	{
		card_pack.pop_back();
		card_pack.pop_back();
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	give_cards
	Description         :	returns a given number of random cards from the card pack 
							and removes them from the pack
	Input Parameters    :	num - number of cards needed
	Return              :	vector of the giving cards
	---------------------------------------------------------------------------------*/
	vector<Card> give_cards(const int &num)
	{
		vector<Card> set;
		for (int i = 0; i < num; i++)
		{
			set.push_back(card_pack.back());
			card_pack.pop_back();
		}
		return set;
	}
};

//strucrure to handle player data
struct Player
{
	string name;
	int marks;
	float win_probability;
	vector<Card> hand;

	Player(string n = "", int m = 0, float p = 0) :name(n), marks(m), win_probability(p) {}
};

//class to handle Poker round data and functions
class Poker_round
{
private:
	int round_no;
	vector<Player> players;
public:
	Poker_round(int r, vector<Player>& p) { round_no = r; players = p; }

	/*---------------------------------------------------------------------------------
	Member Function		:	give_marks
	Description         :	gives the mark according to the combination and 
							the highest card contibuting to the combination
							ex:	if combination mark is 6 and highes card of the 
								combination is 6, the total mark is (6+(6-2)/100)=6.04
	Input Parameters    :	p - player to give marks
	Return              :	total mark according to the combination and its highest card
	---------------------------------------------------------------------------------*/
	float give_marks(Player p) const
	{
		vector<Card> hand = p.hand;
		int suits[4] = { 0 }, values[13] = { 0 };

		for (int i = 0; i<int(hand.size()); i++)
		{
			suits[hand[i].suit]++;
			values[hand[i].value - 2]++;
		}

		for (int i = 0; i < 4; i++)
			if (suits[i] == 5)
				for (int j = 12; j >= 0; j--)
					if (values[j] == 1)
						return (9 + float(j) / 100);

		for (int i = 0; i < 13; i++)
			if (values[i] == 4)
				return (8 + float(i) / 100);

		for (int i = 0; i < 4; i++)
			if (suits[i] == 3)
				for (int j = 0; j < 4; j++)
					if (suits[j] == 2)
						for (int k = 12; k >= 0; k--)
							if (values[k] == 1)
								return (7 + float(k) / 100);

		if (suits[0] + suits[3] == 5 || suits[1] + suits[2] == 5)
			for (int j = 12; j >= 0; j--)
				if (values[j] == 1)
					return (6 + float(j) / 100);

		if (values[12] == 1 && values[0] == 1 && values[1] == 1 && values[2] == 1 && values[3] == 1)
			return (5 + float(3) / 100);
		for (int i = 0; i < 9; i++)
			if (values[i] == 1 && values[i + 1] == 1 && values[i + 2] == 1 && values[i + 3] == 1 && values[i + 4] == 1)
				return (5 + float(i + 4) / 100);

		for (int i = 0; i < 13; i++)
			if (values[i] == 3)
				return (4 + float(i) / 100);

		int count = 0, card_val = 0;
		for (int i = 0; i < 13; i++)
			if (values[i] == 2)
			{
				count++;
				card_val = i;
			}
		if (count == 2)
			return (3 + float(card_val) / 100);

		for (int i = 0; i < 13; i++)
			if (values[i] == 2)
				return (2 + float(i) / 100);

		for (int i = 12; i >= 0; i--)
			if (values[i] == 1)
				return (1 + float(i) / 100);
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	winning_player
	Description         :	finds the winning player of the round
	Input Parameters    :	-
	Return              :	player which wins the round
	---------------------------------------------------------------------------------*/
	Player winning_player() const
	{
		float highest = 0;
		int mark_count = 0;
		Player winner;
		vector<float> marks;
		vector<Player> winners;
		
		for (int i = 0; i<int(players.size()); i++)
			marks.push_back(give_marks(players[i]));

		for (int i = 0; i<int(marks.size()); i++)
			if (marks[i] > highest)
				highest = marks[i];

		for (int i = 0; i<int(marks.size()); i++)
			if (marks[i] == highest)
			{
				mark_count++;
				winners.push_back(players[i]);
			}

		if (mark_count == 1)
			winner = winners[0];
		else
			for (int i = 0; i<int(winners.size()); i++)
				if (winners[i].marks > winner.marks)
					winner = winners[i];
		return winner;
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	score_card
	Description         :	output streams the score board of a round
	Input Parameters    :	output - stream to output
							p - vector of the all game players
							winner - name of the round winning player
	Return              :	-
	---------------------------------------------------------------------------------*/
	void score_card(ostream &output, vector<Player> &p, string &winner) const
	{
		output << "\nRound winner : " << winner;
		output << "\n______________________________________________________________\n\n";
		output << "\tPoker Game Score Card after round " << round_no << endl;
		output << "______________________________________________________________\n";
		output << "\tPlayer\t\tMarks\t\tWin probability\n";
		output << "______________________________________________________________\n\n";
		for (int i = 0; i<int(p.size()); i++)
			output << "\t" << p[i].name << "\t" << p[i].marks << "\t\t" << p[i].win_probability << endl;
		output << "______________________________________________________________\n\n";
	}

	/*---------------------------------------------------------------------------------
	Member Function		:	print_log_scores
	Description         :	outputs round details to the log file
	Input Parameters    :	d - player dealer of the round
							p - vector of the all game players
							winner - name of the round winning player
	Return              :	-
	---------------------------------------------------------------------------------*/
	void print_log_scores(Player &d, vector<Player> &p, string &winner) const
	{
		ofstream log;
		log.open("Poker_Log.txt", ios::app);
		log << "\nRound " << round_no;
		log << "\n\nDealer's hand : ";
		for (int i = 0; i<int(d.hand.size()); i++)
		{
			d.hand[i].print_card(log);
			log << "  ";
		}
		log << endl << "Entered players : ";
		for (int i = 1; i<int(players.size()); i++)
			log << players[i].name << "  ";
		log << endl;
		score_card(log, p, winner);
		log.close();
	}
};


int main()
{
	srand(time(NULL));

	//initializing the game variables
	const int num_of_players = 4;
	const int num_of_rounds = 10;
	float initial_probability = float(1) / num_of_players;
	vector<Player> players;

	//printing header to console
	cout << "Poker Game\n\nThis game is played by 4 players for 10 rounds.\nUsed S for Spades, D for Diamonds, H for Hearts, C for Clubs.\n";
	cout << "______________________________________________________________\n\n";
	
	//printing header to log file
	ofstream log;
	log.open("Poker_Log.txt", ios::app);
	log << "----------------------------------------------------------------------------------\n";
	log << "Poker Game Started on " << get_now();
	log << "\n\nThis game is played by 4 players for 10 rounds.\nUsed S for Spades, D for Diamonds, H for Hearts, C for Clubs.\n";
	log << "______________________________________________________________\n\n";
	log.close();
	
	//creating players (including dealer)
	Player dealer("Dealer");
	for (int i = 0; i < num_of_players; i++)
		players.push_back(Player("Player " + to_string(i + 1)));

	//iterating for the number of rounds
	for (int n = 1; n <= num_of_rounds; n++)
	{
		cout << "\nRound " << n;

		//initializing a card pack to distribute
		Card_pack pack;
		pack.initialize_card_pack();
		pack.shuffle();
		pack.remove_two();
			
		//distributing the cards for players
		dealer.hand = pack.give_cards(5);
		for (int i = 0; i < num_of_players; i++)
			players[i].hand = pack.give_cards(5);

		//printing dealers hand to the console
		cout << "\n\nDealer's hand : ";
		for (int i = 0; i<int(dealer.hand.size()); i++)
		{
			dealer.hand[i].print_card(cout);
			cout << "  ";
		}
		cout << endl;

		//initializing vector to store available players for the round
		vector<Player> avail_players;

		//dealer always enters the round
		avail_players.push_back(dealer);

		//asking user to choose the players entering a round
		cout << "\nEnter '0' to withdraw or '1' to enter round\n";
		for (int i = 0; i < num_of_players; i++)
		{
			int avail;
			do
			{
				cin.clear();
				avail = 2;
				cout << "Player " << i + 1 << " : ";
				cin >> avail;
			} while (avail != 1 && avail != 0);
			//entering user choosen players to the round
			if (avail == 1)
				avail_players.push_back(players[i]);
		}

		//initializing the Poker round with the available players (including dealer)
		Poker_round round(n, avail_players);

		//choosing the winner of the round
		Player winner = round.winning_player();

		//giving marks for the players entered the round
		for (int i = 0; i < int(avail_players.size()); i++)
		{
			if (avail_players[i].name == winner.name)
				avail_players[i].marks += 10;
			else
				avail_players[i].marks -= 1;
		}
		//marks of the dealer is not calculated
		avail_players[0].marks = 0;
		
		//updating the players entered the round
		for (int i = 0; i < int(avail_players.size()); i++)
		{
			for (int j = 0; j < int(players.size()); j++)
				if (avail_players[i].name == players[j].name)
					players[j] = avail_players[i];
		}

		//calculating the maximum and minimum marks of players
		int mx = 0, mn = 100;
		for (int i = 0; i < int(players.size()); i++)
		{
			if (players[i].marks < mn)
				mn = players[i].marks;
			if (players[i].marks > mx)
				mx = players[i].marks;
		}

		//assigining winning probability according to the marks and the number of rounds
		for (int i = 0; i < int(players.size()); i++)
			players[i].win_probability = ((float(players[i].marks - mn) / (mx - mn)) * (float(n) / num_of_rounds) + initial_probability * (num_of_rounds - n) / num_of_rounds);

		//printing round details to the console and log file
		round.score_card(cout, players, winner.name);
		round.print_log_scores(dealer, players, winner.name);
	}

	//printing footer to the log file
	log.open("Poker_Log.txt", ios::app);
	log << "\nPoker Game Ended on " << get_now() << endl;
	log << "----------------------------------------------------------------------------------\n\n\n\n";
	log.close();

	return 0;
}