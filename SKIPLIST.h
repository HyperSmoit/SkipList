// Copyright 2019 Descultu Cristian Petrisor
#ifndef _HOME_STUDENT_SKIPLIST_H_
#define _HOME_STUDENT_SKIPLIST_H_
template <typename T>
class Node {
 public:
	int id, points;
	Node<T> **next;

	Node(int level, int id, int points) {
		this->id = id;
		this->points = points;
		next = new Node<T>*[level + 1];
		for (int i = 0; i < level + 1; i++) {
			next[i] = 0;
		}
	}
	~Node() {
		delete[] next;
	}
	Node(const Node &other) {
		this->id = other.id;
		this->points = other.points;
	}
};
template <typename T>
class Skiplist {
 public:
	Node<T> *head;
	int level;
	int MaxLevel;

	explicit Skiplist(int n) {
		MaxLevel = log(n);
		this->level = 0;
		head = new Node<T>(MaxLevel, minimum, minimum);
	}

	~Skiplist() {
		Node<T> *it = head->next[0];
		while (it != NULL) {
			Node<T> *temp = it;
			it = it->next[0];
			delete temp;
		}
		delete head;
	}
	void InsertPlayer(int id, int points) {
		Node<T> *it = head;
		Node<T> *way[MaxLevel + 1];
		for (int i = 0; i < MaxLevel + 1; i++) {
			way[i] = 0;
		}
		for (int i = level; i >= 0; --i) {
			while (it->next[i] != NULL && it->next[i]->points <= points) {
				if((it->next[i]->points == points && it->next[i]->id < id)) {
					break;
				} else {
					it = it->next[i];
				}
			}
			way[i] = it;
		}
		it = it->next[0];
		if (it == NULL || it->points != points || it->id < id) {
			int lvl = Coin_flip();
			if (lvl > level) {
				for (int i = level + 1; i < lvl + 1; ++i) {
					way[i] = head;
				}
				level = lvl;
			}
			Node<T> *temp = new Node<T>(level, id, points);
			for (int i = 0; i <= lvl; ++i) {
				temp->next[i] = way[i]->next[i];
				way[i]->next[i] = temp;
			}
		}
	}

	void Delete_Player(int id, int points) {
		Node<T> *it = head;
		Node<T> *way[MaxLevel + 1];
		for (int i = 0 ; i < MaxLevel + 1; ++i) {
			way[i] = 0;
		}
		for (int i = level; i >= 0; --i) {
			while (it->next[i] != NULL && it->next[i]->points <= points) {
				if((it->next[i]->points == points && it->next[i]->id == id) ||
				 (it->next[i]->points == points && it->next[i]->id < id)) {
					break;
				} else {
					it = it->next[i];
				}
			}
			way[i] = it;
		}
		it = it->next[0];
		if (it != NULL && it->id == id && it->points == points) {
			for (int i = 0; i <= level; ++i) {
				if (way[i]->next[i] != it) {
					break;
				}
				way[i]->next[i] = it->next[i];
			}
			delete it;
			while (level > 0 && head->next[level] == NULL) {
				level--;
			}
		}
	}
	int Search(int id) {
		Node<T> *it = head->next[0];
		while (it) {
			if(id == it -> id) {
				break;
			}
			it = it->next[0];
		}
		return it->points;
	}
	int Coin_flip() {
		float h_or_t = (float)rand()/RAND_MAX;
		float probability = 1./2;
		int lvl = 0;
		while (h_or_t < probability && lvl < MaxLevel) {
			lvl++;
			h_or_t = (float)rand()/RAND_MAX;
		}
	return lvl;
	}
	void PrintList(std::ofstream &out, int nr_players, int lvldiff_print[],
	 int &printed) {
		int player_id[nr_players] = {0}, player_points[nr_players] = {0};
		Node<T> *it = head->next[0];
		for (int i = 1; i <= nr_players; ++i) {
			player_points[i] = it->points;
			player_id[i] = it->id;
			it = it->next[0];
		}
		for (int i = nr_players; i > 0; --i) {
			if (!printed) {
				out << player_id[i] << ' ' << player_points[i] << ' '
				 << printed << std::endl;
			} else {
				out << player_id[i] << ' ' << player_points[i] << ' '
				 << i- lvldiff_print[player_id[i]] << std::endl;
			}
			lvldiff_print[player_id[i]] = i;
		}
		printed = 1;
	}
	void getLast(int player_ranking[], int nr_players) {
		Node<T> *it = head->next[0];
		for (int i = nr_players; i > 0; --i) {
			player_ranking[it->id] = i;
			it = it->next[0];
		}
	}
};

#endif  // _HOME_STUDENT_SKIPLIST_H_
