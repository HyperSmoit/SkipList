// Copyright 2019 Descultu Cristian Petrisor
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#define minimum -1000000
#define maximum 1000000
#include "SKIPLIST.h"

void Last_rankings(int player_time[], int player_id[], int player_ranking[],
 int nr_players) {
	for (int i = 0; i < nr_players - 1; ++i) {
		for (int j = i + 1; j < nr_players; j++) {
			if(player_time[i] == player_time[j]) {
				if(player_ranking[player_id[i]] > player_ranking[player_id[j]]) {
					int aux = player_time[i];
					player_time[i] = player_time[j];
					player_time[j] = aux;
					aux = player_id[i];
					player_id[i] = player_id[j];
					player_id[j] = aux;
				}
			}
		}
	}
}


void Sort_time_id(int player_time[], int player_id[], int nr_players) {
	for (int i = 0; i < nr_players - 1; ++i) {
		for (int j = i + 1; j < nr_players; j++) {
			if (player_time[i] > player_time[j]) {
				int aux;
				aux = player_time[i];
				player_time[i] = player_time[j];
				player_time[j] = aux;
				aux = player_id[i];
				player_id[i] = player_id[j];
				player_id[j] = aux;
			}
		}
	}
}

void Final_Ranking(std::ifstream &in, int nr_players, Skiplist<int> *list,
 std::ofstream &out) {
	std::string line;
	int player_time[nr_players] = {0}, player_id[nr_players] = {0},
	 player_ranking[nr_players] = {0}, lvldiff_print[nr_players] = {0};
	int printed = 0;
	int p_time_cont = 1;
	int p_time_diff0 = 0;
	for (int i = 0; i < nr_players; ++i) {
		player_id[i] = i + 1;
	}
	for (int i = 0; i < nr_players; ++i) {
		list->InsertPlayer(i + 1, 0);
	}
	list->getLast(player_ranking, nr_players);
	getline(in, line);
	while (!in.eof()) {
		getline(in, line);
		if (line == "print") {
			list->PrintList(out, nr_players, lvldiff_print, printed);
			out << std::endl;
		} else {
			int points_diff = 0;
			p_time_diff0 = 0;
			player_time[0] = atoi(&line[0]);
			for (unsigned i = 1; i < line.length(); ++i) {
				if (line[i] != ' ' && line[i - 1] == ' ') {
					player_time[p_time_cont] = atoi(&line[i]);
					p_time_cont++;
				}
			}
			for (int i = 0; i < p_time_cont; ++i) {
				player_id[i] = i + 1;
				if (player_time[i] != 0) {
					p_time_diff0++;
				}
			}
			Sort_time_id(player_time, player_id, nr_players);
			Last_rankings(player_time, player_id, player_ranking, nr_players);
			for (int i = 0; i < p_time_cont; ++i) {
				int points_per_race = p_time_diff0 / 2 - points_diff;
				if (p_time_diff0 % 2 == 0 && points_per_race == 0) {
					points_per_race--;
					points_diff++;
				}
				if (player_time[i] != 0) {
					points_per_race += list->Search(player_id[i]);
					list->Delete_Player(player_id[i], list->Search(player_id[i]));
					list->InsertPlayer(player_id[i], points_per_race);
					points_diff++;
				}
			}
		}
		p_time_cont = 1;
		list->getLast(player_ranking, nr_players);
	}
}

int main() {
	int nr_players = 0, nr_races = 0, nr_prints = 0;
	std::ifstream in("races.in");
	std::ofstream out("races.out");
	in >> nr_players >> nr_races >> nr_prints;

	Skiplist<int> *list = new Skiplist<int>(nr_players);
	Final_Ranking(in, nr_players, list, out);
	delete list;
	in.close();
	out.close();

	return 0;
}
